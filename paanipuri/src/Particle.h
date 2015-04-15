//
//  Particle.h
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#ifndef paani_Particle_h
#define paani_Particle_h

#include "glm/glm.hpp"
#include <vector>

//This class stores the properties of individual particles

class Particle
{
private:
    
    float mass = 1.0f;    //Mass of all particles will be same
    float radius = 0.5f;  //Radius of all particles will be same
    float density = 0.0f;
    
    //  Current values
    glm::vec3 position;     //Stores the current position of the particle
    glm::vec3 velocity;     //Stores the current velocity of the particle
    glm::vec3 forces;       //Stores the accumulated forces acting on the particle
    
    //predicted values
    glm::vec3 predictedPosition;    //Predicted particle position
    glm::vec3 deltaPi;
    
    glm::ivec3 hashPosition; //stores the grid index where the particle predicted position is
    std::vector <int> neighborIndices;    //stores the indices of all the neighbors
    float lambda;
    
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
    glm::vec3 getPredictedPosition();
    float getLambda();
    glm::vec3 getDeltaPi();
    glm::ivec3 getHashPosition();
    float getDensity();
    std::vector<int> getNeighborIndices();
    
    //setter functions
    void setPosition(glm::vec3);
    void setVelocity(glm::vec3);
    void setForces(glm::vec3);
    void setPredictedPosition(glm::vec3);
    void setLambda(float);
    void setDeltaPi(glm::vec3);
    void setHashPosition(glm::ivec3);
    void setDensity(float d);
    
    void addNeighborIndex(int index);
    void clearNeighbors();
    
};

#endif
