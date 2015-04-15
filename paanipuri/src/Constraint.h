#pragma once
#include "glm/glm.hpp"

class Constraint {
protected:
	
public:
	//TODO: What do I need to init the constraint? anything that is universal?
	Constraint();
	virtual ~Constraint();

	//actually supposed to be the inverse mass matrix..
	virtual void Solve(glm::vec3& position, const float& invMass);
};

class ContactConstraint : public Constraint {
protected:
	int _particleIndex;

public:
	ContactConstraint();
	ContactConstraint(int particleIndex);
	virtual ~ContactConstraint();

	//actually supposed to be the inverse mass matrix..
	virtual void Solve(glm::vec3& position, const float& invMass);
	int getParticleIndex();
};

//shape matching constraint also needs stiffness, which by consequence gives it deformables (Which we do not need) but we might need it just in general
class ShapeMatchingConstraint : public Constraint {
public:
	ShapeMatchingConstraint();
	virtual ~ShapeMatchingConstraint();

	//actually supposed to be the inverse mass matrix..
	virtual void Solve(glm::vec3& position, const float& invMass);
};

class DensityConstraint : public Constraint {
protected:
	int _particleIndex;

public:
	DensityConstraint();
	DensityConstraint(int particleIndex);
	virtual ~DensityConstraint();

	//actually supposed to be the inverse mass matrix..
	virtual void Solve(glm::vec3& position, const float& invMass);
	int getParticleIndex();
};