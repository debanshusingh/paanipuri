//
//  ParticleSystem.cpp
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "ParticleSystem.h"
#include <iostream>

int counter = 0;

//Getter functions
std::vector<Particle> ParticleSystem::getAllParticles()
{
    return particles;
}

float ParticleSystem::getRestDensity()
{
    return restDensity;
}

void ParticleSystem::addParticle(Particle p)
{
    particles.push_back(p);
}

Particle ParticleSystem::getParticle(int index)
{
    return particles[index];
}

glm::vec3 ParticleSystem::getForces()
{
    return forces;
}

void ParticleSystem::setForces(glm::vec3 f)
{
    forces = f;
}

glm::vec3 ParticleSystem::getLowerBounds()
{
    return lowerBounds;
}

glm::vec3 ParticleSystem::getUpperBounds()
{
    return upperBounds;
}

void ParticleSystem::setLowerBounds(glm::vec3 l)
{
    lowerBounds = l;
}

void ParticleSystem::setUpperBounds(glm::vec3 u)
{
    upperBounds = u;
}

float ParticleSystem::getCellSize()
{
    return cellSize;
}

void ParticleSystem::setCellSize(float size)
{
    cellSize = size;
    gridDim = (upperBounds-lowerBounds) / cellSize;
}

//TODO
// Improve the neighbors search later
// Currently naive neighbor searching

std::vector <ParticleSystem::Neighbor> ParticleSystem::findNeighbors(int index)
{
    std::vector<Neighbor> neighborsList;
    Particle & currParticle = particles[index];
    
    glm::vec3 particlePredictedPos = currParticle.getPredictedPosition();
    
    glm::ivec3 particleHashPosition = currParticle.getHashPosition();
    int gridLocation = particleHashPosition.x + gridDim.x * (particleHashPosition.y + gridDim.y * particleHashPosition.z);
    
    int i,k;
    Neighbor p;
    
    currParticle.clearNeighbors();
    
    //check neighbors in same cell
    
    std::vector<int> & pointerToGrid = hashGrid[gridLocation];
    glm::vec3 vectorToNeighbor;

    for(i = 0; i<pointerToGrid.size(); i++)
    {
        k = pointerToGrid[i];
        
        if(k!=index)
        {
            vectorToNeighbor = particlePredictedPos - particles[k].getPredictedPosition();
            
            if(glm::length(vectorToNeighbor) < smoothingRadius + EPSILON)
            {
                p.first = k;
                currParticle.addNeighborIndex(k);
                
                p.second = vectorToNeighbor;
                neighborsList.push_back(p);
            }
        }
    }
    
    return neighborsList;
}

void ParticleSystem::initialiseHashPositions()
{
    glm::ivec3 hashPosition;
    
    hashGrid.clear();

    for (int i=0; i<particles.size(); i++)
    {
        hashPosition = (particles[i].getPredictedPosition() + upperBounds) / cellSize;
        particles[i].setHashPosition(hashPosition);
        int position = hashPosition.x + gridDim.x * (hashPosition.y + gridDim.y * hashPosition.z);
        
        
//        if(hashGrid[position].size() < 20)
//        {
//            hashGrid[position].push_back(i);
//        }
        
        std::vector<glm::ivec3> neighborCells;
//
//        //x
        neighborCells.push_back(hashPosition);
        neighborCells.push_back(glm::ivec3(0,1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,-1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,0,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,-1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,0,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,1,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(0,-1,-1) + hashPosition);
        
        //x+1
        neighborCells.push_back(glm::ivec3(1,0,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,-1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,0,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,-1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,0,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,1,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(1,-1,-1) + hashPosition);
        
        //x-1
        neighborCells.push_back(glm::ivec3(-1,0,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,-1,0) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,0,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,-1,1) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,0,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,1,-1) + hashPosition);
        neighborCells.push_back(glm::ivec3(-1,-1,-1) + hashPosition);
        
        for(int j = 0; j < neighborCells.size(); j++)
        {
            if(isValidCell(neighborCells[j]))
            {
                hashGrid[neighborCells[j].x + gridDim.x * (neighborCells[j].y + gridDim.y * neighborCells[j].z)].push_back(i);
            }
        }
    }
}

bool ParticleSystem::isValidCell(glm::ivec3 cellForCheck)
{
    if(cellForCheck.x >= 0 && cellForCheck.x < gridDim.x)
    {
        if(cellForCheck.y >= 0 && cellForCheck.y < gridDim.y)
        {
            if(cellForCheck.z >= 0 && cellForCheck.z < gridDim.z)
            {
                return true;
            }
        }
    }
    
    return false;
}


float ParticleSystem::getDensity(int index)
{
    float density = 0;
    int i;
    
    std::vector<int> neighbors = particles[index].getNeighborIndices();
    
    //Kernel function implementation
    //  Using poly6 kernel function

    //TODO
    //  Currently mass is 1 for all particles
    //  If mass is changed, change the for loop to multiply by mass
    for(i=0; i<neighbors.size(); i++)
    {
        density +=  wPoly6Kernel((particles[index].getPredictedPosition() - particles[neighbors[i]].getPredictedPosition()), smoothingRadius);
    }
    
    return density;
}

float ParticleSystem::wPoly6Kernel(glm::vec3 distance, float smoothingRadius)
{
    float x = (smoothingRadius*smoothingRadius) - (glm::length(distance)*glm::length(distance));
    float x_3 = x*x*x;

    return poly6Const * x_3/s_9;
}

glm::vec3 ParticleSystem::gradientWSpikyKernel(glm::vec3 distance, float smoothingRadius)
{
    float distanceLength = glm::length(distance);

    float x = (smoothingRadius-distanceLength)*(smoothingRadius-distanceLength);
    
    float gradientW = spikyConst * (1.0f/s_6) * x * 1.0f/(distanceLength+EPSILON);

    return gradientW*distance;
}

glm::vec3 ParticleSystem::gradientConstraintAtParticle(int index)
{
    glm::vec3 gradientReturn(0,0,0);
    float restDensityInverse = 1.0/restDensity;
    
    std::vector<int> neighbors = particles[index].getNeighborIndices();
    
    for(int i=0; i<neighbors.size(); i++)
    {
        gradientReturn += restDensityInverse *
                        gradientWSpikyKernel((particles[index].getPosition() - particles[neighbors[i]].getPosition()), smoothingRadius);
    }
    
    return gradientReturn;
}

glm::vec3 ParticleSystem::gradientConstraintForNeighbor(int index, int neighborIndex)
{
    glm::vec3 gradientReturn(0,0,0);
    float restDensityInverse = 1.0/restDensity;

    
    gradientReturn = restDensityInverse * gradientWSpikyKernel(particles[index].getPosition() - particles[neighborIndex].getPosition(), smoothingRadius);
    
    return (-1.0f * gradientReturn);
}

void ParticleSystem::update()
{
//    counter ++;
    
    applyForces(); // apply forces and predict position
    initialiseHashPositions();  //initialise hash positions to be used in neighbour search
    
    for (int i=0; i<particles.size(); i++) {
        findNeighbors(i);
    }

    for (int k=0; k<solverIterations; k++) {

        for (int i=0; i<particles.size(); i++) {

            findLambda(i);
        }
       
        for (int i=0; i<particles.size(); i++) {

            Particle & currParticle = particles[i];
            glm::vec3 deltaPi = findDeltaPosition(i);
            currParticle.setDeltaPi(deltaPi);
            currParticle.setPredictedPosition(currParticle.getPredictedPosition() + currParticle.getDeltaPi());
            particleCollision(i);

        }
        
    }

    for (int i=0; i<particles.size(); i++) {
        
        Particle & currParticle = particles[i];

        currParticle.setVelocity((currParticle.getPredictedPosition() - currParticle.getPosition()) / timeStep);
        currParticle.setPosition(currParticle.getPredictedPosition());
    }
}

void ParticleSystem::findLambda(int index){
    std::vector<int> neighbours = particles[index].getNeighborIndices();
    
    int numNeighbours = static_cast<int>(neighbours.size());
    
    float sumGradientAtParticle = 0.0f;
    float gradientNeighbour = 0.0f;
    
    for (int i=0; i<numNeighbours; i++) {
        gradientNeighbour = glm::length(gradientConstraintForNeighbor(index, i));
        sumGradientAtParticle += gradientNeighbour*gradientNeighbour;
    }
    
    float gradientParticle = glm::length(gradientConstraintAtParticle(index));
    sumGradientAtParticle += gradientParticle*gradientParticle;
    
    float currDensity = getDensity(index);
    float densityContraint = (currDensity/restDensity) - 1.0f;
    
    float lambdaI = -1.0f * (densityContraint/(sumGradientAtParticle+relaxation));
//    if (fabs(lambdaI)) std::cout<<lambdaI<<std::endl;
    particles[index].setLambda(lambdaI);
}

glm::vec3 ParticleSystem::findDeltaPosition(int index)
{
    glm::vec3 deltaPi(0,0,0);
    
    Particle & currParticle = particles[index];
    
    std::vector<int> neighbors = currParticle.getNeighborIndices();
    float lambda_i = currParticle.getLambda();
    float sCor = 0, k = 0.001, deltaQ = 0.1 * smoothingRadius;
//    int n = 4;
    float spikyTerm;
    
    for(int i=0; i<neighbors.size(); i++)
    {
        float temp = wPoly6Kernel(glm::vec3(deltaQ, 0, 0), smoothingRadius);
//        if (fabs(temp) < EPSILON) k = 0;
//        else k = 0.1;
        
        spikyTerm = wPoly6Kernel( (currParticle.getPredictedPosition() - particles[neighbors[i]].getPredictedPosition()), smoothingRadius) / temp;
        
       
        sCor = -1.0 * k * spikyTerm * spikyTerm * spikyTerm * spikyTerm;
        
        deltaPi += (particles[neighbors[i]].getLambda() + lambda_i + sCor) *
                    gradientWSpikyKernel((currParticle.getPredictedPosition() - particles[neighbors[i]].getPredictedPosition()), smoothingRadius);
    }
    
    return (deltaPi/restDensity);
}

void ParticleSystem::applyForces()
{
    for(int i=0; i<particles.size(); i++)
    {
        Particle & currParticle = particles[i];

        currParticle.setVelocity(currParticle.getVelocity() + timeStep * forces);
        glm::vec3 currPosition = currParticle.getPosition();
        glm::vec3 predictedPosition = currPosition + timeStep * currParticle.getVelocity();
        currParticle.setPredictedPosition(predictedPosition);
    }
}

void ParticleSystem::particleCollision(int index){
    particleBoxCollision(index);
    particleParticleCollision(index);
}

void ParticleSystem::particleParticleCollision(int index)
{
    //as per http://stackoverflow.com/questions/19189322/proper-sphere-collision-resolution-with-different-sizes-and-mass-using-xna-monog
    
    std::vector<int> neighbors = particles[index].getNeighborIndices();
    
    glm::vec3 currentParticlePosition = particles[index].getPredictedPosition(),
                neighborPosition,
                particleVelocity,
                neighborVelocity;

    
    glm::vec3 relativeVelocity,
                collisionNormal,
                vCollision;  //components of relative velocity about collision normal and direction
    
    
    float distance, radius = particles[index].getRadius();
    
    radius= smoothingRadius;
    for(int i=0; i<neighbors.size(); i++)
    {
        particleVelocity = particles[index].getVelocity();

        neighborPosition = particles[neighbors[i]].getPredictedPosition();
        neighborVelocity = particles[neighbors[i]].getVelocity();
        
        distance = glm::distance(currentParticlePosition, neighborPosition);
        
        if(distance < 2 * radius + EPSILON)
        {
            //resolve collision
            relativeVelocity = particleVelocity - neighborVelocity;
            
            collisionNormal = glm::normalize(currentParticlePosition - neighborPosition);
            
            vCollision = glm::dot(collisionNormal, relativeVelocity) * collisionNormal;
            
            particles[index].setVelocity(particleVelocity - vCollision);
            particles[neighbors[i]].setVelocity(neighborVelocity + vCollision);
        }
    }
}

void ParticleSystem::particleBoxCollision(int index)
{
    Particle & currParticle = particles[index];
    glm::vec3 particlePosition = currParticle.getPredictedPosition();

    float radius = currParticle.getRadius();
    float dampingFactor = 0.5f;
    
    if(particlePosition.x - radius < lowerBounds.x + EPSILON || particlePosition.x + radius > upperBounds.x - EPSILON)
    {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(-dampingFactor,1,1));
        currParticle.setPredictedPosition(currParticle.getPosition() + timeStep * currParticle.getVelocity());
    }

    if(particlePosition.y - radius < lowerBounds.y + EPSILON)
    {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1,-dampingFactor,1));
        currParticle.setPredictedPosition(currParticle.getPosition() + timeStep * currParticle.getVelocity());
    }

    if(particlePosition.y + radius > upperBounds.y - EPSILON)
    {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1,-dampingFactor,1));
        glm::vec3 pos = currParticle.getPredictedPosition();
        currParticle.setPredictedPosition(glm::vec3(pos.x, upperBounds.y - radius - EPSILON, pos.z));
    }

    if(particlePosition.z - radius < lowerBounds.z + EPSILON || particlePosition.z + radius > upperBounds.z - EPSILON)
    {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1,1,-dampingFactor));
        currParticle.setPredictedPosition(currParticle.getPosition() + timeStep * currParticle.getVelocity());
    }
}
