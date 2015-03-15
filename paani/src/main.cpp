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
        
        glColor3f(1,1,1);
        
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
        
<<<<<<< HEAD
            glutSolidSphere(radius, 10, 10);
=======
            glutSolidSphere(0.5f, 10, 10);
>>>>>>> 150f2ba6fce9104568ea1b809fab21ab6d24c5f5
        glPopMatrix();
    }
}
