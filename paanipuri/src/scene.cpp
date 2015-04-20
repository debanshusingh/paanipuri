//
//  scene.cpp
//  paani
//
//  Created by Sanchit Garg on 3/12/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "scene.h"
#include <iostream>
#include "Mesh.h"

using namespace std;

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

float Cube::getCellSize()
{
    return cellSize;
}

void Cube::setCenter(glm::vec3 c)
{
    center = c;
}

void Cube::setDimension(glm::vec3 d)
{
    dimensions = d;
}

void Cube::setCellSize(float s)
{
    cellSize = s;
}

Scene::Scene()
{
    //need to add particles
    // create box
    
    gravity = glm::vec3(0.0,-10.0,0.0);
    
    numberOfParticles = 1000;
    
    particleSystem = new ParticleSystem();
    
    cube = new Cube();
    cube->setCenter(glm::vec3(0,0,0));
    cube->setDimension(glm::vec3(10)*particleSystem->getSmoothingRadius());
    cube->setCellSize(particleSystem->getSmoothingRadius());       //depends on cube dimensions and particle radius
    
}

void Scene::init(){
    glm::vec3 position, velocity;
    
    float smoothingRad = particleSystem->getSmoothingRadius() * 1.f;
    int damDim = static_cast <int> (std::cbrt(numberOfParticles)),
    i,j,k=0;
    
    //set up dam break
    
    //  start with the highest y, and keep filling squares
    
    int phase = 0;
    float mass = 1.f;
    for(i=0; i<damDim; i++)
    {
        for(j=0; j<damDim; j++)
        {
            for(k=0; k<damDim; k++)
            {
                position = (glm::vec3(i,j,k)*smoothingRad - glm::vec3(float(damDim) * smoothingRad/2.0f))*0.9f;
                particleSystem->addParticle(Particle(position, velocity, phase, mass));
            }
        }
    }
    
    particleSystem->setUpperBounds(cube->getCenter() + cube->getHalfDimensions());
    particleSystem->setLowerBounds(cube->getCenter() - cube->getHalfDimensions());
    particleSystem->setCellSize(cube->getCellSize());
    particleSystem->setForces(gravity);
    
    std::string objPath = "./paanipuri/objs/GlassBowl.obj";
    mesh.LoadMesh(objPath);
    
    particleSystem->loadContainer(mesh);
}

void Scene::addParticlesToScene(int type)
{
    int particleCount = 125;
    int damDim = static_cast <int> (std::cbrt(particleCount)),
        i,j,k=0;
    float smoothingRad = particleSystem->getSmoothingRadius() * 1.f;
    glm::vec3 position, velocity;
    
    int phase = 1;
    float mass = 10.f;
    
    for(i=0; i<damDim; i++)
    {
        for(j=0; j<damDim; j++)
        {
            for(k=0; k<damDim; k++)
            {
                position = (glm::vec3(i,j,k)*smoothingRad - glm::vec3(float(damDim) * smoothingRad/2.0f))*0.9f;
                particleSystem->addParticle(Particle(position, velocity, phase, mass));
            }
        }
    }
    
    numberOfParticles += particleCount;
}

void Scene::update(){
    
    particleSystem->update();
}