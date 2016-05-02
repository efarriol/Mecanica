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
void Geometry::loadCube(int i, glm::vec4 color)
{
		_numVertices.push_back(36);
		_verticesData.push_back(new Vertex[36]);

		_verticesData[i][0].setPosition(min, min, min);
		_verticesData[i][1].setPosition(max, min, min);
		_verticesData[i][2].setPosition(min, max, min);

		_verticesData[i][3].setPosition(max, min, min);
		_verticesData[i][4].setPosition(max, max, min);
		_verticesData[i][5].setPosition(min, max, min);

		_verticesData[i][6].setPosition(max, min, min);
		_verticesData[i][7].setPosition(max, min, max);
		_verticesData[i][8].setPosition(max, max, min);

		_verticesData[i][9].setPosition(max, min, max);
		_verticesData[i][10].setPosition(max, max, max);
		_verticesData[i][11].setPosition(max, max, min);

		_verticesData[i][12].setPosition(max, min, max);
		_verticesData[i][13].setPosition(min, min, max);
		_verticesData[i][14].setPosition(max, max, max);

		_verticesData[i][15].setPosition(min, min, max);
		_verticesData[i][16].setPosition(min, max, max);
		_verticesData[i][17].setPosition(max, max, max);

		_verticesData[i][18].setPosition(min, min, max);
		_verticesData[i][19].setPosition(min, min, min);
		_verticesData[i][20].setPosition(min, max, max);

		_verticesData[i][21].setPosition(min, min, min);
		_verticesData[i][22].setPosition(min, max, min);
		_verticesData[i][23].setPosition(min, max, max);

		_verticesData[i][24].setPosition(min, max, min);
		_verticesData[i][25].setPosition(max, max, min);
		_verticesData[i][26].setPosition(min, max, max);

		_verticesData[i][27].setPosition(max, max, min);
		_verticesData[i][28].setPosition(max, max, max);
		_verticesData[i][29].setPosition(min, max, max);

		_verticesData[i][30].setPosition(min, min, min);
		_verticesData[i][31].setPosition(min, min, max);
		_verticesData[i][32].setPosition(max, min, max);

		_verticesData[i][33].setPosition(min, min, min);
		_verticesData[i][34].setPosition(max, min, max);
		_verticesData[i][35].setPosition(max, min, min);

		for (int j = 0; j < 36; j++) _verticesData[i][j].setColor(color.x, color.y, color.z, color.w);
}


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
	ifstream file;
	file.open(fileName);
	
	if (file.is_open()){
		//TODO: Read the content and add it into the data structure
		file >> _numBasicObjects;
		for (int i = 0; i < _numBasicObjects; i++) {
			file >> tempObject._objectType >>  tempObject._translate.x >> tempObject._translate.y >> tempObject._translate.z >> tempObject._angle
				>> tempObject._rotation.x >> tempObject._rotation.y >> tempObject._rotation.z >> tempObject._scale.x >> tempObject._scale.y >> tempObject._scale.z;
			_listOfObjects.push_back(tempObject);
		}
		file.close();
	}
	else{
		string message = "The file "+ string(fileName)+" doesn't exists";
		ErrorManagement::errorRunTime(message);
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


void Geometry::loadBasic3DObjects()
{
	for (int i = 0; i < NUMBASICOBJECTS; i++) {
		switch (i) {
		case 0:
			_objectLoader.loadAse("./resources/models/sphere.ASE", _numVertices, _verticesData);
			for (int j = 0; j < _numVertices[0]; j++) _verticesData[0][j].setColor(255, 255, 0, 255);
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
			_Numparticles = 5;
			sysParticles.resize(_Numparticles);
			for (int j = 0; j < _Numparticles; j++) {
				sysParticles[j].setPosition(0.1f + j/10, 0.3f, 0.0f);
				sysParticles[j].setVelocity(0.5, 2, 0);
				sysParticles[j].setLifetime(500);
				sysParticles[j].setBouncing(0.8f);
				sysParticles[j].setFixed(false);

				posSysPart.resize(_Numparticles);
				posSysPart[j] = sysParticles[j].getCurrentPosition(); //Copy position values
			}
			break;
		}
	}




}
