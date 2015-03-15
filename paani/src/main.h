//
//  main.h
//  paani
//
//  Created by Debanshu on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#ifndef paani_main_h
#define paani_main_h

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLUT/GLUT.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imageloader.h"

void init(int argc, char* argv[]);
void display();
void displayParticles();
void handleKeypress(unsigned char key, int x, int y);
GLuint loadTexture(Image* image);

int width=640;
int height=480;


#endif
