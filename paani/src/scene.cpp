//
//  scene.cpp
//  paani
//
//  Created by Sanchit Garg on 3/12/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "scene.h"
#include <iostream>

using namespace std;

Scene::Scene()
{
    //need to add particles
    // create box
    
    cube.setCenter(glm::vec3(0,0,0));
    cube.setDimension(glm::vec3(20));
    numberOfParticles = 750;
    gravity = glm::vec3(0.0,-10.0,0.0);
    
}

void Scene::init(){
    int i;
    glm::vec3 position, velocity = glm::vec3(0,0,0);
    particleSystem = new ParticleSystem();
    
    for(i=0; i<numberOfParticles; i++)
    {
        position = 0.8f*(utilityCore::randomVec3() * cube.getDimensions() - cube.getHalfDimensions());
        particleSystem->addParticle(Particle(position, velocity));
    }
}

//Cube class functions
Cube::Cube(glm::vec3 c, glm::vec3 d)
{
    center = c;
    dimensions = d;
}

glm::vec3 Cube::getCenter()
{
    return center;
}

glm::vec3 Cube::getDimensions()
{
    return dimensions;
}

glm::vec3 Cube::getHalfDimensions()
{
    return dimensions/2.0f;
}

void Cube::setCenter(glm::vec3 c)
{
    center = c;
}

void Cube::setDimension(glm::vec3 d)
{
    dimensions = d;
}