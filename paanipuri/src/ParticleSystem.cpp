//
//  ParticleSystem.cpp
//  paani
//
//  Created by Sanchit Garg on 2/28/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "ParticleSystem.h"
#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

int counter = 0;

using namespace tbb;

//Getter functions
std::vector<Particle>& ParticleSystem::getAllParticles()
{
    return particles;
}

void ParticleSystem::addParticle(Particle p)
{
    particles.push_back(p);
}

glm::vec3 ParticleSystem::getForces()
{
    return forces;
}

float ParticleSystem::getSmoothingRadius()
{
    return smoothingRadius;
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

void ParticleSystem::findNeighbors(int index)
{
    std::vector<Neighbor> neighborsList;
    Particle& currParticle = particles.at(index);
    
    glm::vec3 particlePredictedPos = currParticle.getPredictedPosition();
        
    glm::ivec3 particleHashPosition = currParticle.getHashPosition();
    int gridLocation = particleHashPosition.x + gridDim.x * (particleHashPosition.y + gridDim.y * particleHashPosition.z);
    
    int i,k;
    Neighbor p;
    
    currParticle.clearNeighbors();
    
    //check neighbors in same cell
    
    glm::vec3 vectorToNeighbor;
    if (hashGrid.find(gridLocation) != hashGrid.end()){
        
        for(i = 0; i<hashGrid.at(gridLocation).size(); i++)
        {
            k = hashGrid.at(gridLocation).at(i);
            
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
    }
}

void ParticleSystem::initialiseHashPositions(int index)
{
    
    glm::vec3 temp = particles.at(index).getPredictedPosition() + upperBounds;
    glm::ivec3 hashPosition;
    hashPosition = temp / cellSize;
    particles.at(index).setHashPosition(hashPosition);
    
    std::vector<glm::ivec3> neighborCells;
    
    //x
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
            //should it be atomic?
            hashGrid[neighborCells[j].x + gridDim.x * (neighborCells[j].y + gridDim.y * neighborCells[j].z)].push_back(index);
//            int neighbourIndex = neighborCells[j].x + gridDim.x * (neighborCells[j].y + gridDim.y * neighborCells[j].z);
//            hashGrid[hashPosition.x + gridDim.x * (hashPosition.y + gridDim.y * hashPosition.z)].push_back(neighbourIndex);
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

void ParticleSystem::setupConstraints(){
    
    clearConstraints();
    
    for (int i=0; i<particles.size(); i++)
    {
        if(particles.at(i).getPhase() < 2)
        {
            DensityConstraint* dc = new DensityConstraint(i);
            densityConstraints.push_back(dc);
        }
        
        else
        {
            ShapeMatchingConstraint* sc = new ShapeMatchingConstraint(i);
            shapeConstraints.push_back(sc);
        }
    }
}

void ParticleSystem::clearConstraints()
{
//    for (unsigned int i = 0; i < densityConstraints.size(); i++)
//    {
//        delete densityConstraints.at(i);
//    }
    densityConstraints.clear();
    shapeConstraints.clear();
}

//==========================
// Main UPP solver algorithm
//==========================
void ParticleSystem::update()
{
    
    //run the parallel for from 0 to particles.size()
    parallel_for<size_t>(0, particles.size(), 1, [=](int i)
    {
        applyForces(i); // apply forces and set predicted position
//        applyMassScaling(i); //apply mass scaling
    });

    hashGrid.clear();
    // TODO: enable this when you have folly::AtomicHashMap which allows lock-free hash maps
//    parallel_for<size_t>(0, particles.size()-1, 1, [=](int i)
//    {
//        initialiseHashPositions(i);  //initialise hash positions to be used in neighbour search - SLOW!
//    });
    
    for (int i=0; i<particles.size(); i++) {
        initialiseHashPositions(i);
    }

    setupConstraints();
    
    setupInvMassMatrix();

    parallel_for<size_t>(0, particles.size(), 1, [=](int i)
    {
        findNeighbors(i);
//        findSolidContacts(i) //resolve contact constraints for stable init. config -> update original & predicted pos
    });
    
    for (int iter=0; iter<solverIterations; iter++) // outer loop can't be parallelized
    {
        //constraint-centric approach
        
        //solve densityConstraint group
        parallel_for<size_t>(0, densityConstraints.size(), 1, [=](int j)
        {
             densityConstraints.at(j)->findLambda(particles);
        });

        parallel_for<size_t>(0, densityConstraints.size(), 1, [=](int j)
        {
            //find constraintDelta*lambda and set in deltaPi
            densityConstraints.at(j)->Solve(particles); // 1 density constraint contains index of particle it acts on
         
            particleCollision(j);
            
            // for particles in constraint
            //update delta atomically
            Particle& currParticle = particles.at(densityConstraints.at(j)->getParticleIndex());
//            currParticle.setPredictedPosition(currParticle.getPredictedPosition() + currParticle.getDeltaPi());
            
//            currParticle.setPredictedPosition(currParticle.getPredictedPosition() + (invMassMatrix.coeff(j, j) * currParticle.getDeltaPi()));
            currParticle.setPredictedPosition(currParticle.getPredictedPosition() + (currParticle.getDeltaPi() / currParticle.getMass()));
                //currParticle.setPredictedPosition(currParticle.getPredictedPosition() + currParticle.getDeltaPi());
        });
        
        parallel_for<size_t>(0, shapeConstraints.size(), 1, [=](int j)
        {
            particleCollision(j);
            
            //Change this calculation later: save the groups when particles are added to the scene
//            std::vector<Particle> particleGroup;
            std::vector<int> particleGroup;
            
            Particle& currParticle = particles.at(shapeConstraints.at(j)->getParticleIndex());
            
            for(int i = 0; i<particles.size();i++)
            {
                if(currParticle.getPhase() == particles.at(i).getPhase())
                {
                    particleGroup.push_back(i);
                }
            }
            //----------------------------------------
            
            shapeConstraints.at(j)->Solve(particleGroup, particles);
            
//            Particle& currParticle2 = particles.at(j);

            currParticle.setPredictedPosition(currParticle.getPredictedPosition() + currParticle.getDeltaPi());
        });
    }
    
    parallel_for<size_t>(0, particles.size(), 1, [=](int i)
    {
        Particle& currParticle = particles.at(i);
     
        currParticle.setVelocity((currParticle.getPredictedPosition() - currParticle.getPosition()) / timeStep);
//        viscosity(i);
        currParticle.setPosition(currParticle.getPredictedPosition());
    });
    
    for (int i=0; i<particles.size(); i++)
    {
        Particle& currParticle = particles.at(i);
        glm::vec3 partPos = currParticle.getPosition();
        particlePosData.push_back(partPos);
        
        glm::vec3 partCol(1.0,1.0,1.0);
        if (currParticle.getPhase() == 1){
            partCol = glm::vec3(0.0,1.0,0.0);
        }
        particleColData.push_back(partCol);
    }
}

void ParticleSystem::applyForces(const int i)
{
    Particle& currParticle = particles.at(i);
    
    currParticle.setVelocity(currParticle.getVelocity() + timeStep * forces);
    glm::vec3 currPosition = currParticle.getPosition();
    glm::vec3 predictedPosition = currPosition + timeStep * currParticle.getVelocity();
    currParticle.setPredictedPosition(predictedPosition);
}


// TODO: move VISCOSITY to constraint.cpp
//void ParticleSystem::viscosity(int index)
//{
//    Particle & currParticle = particles.at(index);
//    std::vector<int> neighbors = currParticle.getNeighborIndices();
//    
//    glm::vec3 newVelocity(0.0,0.0,0.0);
//    glm::vec3 currVelocity = currParticle.getVelocity();
//    glm::vec3 currPosition = currParticle.getPredictedPosition();
//    
//    for(int i=0; i<neighbors.size(); i++)
//    {
//        if(particles.at(neighbors[i]).getDensity() > EPSILON)
//        {
//            newVelocity += (1.0f/particles.at(neighbors[i]).getDensity()) * (particles.at(neighbors[i]).getVelocity() - currVelocity) * wPoly6Kernel( (currPosition - particles.at(neighbors.at(i)).getPredictedPosition()), smoothingRadius);
//        }
//    }
//    
//    currParticle.setVelocity(currVelocity + 0.01f * newVelocity);
//}

void ParticleSystem::particleCollision(int index){
    particleBoxCollision(index);
//    particleContainerCollision(index);
}

void ParticleSystem::particleParticleCollision(int index)
{
    //as per http://stackoverflow.com/questions/19189322/proper-sphere-collision-resolution-with-different-sizes-and-mass-using-xna-monog

    Particle &currParticle = particles.at(index);
    
    std::vector<int> neighbors = currParticle.getNeighborIndices();

    glm::vec3 currentParticlePosition = currParticle.getPredictedPosition(),
                neighborPosition,
                particleVelocity,
                neighborVelocity;


    glm::vec3 relativeVelocity,
                collisionNormal,
                vCollision;  //components of relative velocity about collision normal and direction


    float distance, radius = currParticle.getRadius();

    radius= smoothingRadius;
    for(int i=0; i<neighbors.size(); i++)
    {
        particleVelocity = currParticle.getVelocity();

        neighborPosition = particles.at(neighbors.at(i)).getPredictedPosition();
        neighborVelocity = particles.at(neighbors.at(i)).getVelocity();

        distance = glm::distance(currentParticlePosition, neighborPosition);

        if(distance < 2 * radius + EPSILON)
        {
            //resolve collision
            relativeVelocity = particleVelocity - neighborVelocity;

            collisionNormal = glm::normalize(currentParticlePosition - neighborPosition);

            vCollision = glm::dot(collisionNormal, relativeVelocity) * collisionNormal;

            currParticle.setVelocity(particleVelocity - vCollision);
            particles.at(neighbors.at(i)).setVelocity(neighborVelocity + vCollision);
        }
    }
}

void ParticleSystem::loadContainer(Mesh& mesh)
{
    container.numIndices = mesh.getNumVertices(0);
    container.triangles = mesh.getTriangles(0);
    container.normals = mesh.getNormals(0);
    
    //TODO --------------------------------------
    //  scale triangles as per obj size
    
    glm::vec3 min(0.0), max(0.0);
    for(int i = 0; i<container.numIndices; ++i)
    {
        container.triangles[i] *= 10.0f;
        for(int j=0; j<3; j++)
        {
            if(container.triangles[i][j] < min[j])
                min[j] = container.triangles[i][j];
            if(container.triangles[i][j] > max[j])
                max[j] = container.triangles[i][j];
        }
    }
    
    container.boundingCenter = (min + max)/2.0f;
    container.boundingRadius = glm::distance(min, container.boundingCenter);
    
    createContainerGrid();
}

void ParticleSystem::createContainerGrid()
{
    int size = gridDim.x * gridDim.y * gridDim.z;
    
    for(int i = 0;i<size; ++i)
    {
        containerBool.push_back(false);
    }
    
    //voxel finding algo
    //  1. Determine the range of x, y, z the triangle spans.
    //  2. Mark all voxels with that range as true
    
    glm::vec3 v1, v2, v3;         //triangle vertices
    glm::vec3 min(0), max(0);     //saves the min and max x,y,z (triangle span)
    
    for(int i=0; i<container.triangles.size(); i+=3)
    {
        v1 = container.triangles.at(i) + upperBounds;
        v2 = container.triangles.at(i+1) + upperBounds;
        v3 = container.triangles.at(i+2) + upperBounds;
        
        min = v1;
        max = v1;
        
        for(int j = 0; j<3; ++j)
        {
            if(min[j] > v2[j])
                min[j] = v2[j];
            if(min[j] > v3[j])
                min[j] = v3[j];
            
            if(max[j] < v2[j])
                max[j] = v2[j];
            if(max[j] < v3[j])
                max[j] = v3[j];
        }
        
        glm::ivec3 rangeMin, rangeMax;
        
        rangeMin = min/cellSize;
        rangeMax = max/cellSize;
        rangeMax += glm::ivec3(1,1,1);
        
        for(int x = rangeMin.x; x<rangeMax.x; ++x)
        {
            for(int y = rangeMin.y; y<rangeMax.y; ++y)
            {
                for(int z = rangeMin.z; z<rangeMax.z; ++z)
                {
                    if(isValidCell(glm::ivec3(x,y,z)))
                    {
                        containerBool.at(x + gridDim.x * (y + gridDim.y * z)) = true;
                        containerGrid[x + gridDim.x * (y + gridDim.y * z)].push_back(i);
                    }
                }
            }
        }
    }
}

void ParticleSystem::particleContainerCollision(int index)
{
    Particle& currParticle = particles.at(index);
    glm::vec3 particlePredictedPos = currParticle.getPredictedPosition();
    glm::vec3 particlePos = currParticle.getPosition();
    
    glm::ivec3 hashPosition = currParticle.getHashPosition();
    int gridPosition = hashPosition.x + gridDim.x * (hashPosition.y + gridDim.y * hashPosition.z);
    
    if(gridPosition < containerBool.size())
    {
        if(containerBool.at(gridPosition))
        {
//            for(int i = 0; i<containerGrid.at(gridPosition).size(); ++i)
            parallel_for<size_t>(0, containerGrid.at(gridPosition).size(), 1, [=](int i)
            {
                Particle& currParticle = particles.at(index);
                glm::vec3 v0, v1, v2, n;
                float da, db;
                int triIndex;
                
                // triangle-particle collision
                triIndex = containerGrid.at(gridPosition).at(i);
                v0 = container.triangles.at(triIndex);
                v1 = container.triangles.at(triIndex + 1);
                v2 = container.triangles.at(triIndex + 2);
                n = container.normals.at(triIndex/3);
                
                da = glm::dot((v0-particlePos), n);
                db = glm::dot((v0-particlePredictedPos), n);
                
                if(da*db < ZERO_ABSORPTION_EPSILON)
                {
                    currParticle.setVelocity(glm::reflect(currParticle.getVelocity(), n) * 1.0f);
                    //                        currParticle.setPredictedPosition(particlePos + 1.f * timeStep * currParticle.getVelocity());
                    //                        currParticle.setPredictedPosition(particlePos);
                    
                    //if particle goes out of the mesh, increase the multiplying factor of timeStep*n
                    currParticle.setPredictedPosition(particlePos + 2.f * timeStep * n);
                }
            });
        }
    }
}

void ParticleSystem::particleBoxCollision(int index)
{
    Particle& currParticle = particles.at(index);
    
    glm::vec3 particlePosition = (currParticle.getPredictedPosition() + currParticle.getDeltaPi());
    
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

void ParticleSystem::setupInvMassMatrix() {
    //make the correct dimensions of the  matrix
    
    invMassMatrix.resize(particles.size(), particles.size());
    
    
    std::vector<SparseMatrixTriplet> i_triplets;
    std::vector<SparseMatrixTriplet> m_triplets;
    std::vector<SparseMatrixTriplet> invTriplets;
    i_triplets.clear();
    m_triplets.clear();
    for (int i = 0; i < particles.size(); i++)
    {
        float invMass = 1.0f / particles[i].getMass();
        
        i_triplets.push_back(SparseMatrixTriplet(i, i, 1));
        m_triplets.push_back(SparseMatrixTriplet(i, i, particles[i].getMass()));
        invTriplets.push_back(SparseMatrixTriplet(i, i, invMass));
    }
    
    
    invMassMatrix.setFromTriplets(invTriplets.begin(), invTriplets.end());
    int debug = 1;
}

glm::vec3 ParticleSystem::Eigen2GLM(const EigenVector3& eigen_vector)
{
    return glm::vec3(eigen_vector[0], eigen_vector[1], eigen_vector[2]);
}

EigenVector3 ParticleSystem::GLM2Eigen(const glm::vec3& glm_vector)
{
    return EigenVector3(glm_vector[0], glm_vector[1], glm_vector[2]);
}
