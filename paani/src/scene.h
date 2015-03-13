//
//  scene.h
//  paani
//
//  Created by Sanchit Garg on 3/12/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#ifndef __paani__scene__
#define __paani__scene__

#include <stdio.h>
#include <glm/glm.hpp>
#include "ParticleSystem.h"

void sceneInit();

class Cube
{
private:
    glm::vec3 dimensions;
    glm::vec3 center;

public:
    
    //constructors
    Cube();
    Cube(glm::vec3, glm::vec3); //Input center and dimensions
    
    // Getters
    glm::vec3 getDimensions();
    glm::vec3 getHalfDimensions();
    glm::vec3 getCenter();
    
    //Setters
    void setDimension(glm::vec3);
    void setCenter(glm::vec3);
};

class Scene
{
public:
    float timeStep;
    glm::vec3 gravity;
    int numberOfParticles;
    Cube cube;
};

#endif /* defined(__paani__scene__) */