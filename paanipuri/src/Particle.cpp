//
//  Particle.cpp
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "Particle.h"

//static variables

//float Particle::mass = 1.0f;
//float Particle::radius = 1.0f;

//Constructors
Particle::Particle()
{
    this->position = glm::vec3(0,0,0);
    this->velocity = glm::vec3(0,0,0);
    this->predictedPosition = glm::vec3(0,0,0);
    this->deltaPi = glm::vec3(0,0,0);
}

Particle::Particle(glm::vec3 pos)
{
    this->position = pos;
    this->velocity = glm::vec3(0,0,0);
    this->predictedPosition = glm::vec3(0,0,0);
    this->deltaPi = glm::vec3(0,0,0);
}

Particle::Particle(glm::vec3 pos, glm::vec3 vel)
{
    this->position = pos;
    this->velocity = vel;
    this->predictedPosition = glm::vec3(0,0,0);
    this->deltaPi = glm::vec3(0,0,0);
}

//Getter functions
float Particle::getMass()
{
    return this->mass;
}

float Particle::getRadius()
{
    return this->radius;
}

glm::vec3 Particle::getPosition()
{
    return this->position;
}

glm::vec3 Particle::getVelocity()
{
    return this->velocity;
}

glm::vec3 Particle::getForces()
{
    return this->forces;
}

glm::vec3 Particle::getPredictedPosition()
{
    return this->predictedPosition;
}

float Particle::getLambda()
{
    return lambda;
}

glm::vec3 Particle::getDeltaPi()
{
    return deltaPi;
}

float Particle::getDensity(){
    return density;
}

void Particle::setDensity(float density)
{
    this->density = density;
}

std::vector<int> Particle::getNeighborIndices()
{
    return neighborIndices;
}

glm::ivec3 Particle::getHashPosition()
{
    return hashPosition;
}

//Setter functions
void Particle::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Particle::setVelocity(glm::vec3 vel)
{
    this->velocity = vel;
}

void Particle::setForces(glm::vec3 netForce)
{
    this->forces = netForce;
}

void Particle::setPredictedPosition(glm::vec3 pos)
{
    this->predictedPosition = pos;
}

void Particle::addNeighborIndex(int index)
{
    neighborIndices.push_back(index);
}

void Particle::clearNeighbors()
{
    neighborIndices.clear();
}

void Particle::setLambda(float l)
{
    lambda = l;
}

void Particle::setDeltaPi(glm::vec3 p)
{
    deltaPi = p;
}

void Particle::setHashPosition(glm::ivec3 p)
{
    hashPosition = p;
}

