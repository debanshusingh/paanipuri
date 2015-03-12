//
//  ParticleSystem.cpp
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "ParticleSystem.h"

//Getter functions
std::vector<Particle> ParticleSystem::getAllParticles()
{
    return particles;
}

float ParticleSystem::getRestDensity()
{
    return restDensity;
}

unsigned int ParticleSystem::getParticleCount()
{
    return static_cast<int> (getAllParticles().size());
}

//Other functions
void ParticleSystem::addParticle(Particle p)
{
    particles.push_back(p);
}

Particle ParticleSystem::getParticle(int index)
{
    return particles[index];
}


//TODO
// Improve the neighbos search later
// Currently naive neighbor searching

std::vector <ParticleSystem::Neighbor> ParticleSystem::findNeighbors(int index, float radius)
{
    std::vector<Neighbor> neighborsList;
    glm::vec3 particlePos = particles[index].getPosition();
    
    float distance;
    int i;
    Neighbor p;

    for (i = 0; i<particles.size(); i++)
    {
        distance = glm::distance(particlePos, particles[i].getPosition());

        if(distance < radius + EPSILON)
        {
            if(i!=index)
            {
                p.first = i;
                p.second = particlePos - particles[i].getPosition();
                neighborsList.push_back(p);
            }
        }
    }
    
    return neighborsList;
}

float ParticleSystem::getDensity(int index, float smoothingRadius)
{
    float density = 0;
    int i;
    
    std::vector<Neighbor> neighbors = this->findNeighbors(index, smoothingRadius);
    
    //Kernel function implementation
    //  Using poly6 kernel function
    
    //TODO
    //  Currently mass is 1 for all particles
    //  If mass is changed, change the for loop to multiply by mass
    for(i=0; i<neighbors.size(); i++)
    {
        density +=  poly6 * pow((pow(smoothingRadius, 2) - pow(neighbors[i].second.length(), 2)),3)
                            / pow(smoothingRadius, 9);
    }
    
    return density;
}


glm::vec3 wPoly6Kernel(glm::vec3 distance, float smoothingRadius) {
    return glm::vec3(0,0,0);
};



















