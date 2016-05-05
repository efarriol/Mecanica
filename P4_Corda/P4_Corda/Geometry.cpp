#include "Geometry.h"
#include "ErrorManagement.h"
#include <iostream>
#include <fstream>

using namespace std;

//	4
//	0 0 0 0 45 0 0 1 0.1 0.1 0.1  el primero es el id, los 3 sig son traslacion, el siguiente los angulos, los tres siguientes sobre que eje rota y los tres ultimos el escalado
//	1 0.6 0.6 0 0 0 0 0 0.05 0.05 0.05
//	2 0 - 0.9 0 0 0 0 0 1 0.1 0.1
//	2 0 0.9 0 0 0 0 0 1 0.1 0.1

/*
* Constructor 
*/
Geometry::Geometry(){
	max = 1.0f;
	min = -1.0f;
	mid = 0.0f;
	_numBasicObjects = 0;
	maxX = -100000.f; //very small value
	maxY = -100000.f;
	maxZ = -100000.f;
	minX = 100000.f;  //very big value
	minY = 100000.f;
	minZ = 100000.f;
}
//void Geometry::loadParticles(int numVertex, glm::vec4 color) {
//	_numVertices.push_back(numVertex);
//	_verticesData.push_back(new Vertex[numVertex]);
//	_Numparticles = numVertex;
//	sysParticles.resize(_Numparticles);
//	float increment = 0.0f;
//	for (int j = 0; j < _Numparticles; j++) {
//		sysParticles[j].setPosition(0 - increment, 0 , 0 );
//		sysParticles[j].setVelocity(0.5, 2, 0);
//		sysParticles[j].setLifetime(500);
//		sysParticles[j].setBouncing(0.8f);
//		sysParticles[j].setFixed(false);
//		_verticesData[1][j].setPosition(sysParticles[j].getCurrentPosition().x, sysParticles[j].getCurrentPosition().y, sysParticles[j].getCurrentPosition().z);
//		_verticesData[1][j].setColor(color.x, color.y, color.z, color.w);
//		//posSysPart.resize(_Numparticles);
//	    //posSysPart[j] = sysParticles[j].getCurrentPosition(); //Copy position 
//		increment += 0.3f;
//	}
//}



Geometry::~Geometry(){
	for (int i = 0; i < NUMBASICOBJECTS; i++) {
		delete[] _verticesData[i]; //Preguntar como se crea bien el destructor
	}
}

/*
* Load the game elements from a text file
*/
void Geometry::loadGameElements(char fileName[100]){	
	/* Text format
	<number of game elements>
	<type of game element> <vec3 position> <angle> <vec3 rotation> <vec3 scale>	
	*/  
	GameObject tempObject;
	//LOAD SPHERE
	tempObject._objectType = 0;
	tempObject._translate.x = -0.0;
	tempObject._translate.y = 0;
	tempObject._translate.z = 0;
	tempObject._angle = 90;
	tempObject._rotation.x = 1;
	tempObject._rotation.y = 0;
	tempObject._rotation.z = 0;
	tempObject._scale.x = 1;
	tempObject._scale.y = 1;
	tempObject._scale.z = 1;
	_listOfObjects.push_back(tempObject);

	for (int i = 0; i < NUMPARTICLES; i++) {
		tempObject._objectType = 1;
		tempObject._translate.x = 0;
		tempObject._translate.y = 0;
		tempObject._translate.z = 0;
		tempObject._angle = 0;
		tempObject._rotation.x = 0;
		tempObject._rotation.y = 0;
		tempObject._rotation.z = 0;
		tempObject._scale.x = 0.1f;
		tempObject._scale.y = 0.1f;
		tempObject._scale.z = 0.1f;
		_listOfParticles[i]= tempObject;
	}


}

/*
* Get the vertices data for an specific object
* @param objectID is the identifier of the requested object
* @return Vertex * is an array with all the vertices data
*/
Vertex * Geometry::getData(int objectID){
	return _verticesData[objectID];
}


/*
* Get the number of vertices for an specific object
* @param objectID is the identifier of the requested object
* @return int is the number of vertices
*/

int Geometry::getNumVertices(int objectID){
	return _numVertices[objectID];
}

/*
* Get the number of elements to render
*/
int Geometry::getNumGameElements() {
	return _listOfObjects.size();
}

/*
* Get the number of vertices of an specific game element
* @param objectID is the identifier of the requested object
*/
GameObject & Geometry::getGameElement(int objectID) {
	return (_listOfObjects[objectID]);
}
GameObject & Geometry::getGameParticle(int file) {
	return (_listOfParticles[file]);
}

void Geometry::loadBasic3DObjects()
{
	for (int i = 0; i < NUMBASICOBJECTS; i++) {
		switch (i) {
		case 0:
			_objectLoader.loadAse("./resources/models/sphere.ASE", _numVertices, _verticesData);

			for (int j = 0; j < _numVertices[0]; j++)_verticesData[i][j].setColor(255, 255, 0, 255);
			for (int j = 0; j < _numVertices[0]; j++) {
				if (_verticesData[0][j].position.x > maxX) maxX = _verticesData[0][j].position.x;
				if (_verticesData[0][j].position.y > maxY) maxY = _verticesData[0][j].position.y;
				if (_verticesData[0][j].position.z > maxZ) maxZ = _verticesData[0][j].position.z;
				if (_verticesData[0][j].position.x < minX) minX = _verticesData[0][j].position.x;
				if (_verticesData[0][j].position.y < minY) minY = _verticesData[0][j].position.y;
				if (_verticesData[0][j].position.z < minZ) minZ = _verticesData[0][j].position.z;
			}
			break;
		case 1:
			_numVertices.push_back(1);
			_verticesData.push_back(new Vertex[1]);
			for (int j = 0; j < _numVertices[i]; j++) {
				_verticesData[i][j].setColor(255, 0, 0, 255);
			}
			for (int j = 0; j < _numVertices[i]; j++) {
				_verticesData[i][j].setPosition(0, 0, 0);
			}
			break;
		}
	}
}
