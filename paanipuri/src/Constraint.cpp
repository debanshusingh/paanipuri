#include "Constraint.h"

Constraint::Constraint() {

}


Constraint::~Constraint() {

}

void Constraint::Solve(glm::vec3& position, const float& invMass) {
    //so check if there is a contact on the particle. if so, resolve it
    
}

ContactConstraint::ContactConstraint() {

}

ContactConstraint::ContactConstraint(int particleIndex) {
	_particleIndex = particleIndex;
}

ContactConstraint::~ContactConstraint() {
	
}

//TODO: Implement
void ContactConstraint::Solve(glm::vec3& position, const float& invMass) {
	//so check if there is a contact on the particle. if so, resolve it

}

DensityConstraint::DensityConstraint() {

}

DensityConstraint::DensityConstraint(int particleIndex) {
	_particleIndex = particleIndex;
}

DensityConstraint::~DensityConstraint() {
	
}

//TODO: Implement
void DensityConstraint::Solve(glm::vec3& position, const float& invMass) {
	
}