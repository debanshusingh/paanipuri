#include "Simulation.h"
#include <cmath>

Simulation::Simulation() {
	restitutionCoefficient = 0.1f;
	deltaT = 0.0333; //timestep
	kCoefficient = 1; //not sure what it is. paper doesn't explain it, just says it is usually between 1 and 5
	stabilizationIterations = 10;
}

Simulation::~Simulation() {

}

void Simulation::setForces(glm::vec3 forces) {
	_forces = forces;
}

glm::vec3 Simulation::getForces() {
	return _forces;
}

glm::vec3 Simulation::getLowerBounds() {
    return _lowerBounds;
}

void Simulation::setLowerBounds(glm::vec3 lowerBounds) {
    _lowerBounds = lowerBounds;
}

glm::vec3 Simulation::getUpperBounds() {
    return _upperBounds;
}

void Simulation::setUpperBounds(glm::vec3 upperBounds) {
    _upperBounds = upperBounds;
}

float Simulation::getCellSize() {
    return _cellSize;
}

void Simulation::setCellSize(float size) {
    _cellSize = size;
    _gridDimensions = (_upperBounds - _lowerBounds) / _cellSize;
}

void Simulation::applyForces() {
	for(int i = 0; i < particles.size(); i++) {
		Particle&currentParticle = particles[i];
		currentParticle.setVelocity(currentParticle.getVelocity() + deltaT * _forces);
	}
}

void Simulation::predictPositions() {
	for(int i = 0; i < particles.size(); i++) {
		glm::vec3 predictedPosition = particles[i].getPosition() + deltaT * particles[i].getVelocity();
		particles[i].setPredictedPosition(predictedPosition);
	}
}

void Simulation::applyMassScaling() {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].setPredictedMass(particles[i].getMass() * exp(-kCoefficient * particleHeight(particles[i].getPredictedPosition())));
	}
}

//std::vector<Neighbor> Simulation::findNeighboringParticles(int index) {
//	std::vector<Neighbor> test;
//	return test;
//}

void Simulation::updateVelocity() {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].setVelocity((1 / deltaT) * (particles[i].getPredictedPosition() - particles[i].getPosition()));
	}
}

void Simulation::advectDiffuse() {
	//not sure what this is exactly
	//i believe this is related to fluid and smoke though, so ignore it for now
}

void Simulation::applyInternalForces() {
	//i think this only applies to fluids, so not doing it as of now.
	//since this is applying drag and vort, but i could be wrong.
}

void Simulation::updatePositions() {
	//either updates positions or applies the sleeping if the velocity is drifting bellow the user defined threshold
	for(int i = 0; i < particles.size(); i++) {
		if(glm::length(particles[i].getPredictedPosition() - particles[i].getPosition()) > sleepThreshold) {
			particles[i].setPosition(particles[i].getPredictedPosition());
		}
		
		//else we just keep the original position.. and htus do nothing?
		
	}
}

void Simulation::simulationLoop() {
	//apply the forces first
	applyForces();

	//predict positions
	predictPositions();

	//apply mass scaling
	applyMassScaling();

	//for each particle, find the neighbors and then solve the solid contacts
	for(int i = 0; i < particles.size(); i++) {
		//TODO: find neighboring particles
		//findNeighboringParticles(i);
		//and with this info solve hte solid contacts..
		//(just naively solve solid contacts now)
//		detectCollisions(particles[i].getPredictedPosition(), particles[i].getCollisions());

		//TODO
		//TEMP
		particleBoxCollision(i);
	}

	//stability iterations
	for (int i = 0; i < stabilizationIterations; i++) {
		//loop for all particles, the innner loop will loop for all collisions on that particle, if it's zero, that loop will not run
		for (int j = 0; j < particles.size(); j++) {
			glm::vec3 deltaX = glm::vec3(0.0, 0.0, 0.0);
			int n = 0; //number of constraints effecting this particle
			//the position and velocity s houldn't be changed inside the function resolveCollisions
			//also, what counts as a constraint here for our counting? # of collisions?
//			if(particles[j].getCollisions().size() > 0) {
//				resolveCollisions(particles[j].getPosition(), particles[j].getVelocity(), particles[j].getCollisions(), deltaX, n);
//				//now update the particle with your calculations
//				particles[j].setPosition(particles[j].getPosition() + (deltaX / (float) n)); //shouldn't change anything to convert to float right?
//				particles[j].setPredictedPosition(particles[j].getPredictedPosition() + (deltaX / (float) n));
//			}
			
		}
	}

	//solver iterations
	for (int i = 0; i < solverIterations; i++) {
		for (int j = 0; j < numberOfConstraintGroups; j++) { //constraint groups have to be recorded then
			//so within here, i would do another loop for the collisions?
			for (int k = 0; k < particles.size(); k++) {

				//TODO: not sure if phase is being used correctly here
				//It isn't, phase should be used here but not this way. This should just be number of constraint groups. Phases are different
				if(particles[i].getPhase() == (numberOfConstraintGroups - 1)) {
					//similar to the above loop for stability iterations, but will use a different constraint solver
					//also going to focus on the constraint groups (the groups that form objects)
					glm::vec3 deltaX = glm::vec3(0.0, 0.0, 0.0);
					int n = 0; //number of constraints effecting this particle

					//call constraint solver
					//still waiting on sibi for this

					//update predicted position after each constraint solve?
					particles[j].setPredictedPosition(particles[j].getPredictedPosition() + (deltaX / (float) n));
				}
			}
			
		}
	}

	//update velocity
	updateVelocity();
	//advectDiffuse();
	//applyInternalForces();
	updatePositions();
}

float Simulation::particleHeight(glm::vec3 position) {
	//given the position of a particle, return the height of hte particle from the ground plane

	//TODO this is temp!
	return position.y;
}

//call first
//eventually work neighbors into this so we don't have to search the whole space
//currently only detects collisions between particles (not between box or planes)
void Simulation::detectCollisions(const glm::vec3& position, std::vector<Collision>& collisions) {
	glm::vec3 normal;
	double distance;
	collisions.clear(); //this is the collisions on the particle iteself so this is cool to clear each time


	for (int i = 0; i < particles.size(); i++) {
		for (int j = i + 1; j < particles.size(); j++) {
			//so we are checking all the particles against each other?

//			if (particles[i].IntersectionTest(particles[j].getPosition(), normal, distance)) {
//				//so a collision happened
//				collisions.push_back(Collision(i, normal, distance));
//			}
		}
	}
}

//call after detect collisions
void Simulation::resolveCollisions(glm::vec3& position, glm::vec3 velocity, std::vector<Collision>& collisions, glm::vec3& deltaX, int& n) {
	glm::vec3 p = position, v = velocity; //save them so we are not modifying the original values
	
	for (std::vector<Collision>::iterator iterator = collisions.begin(); iterator != collisions.end(); ++iterator) {
		int id = iterator->_id;
		p -= iterator->_normal * (float) iterator->_distance; //this should correct the position
		//correct the velocity
		float newVelocity = glm::dot(iterator->_normal, v);
		v += -(1 + restitutionCoefficient) * newVelocity * iterator->_normal;
		
		//increment n
		n++;
	}

	//calculate the deltaX and n (which i am still confused about, just doing size of collisions now)
	deltaX = p - position;
}

//TODO
void Simulation::planeCollision() {
	
}

Particle Simulation::getParticle(int index) {
	return particles[index];
}

void Simulation::addParticle(Particle particle) {
	particles.push_back(particle);
}

//switch to plane collision!
//this is temp
void Simulation::particleBoxCollision(int index) {
    Particle& currParticle = particles[index];
	glm::vec3 particlePosition = currParticle.getPredictedPosition();

    float radius = currParticle.getRadius();
    float dampingFactor = 0.5f;
    
    if(particlePosition.x - radius < _lowerBounds.x + 1e-3 || particlePosition.x + radius > _upperBounds.x - 1e-3) {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(-dampingFactor, 1, 1));
        currParticle.setPredictedPosition(currParticle.getPosition() + deltaT * currParticle.getVelocity());
    }

    if(particlePosition.y - radius < _lowerBounds.y + 1e-3) {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1, -dampingFactor, 1));
        currParticle.setPredictedPosition(currParticle.getPosition() + deltaT * currParticle.getVelocity());
    }

    if(particlePosition.y + radius > _upperBounds.y - 1e-3) {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1, -dampingFactor, 1));
        glm::vec3 pos = currParticle.getPredictedPosition();
        currParticle.setPredictedPosition(glm::vec3(pos.x, _upperBounds.y - radius - 1e-3, pos.z));
    }

    if(particlePosition.z - radius < _lowerBounds.z + 1e-3 || particlePosition.z + radius > _upperBounds.z - 1e-3) {
        currParticle.setVelocity(currParticle.getVelocity() * glm::vec3(1, 1, -dampingFactor));
        currParticle.setPredictedPosition(currParticle.getPosition() + deltaT * currParticle.getVelocity());
    }
}

void Simulation::clearConstraints() {
	//err, constraint groups?
	_rigidBodyConstraints.clear();
	_fluidConstraints.clear();
}

//maybe take in something to determine what's what?
void Simulation::setupConstraints() {
	clearConstraints();

	//so I would have to know what each particle is apart of (phase?) and then set up accordingly?
	//this seems rough, I won't know what's what when I am getting it from maya.
	
	//so for now, assume that rigid bodies have phase one, and fluids phase 2
	//will fluids also have contact constraints...?
	for(int i = 0; i < particles.size(); i++) {
		switch(particles[i].getPhase()) {
			case 0: {
				//rigid body
					ContactConstraint *constraint = new ContactConstraint(i);
					_rigidBodyConstraints.push_back(constraint);
				}
				break;
			case 1: {
				//fluid
					DensityConstraint *constraint = new DensityConstraint(i);
					_fluidConstraints.push_back(constraint);
				}
				break;
		}
	}
}