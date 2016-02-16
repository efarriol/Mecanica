#pragma once;
#include <iostream>  //per escriure a consola
#include <string>    //per poder passar el nom al cout
#include <fstream>   //per escriure a fitxer
#include <chrono>    //per comptar el temps d'execucio 
#include <vector>    //per crear vectors d'estructures tipus: Point, Lines, etc.

#include "Geometry.h"
#define PRINT_VEC3(_v) std::cout << _v.x << " " << _v.y << " " << _v.z << std::endl;
#define FPRINT_VEC3(_v) fileOut << _v.x << " " << _v.y << " " << _v.z << "\n";


//for lazy people....
#define tempsActual std::chrono::high_resolution_clock::now()  

void main()
{
	//Fitxer de resultats
	std::string filename = "results.txt";

	std::ofstream fileOut(filename, std::ios::out);
	if (!fileOut)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}

	//Geometry
	Point p, q, r;
	std::chrono::high_resolution_clock::time_point tini, tfin;
	tini = tempsActual; //the same as  std::chrono::high_resolution_clock::now();

	/// Test Point methods
	float alfa = 0.5;
	float x = 1.0f; float y = 0.5f; float z = 1.5f;
	glm::vec3 pos(0, 0.1,-2);
	p.setPosition(pos);
	q.setPosition(x, y, z);
	//print values
	std::cout << "Position point p" << std::endl;
	//	std::cout << p.position.x << "  " << p.position.y << "  " << p.position.z << std::endl;
	PRINT_VEC3(p.position);
	FPRINT_VEC3(p.position); //write into a file
	std::cout << "Position point q" << std::endl;
//	std::cout << q.position.x << "  " << q.position.y << "  " << q.position.z << std::endl;
	PRINT_VEC3(q.position);
	FPRINT_VEC3(q.position);
	//compute distance (length of the vector q-p)
	float dist = glm::length(p.position - q.position);
	// a consola
	std::cout << "distancia de p a q = " << dist << std::endl;
	std::cout << "distancia entre els punts p i q = " << p.distPoint2Point(q) << std::endl;
	// a fitxer
	fileOut << "distancia de p a q = " << dist << "\n";
	fileOut << "distancia entre els punts p i q = " << p.distPoint2Point(q) << "\n";


	//move the point q until it is far from p (>5 units)
	std::cout << std::endl;
	std::cout << "separem els punts  " << std::endl;
	for (int i = 0; i < 11; i++){
		q.setPosition(x+i, y, z);
		std::cout << " iter = " << i << " present point = " << q.position.x << "  " << q.position.y << "  " << q.position.z << std::endl;
		std::cout << "dist to p =" << p.distPoint2Point(q.position) << std::endl;
		if (p.distPoint2Point(q.position) > 5){
			std::cout << "hem passat de 5 " << std::endl;
			break;
		}
	}


	
	r = p.pointInSegment(q, alfa); //punt mig del segment pq, ja que alfa =0.5
	std::cout << std::endl;
	std::cout << "Segment pq point at value alfa = " << alfa << std::endl;
	std::cout << r.position.x << "  " << r.position.y << "  " << r.position.z << std::endl;
	std::cout << "verify dist to mid point: " << p.distPoint2Point(r) << std::endl; 
	std::cout << "verify mid distance between the points: " << 0.5f*p.distPoint2Point(q) << std::endl;

	std::cout << "\n------------------------------------Line-------------------------------------\n" << std::endl;
	
	/// Test Line methods
	Line line;
	line.point = glm::vec3(0); //origen of coordinates
	line.setDirection(glm::vec3(1, 0, 0)); //inicialitzacio com a vec3
	q.setPosition(0.0f, 2.0f, 0.0f); //inicialitzacio com a floats
	std::cout << "point q = ";
	std::cout << q.position.x << "  " << q.position.y << "  " << q.position.z << std::endl;

	dist = line.distLine2Point(q);
	std::cout << "dist from q to line = " << dist << std::endl;
	r = line.closestPointInLine(q);
	std::cout << "closest point to q in the line = " << std::endl;
	std::cout << r.position.x << "  " << r.position.y << "  " << r.position.z << std::endl;
	std::cout << "check point r is the line (bool) = " << line.isInside(r) << std::endl;
	std::cout << "check again point r is the line (dist) =" << line.distLine2Point(r) << std::endl;


	Line line2;
	line2.point = q;
	line2.setDirection(glm::vec3(0, 0, 1));	

	tfin = tempsActual; //std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	std::cout << "milliseconds = " << duration << std::endl;

	std::cout << "dist between lines = " << line.distLine2Line(line2) << std::endl;
	
	std::cout << "\n------------------------------------Plane-------------------------------------\n" << std::endl;
	/// Test Plane methods
	Plane pla(glm::vec3(0), glm::vec3(1, 1, 1));
	pla.setPosition(glm::vec3(2, 2, 2));
	std::cout << "plane equation " << std::endl;
	std::cout << pla.normal.x << " .x + " << pla.normal.y << ".y +" << pla.normal.z << ".z + " << pla.dconst << "  = 0 " << std::endl;
	std::cout << "verifiquem que esta sobre el pla, bool= : " << pla.isInside(glm::vec3(2.0f)) << std::endl;

	q.setPosition(glm::vec3(0, 5, 6));
	dist = pla.distPoint2Plane(q.position);
	std::cout << "distancia des de q: " << dist << std::endl;
	r.setPosition(pla.closestPointInPlane(q.position)); //point in the plane
	std::cout << "r = " << r.position.x << "  " << r.position.y << "  " << r.position.z << std::endl;
	dist = pla.distPoint2Plane(r.position);
	std::cout << "verifiquem distancia: " << dist << std::endl;
	p.setPosition(-5, -6, 0);
	dist = pla.distPoint2Plane(p.position);
	std::cout << "distancia des de p: " << dist << std::endl;
	std::cout << "r = "<< r.position.x << "  " << r.position.y << "  " << r.position.z << std::endl;
	bool tall = pla.intersecSegment(p.position, q.position, r.position);
	std::cout << "verifiquem que esta sobre el pla, bool= : " << tall << std::endl;
	std::cout << "r = " << r.position.x << "  " << r.position.y << "  " << r.position.z << std::endl;
	dist = pla.distPoint2Plane(r.position);
	std::cout << "distancia des de r: " << dist << std::endl;
	
	system("pause");

	
};