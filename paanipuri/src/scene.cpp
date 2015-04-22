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
    
    numberOfParticles = 1;
    
    particleSystem = new ParticleSystem();
    
    cube = new Cube();
    cube->setCenter(glm::vec3(0,0,0));
    cube->setDimension(glm::vec3(13)*particleSystem->getSmoothingRadius());
    cube->setCellSize(particleSystem->getSmoothingRadius());       //depends on cube dimensions and particle radius
    
}

void Scene::init(){
    glm::vec3 position(0), velocity(0);
    
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
    glm::vec3 position(0), velocity(0);
    
    int phase = 1;
    float mass = 4.f;
    
    switch (type) {

        case 1:
            phase = 1;
            mass = 100.f;
            break;
            
        case 2:
            phase = 0;
            mass = 1.f;
            break;
            
        default:
            break;
    }
    
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

void Scene::addCubeToScene()
{
    int particleCount = 27;

    int damDim = static_cast <int> (std::cbrt(particleCount)),
    i,j,k=0;
    float smoothingRad = 1.f;
    glm::vec3 position(0), velocity(0);
    
    int phase = 3;
    float mass = 100.f;
    
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

void Scene::addBallToScene()
{
    //reference: http://stackoverflow.com/questions/9084189/draw-a-sphere-using-3d-pixels-voxels
    
    float radius = 5.0f;
    int lats=10, longs=10;
    int particleCount = 0;

    glm::vec3 position1(0), position2(0), velocity(0);

    int phase = 2;
    float mass = 100.f;

    int i, j;
    for (i = 0; i <= lats; i++)
    {
        double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);
        
        double lat1 = PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        
        for (j = 0; j <= longs; j++)
        {
            double lng = 2 * PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            
//            plot(x * zr0, y * zr0, z0);
//            plot(x * zr1, y * zr1, z1);
            
            position1 = glm::vec3(x * zr0, y * zr0, z0);
            position2 = glm::vec3(x * zr1, y * zr1, z1);
            
            particleSystem->addParticle(Particle(position1, velocity, phase, mass));
            particleSystem->addParticle(Particle(position2, velocity, phase, mass));
            
            particleCount+=2;
        }
    }
    
    numberOfParticles += particleCount;
}

void Scene::update(){
    
    particleSystem->update();
}