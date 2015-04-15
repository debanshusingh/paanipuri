#pragma once
#include "Particle.h"
#include <vector>
//#include "glm\glm\glm.hpp"
#include "Collision.h"
#include "Constraint.h"

using namespace std;

class Simulation {
private:
	vector<Particle> particles;
	//vector<Collision> _collisions;

	//various constraint groups
	//vector<vector<Constraint>> _rigidBodyConstraints; This way might be better, allow me to have more than one rigid body?
	vector<Constraint*> _rigidBodyConstraints; //for now this would assume just one rigid body in the scene I guess (still not sure how to group and how to use phase)
	vector<Constraint*> _fluidConstraints;

	float restitutionCoefficient;
	int stabilizationIterations;
	int solverIterations;
	float deltaT; //timestep
	int numberOfConstraintGroups; //constraint groups identify the different objects
	float sleepThreshold;
	int kCoefficient; //not sure what it is. paper doesn't explain it, just says it is usually between 1 and 5
	glm::vec3 _forces, _lowerBounds, _upperBounds;
	float _cellSize;
	glm::ivec3 _gridDimensions; //will need for neighbors / hash grid when i get around to implementing it

	void clearConstraints();
	void setupConstraints();
public:
	Simulation();
	~Simulation();

	void simulationLoop();

	void detectCollisions(const glm::vec3& position, std::vector<Collision>& collisions);
	void resolveCollisions(glm::vec3& position, glm::vec3 velocity, std::vector<Collision>& collisions, glm::vec3& deltaX, int& n);

	float particleHeight(glm::vec3 position);
	void applyForces();
	void predictPositions();
	void applyMassScaling();
//	std::vector<Neighbor> findNeighboringParticles(int index);
	void updateVelocity();
	void advectDiffuse();
	void applyInternalForces();
	void updatePositions();

	void particleBoxCollision(int index);

	void setForces(glm::vec3 forces);
	glm::vec3 getForces();

	glm::vec3 getLowerBounds();
	void setLowerBounds(glm::vec3);

	glm::vec3 getUpperBounds();
    void setUpperBounds(glm::vec3);

	float getCellSize();
    void setCellSize(float size);

	void planeCollision();

	//should be able to add and access particles int he scene
	Particle getParticle(int index);
	void addParticle(Particle particle);
};

