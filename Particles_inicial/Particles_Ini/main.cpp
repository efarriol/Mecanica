#include <iostream>
#include "Particle.h"
#include "GeometryPlane.h"


void main(){
	float dt = 0.01f;  //simulation time
	float tini = 0.0f; 
	float tfinal = 6.0f; //final time of simulation 
	// particle inicialization
	Particle p(0.0f, 5.0f, 0.0f); //initial position of the particle

	p.setLifetime(5.0f);
	std::cout << "Lifetime =" << p.getLifetime() << std::endl;
	p.setBouncing(1.0f);
	p.addForce(0, -9.8f, 0);
	//p.setFixed(true);

	// define a floor plane for collision
	glm::vec3 punt(0.0f);
	glm::vec3 normal(0, 1, 0);
	Plane plane(punt, normal);
	// simulation loop
	int count = 0; //per el num de rebots
	float disact, disant;
	disact = plane.distPoint2Plane(p.getCurrentPosition());
	float time = tini;
	while (time <= tfinal) {
		if (p.getLifetime() > 0) {
			p.updateParticle(dt, Particle::UpdateMethod::EulerSemi);
			std::cout << "posicio = " << p.getCurrentPosition().x << "  " << p.getCurrentPosition().y
				<< "  " << p.getCurrentPosition().z << "  temps = " << time << std::endl;
			//Check for floor collisions
			disant = disact;
			p.setLifetime(p.getLifetime() - dt);
			disact = plane.distPoint2Plane(p.getCurrentPosition());
			if (disant*disact < 0.0f) { 
				//only valid for the plane y=0 (floor plane)
				//p.setPosition(p.getCurrentPosition().x, -p.getCurrentPosition().y, p.getCurrentPosition().z); //Simplement li "invrteix" la posicio perque nomes es mou en y
				//p.setVelocity(p.getVelocity().x, -p.getBouncing()*p.getVelocity().y, p.getVelocity().z);

				p.setPosition(p.getCurrentPosition() - (1.0f + p.getBouncing())*disact*normal);
				p.setVelocity(p.getVelocity() - (1.0f + p.getBouncing())*(glm::dot(p.getVelocity(), plane.normal)*normal));

				std::cout << "rebot = " << count++ << std::endl;
				disact = -disact; //
				system("PAUSE");
			}
		}
		time = time + dt; //increase time counter
	}
	system("PAUSE");
}