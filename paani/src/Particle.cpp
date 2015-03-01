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
}

Particle::Particle(glm::vec3 pos)
{
    this->position = pos;
}

Particle::Particle(glm::vec3 pos, glm::vec3 vel)
{
    this->position = pos;
    this->velocity = vel;
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