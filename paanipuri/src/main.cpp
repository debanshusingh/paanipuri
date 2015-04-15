//
//  main.cpp
//  paanipuri
//
//  Created by Debanshu on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "scene.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace glm;
Scene* scene;

GLFWwindow* gWindow;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint _textureId; //The id of the texture
GLuint shaderProgram;
GLuint locationPos;
GLuint locationCol;
GLuint locationNor;
GLuint unifProj;
GLuint unifView;
GLuint unifModel;
GLuint unifModelInvTr;
GLuint unifLightPos;
GLuint unifLightColor;
GLuint unifCamPos;

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

void handleKeypress(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q' :
        case 'Q' :
            exit(0);
    }
}

std::string textFileRead(const char *filename)
{
    // http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
    std::ifstream in(filename, std::ios::in);
    if (!in) {
        std::cerr << "Error reading file" << std::endl;
        throw (errno);
    }
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

void printShaderInfoLog(int shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL COMPILE ERROR" << std::endl;
    
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
    
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void printGLErrorLog()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << error << ": ";
        const char *e =
        error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
        error == GL_INVALID_ENUM                  ? "GL_INVALID_ENUM" :
        error == GL_INVALID_VALUE                 ? "GL_INVALID_VALUE" :
        error == GL_INVALID_INDEX                 ? "GL_INVALID_INDEX" :
        "unknown";
        std::cerr << e << std::endl;
        // Throwing here allows us to use the debugger stack trace to track
        // down the error.
#ifndef __APPLE__
        // But don't do this on OS X. It might cause a premature crash.
        // http://lists.apple.com/archives/mac-opengl/2012/Jul/msg00038.html
        throw;
#endif
    }
}

void printLinkInfoLog(int prog)
{
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL LINK ERROR" << std::endl;
    
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);
    
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void initShader(){
    // Read in the shader program source files
    
    std::string vertSourceS = textFileRead("./paanipuri/shaders/vert.glsl");
    std::string fragSourceS = textFileRead("./paanipuri/shaders/frag.glsl");
    
    const char *vertSource = vertSourceS.c_str();
    const char *fragSource = fragSourceS.c_str();
    
    // Tell the GPU to create new shaders and a shader program
    GLuint shadVert = glCreateShader(GL_VERTEX_SHADER);
    GLuint shadFrag = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();
    
    // Load and compiler each shader program
    // Then check to make sure the shaders complied correctly
    
    // - Vertex shader
    glShaderSource    (shadVert, 1, &vertSource, NULL);
    glCompileShader   (shadVert);
    printShaderInfoLog(shadVert);
    // - Diffuse fragment shader
    glShaderSource    (shadFrag, 1, &fragSource, NULL);
    glCompileShader   (shadFrag);
    printShaderInfoLog(shadFrag);
    
    // Link the shader programs together from compiled bits
    glAttachShader  (shaderProgram, shadVert);
    glAttachShader  (shaderProgram, shadFrag);
    glLinkProgram   (shaderProgram);
    printLinkInfoLog(shaderProgram);
    
    // Clean up the shaders now that they are linked
    glDetachShader(shaderProgram, shadVert);
    glDetachShader(shaderProgram, shadFrag);
    glDeleteShader(shadVert);
    glDeleteShader(shadFrag);
    
    // Find out what the GLSL locations are, since we can't pre-define these
    locationPos = glGetAttribLocation (shaderProgram, "vs_Position");
    //    locationNor    = glGetAttribLocation (shaderProgram, "vs_Normal");
    //    locationCol    = glGetAttribLocation (shaderProgram, "vs_Color");
    unifProj    = glGetUniformLocation(shaderProgram, "u_projection");
    unifView    = glGetUniformLocation(shaderProgram, "u_view");
    unifModel   = glGetUniformLocation(shaderProgram, "u_model");
    //    unifModelInvTr = glGetUniformLocation(shaderProgram, "u_ModelInvTr");
    //    unifLightPos   = glGetUniformLocation(shaderProgram, "u_LightPos");
    //    unifLightColor = glGetUniformLocation(shaderProgram, "u_LightColor");
    //    unifCamPos     = glGetUniformLocation(shaderProgram, "u_CamPos");
    
    //printGLErrorLog();
    
    // Camera setup
    glUseProgram(shaderProgram);
    
    glm::vec3 camEye = glm::vec3(0,2,50);
    glm::vec3 camCenter = glm::vec3(0,0,0);
    glm::vec3 camUp = glm::vec3(0,1,0);
    
    glm::mat4 view = glm::lookAt(camEye, camCenter, camUp);
    glUniformMatrix4fv(unifView, 1, GL_FALSE, &view[0][0]);
    
    glm::mat4 projection = glm::perspective<float>(50.0, (float)SCREEN_SIZE.x/SCREEN_SIZE.y, 0.1f, 100.0f);
    glUniformMatrix4fv(unifProj, 1, GL_FALSE, &projection[0][0]);
    
    glUseProgram(0);
    
    
}

void displayParticles()
{
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Tell the GPU which shader program to use to draw things
    // bind the program (the shaders)
    glUseProgram(shaderProgram);
    
    // set model matrix
    glm::mat4 model = glm::mat4();
    glUniformMatrix4fv(unifModel, 1, GL_FALSE, &model[0][0]);
    
    glBindVertexArray(gVAO);
    
    //bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three verticies into the VBO
    std::vector<glm::vec3> particlePosData;
    std::vector<Particle> particles = scene->particleSystem->getAllParticles();
    for (std::vector<Particle>::iterator it=particles.begin(); it < particles.end(); it++)
    {
        Particle particle = *it;
        glm::vec3 partPos = particle.getPosition();
        particlePosData.push_back(partPos);
    }
    
    glBufferData(GL_ARRAY_BUFFER, particlePosData.size()*sizeof(glm::vec3), &particlePosData[0], GL_DYNAMIC_DRAW);
    
    //    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(locationPos);
    glVertexAttribPointer(locationPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    const GLuint numIndices = scene->numberOfParticles;
    glDrawArrays(GL_POINTS, 0, numIndices);
    
    // unbind the VAO
    glBindVertexArray(0);
    // unbind the program
    glUseProgram(0);
    
    glfwSwapBuffers(gWindow);
    
}

void displayBackground()
{
    //    glm::vec3 dimensions = cube->getHalfDimensions();
    //
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, _textureId);
    //
    //    //Bottom
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glColor3f(0.4f, 0.4f, 0.4f);
    //    glBegin(GL_QUADS);
    //
    //    glNormal3f(0.0, 1.0f, 0.0f);
    //    glTexCoord2f(0.0f, 0.0f);
    //    glVertex3f(-dimensions.x, -dimensions.y, dimensions.z);
    //    glTexCoord2f(1.0f, 0.0f);
    //    glVertex3f(dimensions.x, -dimensions.y, dimensions.z);
    //    glTexCoord2f(1.0f, 1.0f);
    //    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    //    glTexCoord2f(0.0f, 1.0f);
    //    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    //
    //    glEnd();
    //
    //    //Back
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glColor3f(0.4f, 0.4f, 0.4f);
    //    glBegin(GL_QUADS);
    //
    //    glNormal3f(0.0, 0.0f, 1.0f);
    //    glTexCoord2f(0.0f, 0.0f);
    //    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    //    glTexCoord2f(1.0f, 0.0f);
    //    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    //    glTexCoord2f(1.0f, 1.0f);
    //    glVertex3f(dimensions.x, dimensions.y, -dimensions.z);
    //    glTexCoord2f(0.0f, 1.0f);
    //    glVertex3f(-dimensions.x, dimensions.y, -dimensions.z);
    //
    //    glEnd();
    //
    //    //Right
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glColor3f(0.4f, 0.4f, 0.4f);
    //    glBegin(GL_QUADS);
    //
    //    glNormal3f(-1.0, 0.0f, 0.0f);
    //    glTexCoord2f(0.0f, 0.0f);
    //    glVertex3f(dimensions.x, dimensions.y, -dimensions.z);
    //    glTexCoord2f(1.0f, 0.0f);
    //    glVertex3f(dimensions.x, dimensions.y, dimensions.z);
    //    glTexCoord2f(1.0f, 1.0f);
    //    glVertex3f(dimensions.x, -dimensions.y, dimensions.z);
    //    glTexCoord2f(0.0f, 1.0f);
    //    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    //
    //    glEnd();
    //
    //    //Left
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glColor3f(0.4f, 0.4f, 0.4f);
    //    glBegin(GL_QUADS);
    //
    //    glNormal3f(1.0, 0.0f, 0.0f);
    //    glTexCoord2f(0.0f, 0.0f);
    //    glVertex3f(-dimensions.x, dimensions.y, dimensions.z);
    //    glTexCoord2f(1.0f, 0.0f);
    //    glVertex3f(-dimensions.x, dimensions.y, -dimensions.z);
    //    glTexCoord2f(1.0f, 1.0f);
    //    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    //    glTexCoord2f(0.0f, 1.0f);
    //    glVertex3f(-dimensions.x, -dimensions.y, dimensions.z);
    //
    //    glEnd();
    //
}


void loadParticles() {
    
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    //bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three verticies into the VBO
    std::vector<glm::vec3> particlePosData;
    std::vector<Particle> particles = scene->particleSystem->getAllParticles();
    for (std::vector<Particle>::iterator it=particles.begin(); it < particles.end(); it++)
    {
        Particle particle = *it;
        glm::vec3 partPos = particle.getPosition();
        particlePosData.push_back(partPos);
    }
    
    glBufferData(GL_ARRAY_BUFFER, particlePosData.size()*sizeof(glm::vec3), &particlePosData[0], GL_DYNAMIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(locationPos);
    glVertexAttribPointer(locationPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void display() {
    
    scene->update();
    displayParticles();
}

void initGLFW(int argc, char* argv[]) {
    
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "paani", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    glfwMakeContextCurrent(gWindow);
    
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}

int main(int argc, char * argv[]) {
    
    initGLFW(argc, argv);
    
    initShader();
    
    scene = new Scene();
    scene->init();
    
    //    Image* image = loadBMP("./paani/src/white.bmp");
    //    _textureId = loadTexture(image);
    //    delete image;
    
    loadParticles();
    
    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        // draw one frame
        display();
    }
    
    // clean up and exit
    glfwTerminate();
    
    return 0;
}