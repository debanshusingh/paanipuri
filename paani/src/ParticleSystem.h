//
//  ParticleSystem.h
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#ifndef __paani__ParticleSystem__
#define __paani__ParticleSystem__

#include <stdio.h>
#include "Particle.h"
#include <vector>
#include <utility>
#include "utilities.h"


//This class stores the list of particles and handles their interactions

class ParticleSystem
{

private:
    std::vector <Particle> particles;   //List of all particles in the system
    const float poly6 = 315 / (64 * PI);
    const float restDensity = 1000;
    
public:
    
    typedef std::pair<int, glm::vec3> Neighbor;
    
    //Getter functions
    std::vector<Particle> getAllParticles();   //Returns the list of all particles in the system
    float getRestDensity();
    unsigned int getParticleCount();
    
    //Other functions
    void addParticle(Particle);               //Add a particle to the system
    Particle getParticle(int index);            //Returns a particlular add some index
    
    //Function to return a list of all the neighbors within the specified distance
    // Stored as a pair of index and distance to the particle
    std::vector<Neighbor> findNeighbors(int index, float radius);
    
    //function that returns the density of a particle
    float getDensity(int index, float smoothingRadius);
};

#endif
