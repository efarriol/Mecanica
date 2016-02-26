//----------------------------------------------------------------------------------------------------------
// Pràctica realitzada per Pol Casamiquela i Eloi Farriol 2n A Grau en Continguts Digitals Interactius ENTI 
//----------------------------------------------------------------------------------------------------------

#pragma once;
#include <iostream>  //per escriure a consola
#include <string>    //per poder passar el nom al cout
#include <fstream>   //per escriure a fitxer
#include <chrono>    //per comptar el temps d'execucio 
#include <vector>    //per crear vectors d'estructures tipus: Point, Lines, etc.

#include "Geometry.h"
#define PRINT_VEC3(_v) fileOut << _v.x << " " << _v.y << " " << _v.z << std::endl;
#define FPRINT_VEC3(_v) fileOut << _v.x << " " << _v.y << " " << _v.z << std::endl;


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
	std::chrono::high_resolution_clock::time_point tini, tfin, tiniGlobal, tfinGlobal;
	tiniGlobal = tempsActual;

	//General variables
	const float angleBeta = 30.0f;
	const float angleDelta = 45.0f;
	const int originRadius = 1;
	const float maxAlpha = 10.0f;
	const float increase = 0.01f;
	glm::vec3 director;
	director.x = glm::cos(glm::radians(angleBeta)) * glm::cos(glm::radians(angleDelta)) * originRadius;
	director.y = glm::sin(glm::radians(angleDelta)) * originRadius;
	director.z = glm::sin(glm::radians(angleBeta)) * glm::cos(glm::radians(angleDelta)) * originRadius;

	// Gestionar un vector d'estructures (per exemple tipus Point)
	const float nPunts = maxAlpha / increase;
	fileOut << " " << std::endl;
	fileOut << "Numero de Punts = " << nPunts << std::endl;

	// Primer metode: vector dimensionat i assignem valors
	std::vector<Point> punts(nPunts);
	fileOut << "Longitud del vector punts = " << punts.size() << std::endl;
	//Comptar el temps d'execució
	 //std::chrono::high_resolution_clock::now();
	static int counter = 0;
	for (float i = 0.0f; i < maxAlpha; i += increase) {
		punts[counter] = glm::vec3(0) + i * director;
		counter++;
	}
	fileOut << "\n------------------------------------Sphere-------------------------------------\n" << std::endl;

	tini = tempsActual;
	
	Sphere sphere(glm::vec3(1, 2, 1), 3.0f);

	for (counter = 1; counter < int(nPunts); counter++) {
		if (sphere.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			fileOut << "\nPunt interseccio exacte: " << std::endl;
			fileOut << "	x: " << sphere.puntTall.x << std::endl;
			fileOut << "	y: " << sphere.puntTall.y << std::endl;
			fileOut << "	z: " << sphere.puntTall.z << std::endl;;
		}
		if (sphere.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en la esfera: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;;
		}
	}

	tfin = tempsActual;
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;


	fileOut << "\n------------------------------------Plane-------------------------------------\n" << std::endl;

	tini = tempsActual;
	Plane plane(glm::vec3(1, 2, 1), glm::vec3(-1, 1, 1));

	for (counter = 1; counter < int(nPunts); counter++) {
		if (plane.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			fileOut << "\nPunt interseccio entre: " << std::endl;
			fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
			fileOut << "\nPunt interseccio exacte: " << std::endl;
			fileOut << "	x: " << plane.puntTall.x << std::endl;
			fileOut << "	y: " << plane.puntTall.y << std::endl;
			fileOut << "	z: " << plane.puntTall.z << std::endl;
		}
		if (plane.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en el pla: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}

	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;

	fileOut << "\n------------------------------------Triangle------------------------------------\n" << std::endl;

	tini = tempsActual;
	Triangle triangle(glm::vec3(0, 0, 0), glm::vec3(1, 5, 1), glm::vec3(-1, 3, 2));

	for (counter = 1; counter < (int)nPunts; counter++) {
		if (triangle.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			fileOut << "\nPunt interseccio entre: " << std::endl;
			fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
			fileOut << "\nPunt interseccio exacte: " << std::endl;
			fileOut << "	x: " << triangle.puntTall.x << std::endl;
			fileOut << "	y: " << triangle.puntTall.y << std::endl;
			fileOut << "	z: " << triangle.puntTall.z << std::endl;
		}
		if (triangle.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en el triangle: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}
	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;
	
	fileOut << "\n------------------------------------Box------------------------------------\n" << std::endl;

	tini = tempsActual;

	Box box(glm::vec3(0, 0, 0), glm::vec3(5, 0, 0), glm::vec3(5, 5, 0), glm::vec3(0, 5, 0), glm::vec3(0, 0, 5));
	for (counter = 1; counter < (int)nPunts; counter++) {
		if (box.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			if (box.faceCut == 1)
			{
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla1.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla1.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla1.puntTall.z << std::endl;
			}
			else if (box.faceCut == 2) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla2.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla2.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla2.puntTall.z << std::endl;

			}
			else if (box.faceCut == 3) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla3.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla3.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla3.puntTall.z << std::endl;

			}
			else if (box.faceCut == 4) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla4.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla4.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla4.puntTall.z << std::endl;

			}
			else if (box.faceCut == 5) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla5.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla5.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla5.puntTall.z << std::endl;

			}
			else if (box.faceCut == 6) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
				fileOut << "	x: " << box.pla6.puntTall.x << std::endl;
				fileOut << "	y: " << box.pla6.puntTall.y << std::endl;
				fileOut << "	z: " << box.pla6.puntTall.z << std::endl;
			}
		}
		if (box.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en la capsa: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}

	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;

	fileOut << "\n------------------------------------Cylinder------------------------------------\n" << std::endl;

	tini = tempsActual;

	Cylinder cylinder(5.0f, glm::vec3(0, 8, 0), glm::vec3(0, 0, 0));

	for (counter = 1; counter < (int)nPunts; counter++) {
		if (cylinder.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			fileOut << "\nPunt interseccio entre: " << std::endl;
			fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
			fileOut << "\nPunt interseccio exacte: " << std::endl;
			fileOut << "	x: " << cylinder.puntTall.x << std::endl;
			fileOut << "	y: " << cylinder.puntTall.y << std::endl;
			fileOut << "	z: " << cylinder.puntTall.z << std::endl;
		}
		if (cylinder.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en el cilindre: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}

	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;

	fileOut << "\n------------------------------------Capsule------------------------------------\n" << std::endl;

	tini = tempsActual;

	Capsule capsule(5.0f, glm::vec3(0, 0, 0), glm::vec3(0, -8, 0));

	for (counter = 1; counter < (int)nPunts; counter++) {
		if (capsule.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			fileOut << "\nPunt interseccio entre: " << std::endl;
			fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
			fileOut << "\nPunt interseccio exacte: " << std::endl;
			fileOut << "	x: " << capsule.puntTall.x << std::endl;
			fileOut << "	y: " << capsule.puntTall.y << std::endl;
			fileOut << "	z: " << capsule.puntTall.z << std::endl;
		}
		if (capsule.isInside(punts[counter].position)) {
			fileOut << "\nPunt contingut en la capsula: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}

	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;


	fileOut << "\n------------------------------------Tetrahedron------------------------------------\n" << std::endl;
	// En aquest apartat, quan arribem a cert nombre d'iteracions, la funcio que calcula els punts interns al tetraedre, deixa de calcular 
	// punts que haurien de continuar estant dins del mateix. Hem intetat arreglar-ho pero no hem pogut trobar la solucio
	tini = tempsActual;

	Tetrahedron tetrahedron(glm::vec3(0, 0, -5), glm::vec3(0, 0, 5), glm::vec3(5, 0, 0), glm::vec3(0, 5, 0));
	for (counter = 1; counter < (int)nPunts; counter++) {
		if (tetrahedron.intersecSegment(punts[counter - 1].position, punts[counter].position, glm::vec3(NULL))) {
			if (tetrahedron.faceCut == 1)
			{
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara del tetraedre: " << tetrahedron.faceCut << std::endl;
				fileOut << "	x: " << tetrahedron.face1.puntTall.x << std::endl;
				fileOut << "	y: " << tetrahedron.face1.puntTall.y << std::endl;
				fileOut << "	z: " << tetrahedron.face1.puntTall.z << std::endl;
			}
			else if (tetrahedron.faceCut == 2) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara del tetraedre: " << tetrahedron.faceCut << std::endl;
				fileOut << "	x: " << tetrahedron.face2.puntTall.x << std::endl;
				fileOut << "	y: " << tetrahedron.face2.puntTall.y << std::endl;
				fileOut << "	z: " << tetrahedron.face2.puntTall.z << std::endl;

			}
			else if (tetrahedron.faceCut == 3) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara del tetraedre: " << tetrahedron.faceCut << std::endl;
				fileOut << "	x: " << tetrahedron.face3.puntTall.x << std::endl;
				fileOut << "	y: " << tetrahedron.face3.puntTall.y << std::endl;
				fileOut << "	z: " << tetrahedron.face3.puntTall.z << std::endl;

			}
			else if (tetrahedron.faceCut == 4) {
				fileOut << "\nPunt interseccio entre: " << std::endl;
				fileOut << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
				fileOut << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
				fileOut << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
				fileOut << "\nPunt interseccio exacte en la cara del tetraedre: " << tetrahedron.faceCut << std::endl;
				fileOut << "	x: " << tetrahedron.face4.puntTall.x << std::endl;
				fileOut << "	y: " << tetrahedron.face4.puntTall.y << std::endl;
				fileOut << "	z: " << tetrahedron.face4.puntTall.z << std::endl;

			}
		
		}
		if (tetrahedron.isInside(punts[counter].position)) { 
			fileOut << "\nPunt contingut en la tetraedre: " << std::endl;
			fileOut << "	x: " << punts[counter].position.x << std::endl;
			fileOut << "	y: " << punts[counter].position.y << std::endl;
			fileOut << "	z: " << punts[counter].position.z << std::endl;
		}
		
	}

	tfin = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	fileOut << "\nTemps d'execucio: " << duration << " milisegons" << std::endl;

	fileOut.close();

	tfinGlobal = tempsActual;
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfinGlobal - tiniGlobal).count();

	std::cout << "Practica realitzada per Pol Casamiquela i Eloi Farriol\n\nTots els resultats de la practica (incluint els del cilindre, la capsula, la capsa i el tetraedre) estan al document results.txt\nEn el document escribim totes les x, y, z de tots els punts d'interseccio i els isInsides, per el que queda un document molt extens. Si consideres que el format no es comode, podem modificar-lo i tornar-lo a enviar\n\n";
	std::cout << "Duracio total de l'execucio: " << duration << " milisegons\n" << std::endl;
	system("pause");
};
