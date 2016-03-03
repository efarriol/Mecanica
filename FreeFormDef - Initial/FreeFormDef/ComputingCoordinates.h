#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


class ComputingCoordinates{
	int ndivx, ndivy, ndivz;
	float xmin, xmax, ymin, ymax, zmin, zmax, l, m, n, dx, dy, dz;
	glm::vec3 Q, U, V, W;
	std::vector<glm::vec3> grid;
	std::vector<glm::vec3> gridAABB;
public:
	ComputingCoordinates();
	ComputingCoordinates(int _ndivx, int _ndivy, int _ndivz);
	~ComputingCoordinates();

	std::vector<glm::vec3>  ComputeGridCoordenates();
	glm::vec3 ComputeLocalCoordinates(glm::vec3& point);
	void CalcualteLengths(std::vector<glm::vec3>& _vertexData);
	std::vector<glm::vec3> AABBconstruction();
};

