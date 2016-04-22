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
	AABB tempAABB;
	ifstream file;
	file.open(fileName);
	
	if (file.is_open()){
		//TODO: Read the content and add it into the data structure
		file >> _numBasicObjects;
		file >> tempObject._maxCars;
	
		for (int i = 0; i < _numBasicObjects; i++) {
			file >> tempObject._objectType >> tempObject._collisionType >> tempAABB._multiplier >> tempObject._translate.x >> tempObject._translate.y >> tempObject._translate.z >> tempObject._angle
				>> tempObject._rotation.x >> tempObject._rotation.y >> tempObject._rotation.z >> tempObject._scale.x >> tempObject._scale.y >> tempObject._scale.z;
			_listOfObjects.push_back(tempObject);

			if (tempObject._collisionType == 1) {
				// Creation of the AABB
				tempAABB._centre.x = tempObject._translate.x;
				tempAABB._centre.y = tempObject._translate.y;
				tempAABB._centre.z = tempObject._translate.z;
				tempAABB._extent.x = tempObject._scale.x*tempAABB._multiplier;
				tempAABB._extent.y = tempObject._scale.y*tempAABB._multiplier;
				tempAABB._extent.z = tempObject._scale.z*tempAABB._multiplier;
				
				_listOfAABB.push_back(tempAABB);
			
			}
			if (tempObject._collisionType == 2) {
				tempAABB._centre.x = tempObject._translate.x;
				tempAABB._centre.y = tempObject._translate.y;
				tempAABB._centre.z = tempObject._translate.z;
				tempAABB._extent.x = ((minX - maxX ) - 0.05f) / 2;
				tempAABB._extent.y = ((minY - maxY) + 0.3f) / 2;
				tempAABB._extent.z = (minZ - maxZ) / 2;

				_listOfAABB.push_back(tempAABB);
			}
		
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

AABB & Geometry::getAABB(int objectID)
{

	return (_listOfAABB[objectID]);
}

void Geometry::loadBasic3DObjects()
{
	for (int i = 0; i < NUMBASICOBJECTS; i++) {
		switch (i) {
		case 0:
			//Blue cubes
			loadCube(BLUE_CUBE, glm::vec4(0, 0, 255, 255));
			break;
		case 1:
			loadCube(RED_CUBE, glm::vec4(255, 0, 0, 255));
			break;
		case 2:
			loadCube(WHITE_CUBE, glm::vec4(255, 255, 255, 255));
			break;
		case 3:
			_objectLoader.loadAse("./resources/models/taxi.ASE", _numVertices, _verticesData);
			for (int j = 0; j < _numVertices[i]; j++) _verticesData[i][j].setColor(255, 255, 0, 255);
			for (int j = 0; j < _numVertices[i]; j++) {
				if (_verticesData[i][j].position.x > maxX) maxX = _verticesData[i][j].position.x;
				if (_verticesData[i][j].position.y > maxY) maxY = _verticesData[i][j].position.y;
				if (_verticesData[i][j].position.z > maxZ) maxZ = _verticesData[i][j].position.z;
				if (_verticesData[i][j].position.x < minX) minX = _verticesData[i][j].position.x;
				if (_verticesData[i][j].position.y < minY) minY = _verticesData[i][j].position.y;
				if (_verticesData[i][j].position.z < minZ) minZ = _verticesData[i][j].position.z;
			}

			break;
		}
	}
}
