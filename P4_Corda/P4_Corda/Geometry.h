#pragma once
#include "Vertex.h"
#include <vector>
#include "GameObject.h"
#include "ObjectLoader.h"

#define ASE_OBJECTS 0
#define NUMBASICOBJECTS 1

//This class stores and manipulates all the objects loaded from the text fileddd
class Geometry
{
	std::vector <Vertex *> _verticesData;
	std::vector <int> _numVertices;
	std::vector <GameObject> _listOfObjects;
	glm::vec3 center;
	float max, min, mid;
	int _numBasicObjects;
	float maxX; //very small value
	float maxY;
	float maxZ;
	float minX;  //very big value
	float minY;
	float minZ;

public:
	Geometry();
	~Geometry();
	void loadGameElements(char fileName[100]);
	Vertex * getData(int objectID);
	int getNumVertices(int objectID);
	int getNumGameElements();
	GameObject & getGameElement(int objectID);
	void loadCube(int cube, glm::vec4 color);
	void loadBasic3DObjects();
	ObjectLoader _objectLoader;
};