#include "ComputingCoordinates.h"




ComputingCoordinates::ComputingCoordinates(){
}

ComputingCoordinates::ComputingCoordinates(int _ndivx, int _ndivy, int _ndivz){
	ndivx = _ndivx;
	ndivy = _ndivy;
	ndivz = _ndivz;

	Q.x = 0; Q.y = 0; Q.z = 0;
	U.x = 1; U.y = 0; U.z = 0;
	V.x = 0; V.y = 1; V.z = 0;
	W.x = 0; W.y = 0; W.z = 1;
}


ComputingCoordinates::~ComputingCoordinates(){
}

std::vector<glm::vec3> ComputingCoordinates::ComputeGridCoordenates(){
	
	for (float i = 0.f; i < ndivx; i++) {
		for (float j = 0.f; j < ndivy; j++) {
			for (float k = 0; k < ndivz; k++) {
				grid.push_back(Q + (i / ndivx)*U + (j / ndivy)*V + (k / ndivz)*W);
			}
		}
	}
	return grid;
}

glm::vec3 ComputingCoordinates::ComputeLocalCoordinates(glm::vec3& point){
	glm::vec3 P, s, t, u;
	s = (point - Q)*(U / glm::length(U));
	t = (point - Q)*(V / glm::length(V));
	u = (point - Q)*(W / glm::length(W));

	P = Q + s*U + t*V + u*W;
	return P;
}

void ComputingCoordinates::CalcualteLengths(std::vector<glm::vec3>& _vertexData){
	xmax = -100000.f; //very small value
	ymax = -100000.f;
	zmax = -100000.f;
	xmin = 100000.f;  //very big value
	ymin = 100000.f;
	zmin = 100000.f;

	for (int i = 0; i < _vertexData.size(); i++)
	{
		if (_vertexData[i].x > xmax) xmax = _vertexData[i].x;
		if (_vertexData[i].y > ymax) ymax = _vertexData[i].y;
		if (_vertexData[i].z > zmax) zmax = _vertexData[i].z;
		if (_vertexData[i].x < xmin) xmin = _vertexData[i].x;
		if (_vertexData[i].y < ymin) ymin = _vertexData[i].y;
		if (_vertexData[i].z < zmin) zmin = _vertexData[i].z;
	}
	l = xmax - xmin;
	m = ymax - ymin;
	n = zmax - zmin;

	dx = l / ndivx;
	dy = m / ndivy;
	dz = n / ndivz;

}

std::vector<glm::vec3> ComputingCoordinates::AABBconstruction()
{
	for (float i = 0.f; i < ndivx; i++) {
		for (float j = 0.f; j < ndivy; j++) {
			for (float k = 0; k < ndivz; k++) {
				gridAABB.push_back(Q + (i*dx, j*dy, k*dz));
			}
		}
	return gridAABB;
}




