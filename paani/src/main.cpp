//
//  main.cpp
//  paani
//
//  Created by Debanshu on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include <iostream>
#include "main.h"

using namespace glm;

int main(int argc, char * argv[]) {
    
    init(argc, argv);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}


void init(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("paani");
    glutFullScreen();
    glutKeyboardFunc(handleKeypress);
    
    glewInit();
    
    
}

void display(){
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glutSolidSphere(0.5f, 10, 10);
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