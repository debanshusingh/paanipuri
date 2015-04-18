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
    
    //number of particles should be a cube (1,8,27...)
    numberOfParticles = 2;
    numberOfParticles *= (numberOfParticles*numberOfParticles);
    
    particleSystem = new ParticleSystem();
    
    cube = new Cube();
    cube->setCenter(glm::vec3(0,0,0));
    cube->setDimension(glm::vec3(30)*particleSystem->getSmoothingRadius());
    cube->setCellSize(particleSystem->getSmoothingRadius());       //depends on cube dimensions and particle radius
    
}

void Scene::init(){
    glm::vec3 position, velocity = glm::vec3(utilityCore::randomFloat(),0,0);
    
    float smoothingRad = 0.5f;//particleSystem->getSmoothingRadius() * 1.f;
    int damnDim = static_cast <int> (std::cbrt(numberOfParticles)),
    //    int damnDim = static_cast <int> (std::sqrt(numberOfParticles)),
    i,j,k=0;
    
    //set up dam break
    
    //  start with the highest y, and keep filling squares
    
    for(i=0; i<damnDim; i++)
    {
        for(j=0; j<damnDim; j++)
        {
            for(k=0; k<damnDim; k++)
            {
                position = (glm::vec3((i)*smoothingRad, j*smoothingRad, k*smoothingRad) - glm::vec3(float(damnDim) * smoothingRad/2.0f))*0.9f;
                //                position.z = 0.0f;
                particleSystem->addParticle(Particle(position, velocity));
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

void Scene::update(){
    
    particleSystem->update();
}