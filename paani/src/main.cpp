//
//  main.cpp
//  paani
//
//  Created by Debanshu on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include <iostream>
#include "main.h"
#include "scene.h"

using namespace glm;
Scene* scene;
GLuint _textureId; //The id of the texture

int main(int argc, char * argv[]) {
    
    init(argc, argv);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #0
    glEnable(GL_LIGHT2); //Enable light #0
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    scene = new Scene();
    scene->init();

     Image* image = loadBMP("/Users/sanchitgarg/Desktop/cggt/spring 2015/advanced cg/cis660pbf/code/paani/paani/src/white.bmp");
//        Image* image = loadBMP("./vtr.bmp");
    _textureId = loadTexture(image);
    delete image;
    
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}

void init(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("paani");
//    glutFullScreen();
    glutKeyboardFunc(handleKeypress);
    glewInit();
    time_t t;
    srand((unsigned int)time(&t));
}

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

void display() {
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(10,1,0,0);
    glTranslatef(0,-8,-50.0f);
    
    GLfloat ambientColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    GLfloat lightColor0[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos0[] = {0.0f, 0.0f, -15.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    GLfloat lightColor1[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos1[] = {10.0f, -12.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    
    GLfloat lightColor2[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos2[] = {-10.0f, -12.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    
    scene->update();
    scene->display();
    
    glutPostRedisplay();
    glutSwapBuffers();
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

void Scene::display()
{
    displayBackground();
    displayParticles();
}

void Scene::displayParticles()
{    
    unsigned int i;
    float radius = particleSystem->getParticle(0).getRadius();
    
    glm::vec3 position, color;
    GLfloat mat_diffused[] = {0.3,0.4,1.0};
    GLfloat mat_spec[] = {1.0,1.0,1.0};
    
    for(i=0; i<numberOfParticles; i++)
    {
        position = particleSystem->getParticle(i).getPosition();
        //color = utilityCore::randomVec3();
        if(std::isnan(position.x) || std::isnan(position.y) || std::isnan(position.z))
        {
            std::cout<<" ";
        }
        
        glColor3f(0.3,0.4,1.0);
        
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffused);
//            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
            glutSolidSphere(radius, 16, 16);

        glPopMatrix();
    }
}

void Scene::displayBackground()
{
    glm::vec3 dimensions = cube->getHalfDimensions();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    
    //Bottom
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    
    glNormal3f(0.0, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-dimensions.x, -dimensions.y, dimensions.z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dimensions.x, -dimensions.y, dimensions.z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    
    glEnd();

    //Back
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    
    glNormal3f(0.0, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dimensions.x, dimensions.y, -dimensions.z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-dimensions.x, dimensions.y, -dimensions.z);
    
    glEnd();

    //Right
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    
    glNormal3f(-1.0, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(dimensions.x, dimensions.y, -dimensions.z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dimensions.x, dimensions.y, dimensions.z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dimensions.x, -dimensions.y, dimensions.z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(dimensions.x, -dimensions.y, -dimensions.z);
    
    glEnd();
    
    //Left
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    
    glNormal3f(1.0, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-dimensions.x, dimensions.y, dimensions.z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-dimensions.x, dimensions.y, -dimensions.z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-dimensions.x, -dimensions.y, -dimensions.z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-dimensions.x, -dimensions.y, dimensions.z);
    
    glEnd();

}

