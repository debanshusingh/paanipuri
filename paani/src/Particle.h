//
//  Particle.h
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#ifndef paani_Particle_h
#define paani_Particle_h

#include <glm/glm.hpp>


//This class stores the properties of individual particles

class Particle
{
private:
    
    const float mass = 1.0f;    //Mass of all particles will be same
    const float radius = 1.0f;  //Radius of all particles will be same
    
    glm::vec3 position;     //Stores the current position of the particle
    glm::vec3 velocity;     //Stores the current velocity of the particle
    glm::vec3 forces;       //Stores the accumulated forces acting on the particle
    
public:
    
    //Constructors
    Particle();             //Default
    Particle(glm::vec3);    //Construct a particle at a specific position
    Particle(glm::vec3, glm::vec3); //Construct the particle with pos and vel
    
    //getter funcitons
    float getMass();
    float getRadius();
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec3 getForces();
    
    //setter functions
    void setPosition(glm::vec3);
    void setVelocity(glm::vec3);
    void setForces(glm::vec3);
};

#endif
