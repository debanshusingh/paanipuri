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

//This class stores the list of particles and handles their interactions

class ParticleSystem
{
private:
    std::vector <Particle> particles;   //List of all particles in the system

public:
    
    //Getter functions
    std::vector<Particle> getAllParticles();   //Returns the list of all particles in the system
    
    
    //Other functions
    void addParticle(Particle);               //Add a particle to the system
    Particle getParticle(int index);            //Returns a particlular add some index
    
    //Function to return a list of all the neighbors within the specified distance
    std::vector<int> findNeighbors(int index, int distance);
};

#endif
