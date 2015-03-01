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
std::vector<int> ParticleSystem::findNeighbors(int index, int distance)
{
    std::vector<int> neighborsList;
    glm::vec3 particlePos = particles[index].getPosition();
    
    int i;
    
    for (i = 0; i<particles.size(); i++)
    {
        if(glm::distance(particlePos, particles[i].getPosition()) < distance)
        {
            if(i!=index)
            {
                neighborsList.push_back(i);
            }
        }
    }
    
    return neighborsList;
}