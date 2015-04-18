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

void DensityConstraint::Solve(glm::vec3& position, const float& invMass) {
	
}

//TODO: setup density constraint to find lambda
void DensityConstraint::Solve(std::vector<Particle>& particles) {
    findLambda(_particleIndex, particles);
    glm::vec3 deltaPi = findDeltaPosition(_particleIndex, particles); //constraintDelta*lambda
    Particle& currParticle = particles.at(_particleIndex);
    currParticle.setDeltaPi(deltaPi);
}

void DensityConstraint::findLambda(int index, std::vector<Particle>& particles){
    std::vector<int> neighbors = particles.at(index).getNeighborIndices();
    
    int numNeighbors = static_cast<int>(neighbors.size());
    
    float sumGradientAtParticle = 0.0f;
    float gradientNeighbor = 0.0f;
    
    for (int i=0; i<numNeighbors; i++) {
        gradientNeighbor = glm::length(gradientConstraintForNeighbor(index, i, particles));
        sumGradientAtParticle += gradientNeighbor*gradientNeighbor;
    }
    
    float gradientParticle = glm::length(gradientConstraintAtParticle(index, particles));
    sumGradientAtParticle += gradientParticle*gradientParticle;
    
    float currDensity = this->getDensity(index, particles);
    float densityContraint = (currDensity/restDensity) - 1.0f; //density constraint
    
    float lambdaI = -1.0f * (densityContraint/(sumGradientAtParticle+relaxation)); //findLambda using density constraint
    
    // do these have to be atomic?
    particles.at(index).setDensity(currDensity);
    particles.at(index).setLambda(lambdaI);
}

float DensityConstraint::getDensity(int index, std::vector<Particle>& particles)
{
    float density = 0;
    int i;
    
    std::vector<int> neighbors = particles.at(index).getNeighborIndices();
    
    //Kernel function implementation
    //  Using poly6 kernel function
    
    //TODO
    //  Currently mass is 1 for all particles
    //  If mass is changed, change the for loop to multiply by mass
    for(i=0; i<neighbors.size(); i++)
    {
        density +=  wPoly6Kernel((particles.at(index).getPredictedPosition() - particles.at(neighbors.at(i)).getPredictedPosition()), smoothingRadius);
    }
    
    return density;
}

float DensityConstraint::wPoly6Kernel(glm::vec3 distance, float smoothingRadius)
{
    float x = (smoothingRadius*smoothingRadius) - (glm::length(distance)*glm::length(distance));
    float x_3 = x*x*x;
    
    return poly6Const * x_3/s_9;
}

glm::vec3 DensityConstraint::gradientWSpikyKernel(glm::vec3 distance, float smoothingRadius)
{
    float distanceLength = glm::length(distance);
    
    float x = (smoothingRadius-distanceLength)*(smoothingRadius-distanceLength);
    
    float gradientW = spikyConst * (1.0f/s_6) * x * 1.0f/(distanceLength+EPSILON);
    
    return gradientW*distance;
}

glm::vec3 DensityConstraint::gradientConstraintAtParticle(int index, std::vector<Particle>& particles)
{
    glm::vec3 gradientReturn(0,0,0);
    float restDensityInverse = 1.0/restDensity;
    
    std::vector<int> neighbors = particles.at(index).getNeighborIndices();
    
    for(int i=0; i<neighbors.size(); i++)
    {
        gradientReturn += restDensityInverse *
        gradientWSpikyKernel((particles.at(index).getPosition() - particles.at(neighbors.at(i)).getPosition()), smoothingRadius);
    }
    
    return gradientReturn;
}

glm::vec3 DensityConstraint::gradientConstraintForNeighbor(int index, int neighborIndex, std::vector<Particle>& particles)
{
    glm::vec3 gradientReturn(0,0,0);
    float restDensityInverse = 1.0/restDensity;
    
    
    gradientReturn = restDensityInverse * gradientWSpikyKernel(particles.at(index).getPosition() - particles[neighborIndex].getPosition(), smoothingRadius);
    
    return (-1.0f * gradientReturn);
}

glm::vec3 DensityConstraint::findDeltaPosition(int index, std::vector<Particle>& particles)
{
    glm::vec3 deltaPi(0,0,0);
    
    Particle& currParticle = particles.at(index);
    
    std::vector<int> neighbors = currParticle.getNeighborIndices();
    float lambda_i = currParticle.getLambda();
    float sCorr = 0, k = 0.1, deltaQ = 0.1 * smoothingRadius;
    //    int n = 4; //avoid using pow
    float artificialTerm;
    
    // constraintDelta * lambda
    for(int i=0; i<neighbors.size(); i++)
    {
        float temp = wPoly6Kernel(glm::vec3(deltaQ, 0, 0), smoothingRadius);
        
        artificialTerm = wPoly6Kernel((currParticle.getPredictedPosition() - particles.at(neighbors.at(i)).getPredictedPosition()), smoothingRadius) / (temp+EPSILON);
        
        sCorr = -1.0 * k * artificialTerm * artificialTerm * artificialTerm * artificialTerm;
        
        deltaPi += (particles.at(neighbors[i]).getLambda() + lambda_i + sCorr) *
        gradientWSpikyKernel((currParticle.getPredictedPosition() - particles.at(neighbors.at(i)).getPredictedPosition()), smoothingRadius);
    }
    
    return (deltaPi/restDensity);
}

void DensityConstraint::viscosity(int index, std::vector<Particle>& particles)
{
    Particle& currParticle = particles.at(index);
    std::vector<int> neighbors = currParticle.getNeighborIndices();
    
    glm::vec3 newVelocity = currParticle.getVelocity();
    
    for(int i=0; i<neighbors.size(); i++)
    {
        newVelocity += (1/particles.at(neighbors[i]).getDensity()) * (particles.at(neighbors[i]).getVelocity() - currParticle.getVelocity()) * wPoly6Kernel( (currParticle.getPredictedPosition() - particles.at(neighbors.at(i)).getPredictedPosition()), smoothingRadius);
    }
    
    currParticle.setVelocity(newVelocity);
}