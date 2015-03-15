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

int main(int argc, char * argv[]) {
    
    init(argc, argv);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
//    glEnable(GL_LIGHT1); //Enable light #1
    glShadeModel(GL_SMOOTH);
    scene = new Scene();
    scene->init();

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

void display() {
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0,0,-60.0f);
    
    GLfloat ambientColor[] = {1.f, 1.f, 1.f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    GLfloat lightColor0[] = {1.f, 1.f, 1.f, 1.0f} ;// Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 10.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    //glutSolidSphere(0.5f, 10, 10);
    scene->particleSystem->update();
    scene->displayParticles();
    
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

void Scene::displayParticles()
{    
    unsigned int i;
    float radius = particleSystem->getParticle(0).getRadius();
    
    glm::vec3 position, color;

    for(i=0; i<numberOfParticles; i++)
    {
        position = particleSystem->getParticle(i).getPosition();
        //color = utilityCore::randomVec3();
        if(std::isnan(position.x) || std::isnan(position.y) || std::isnan(position.z))
        {
            std::cout<<" ";
        }
        
        glColor3f(0.39,0.57,1.0);
        
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
        
            glutSolidSphere(radius, 10, 10);

        glPopMatrix();
    }
}
