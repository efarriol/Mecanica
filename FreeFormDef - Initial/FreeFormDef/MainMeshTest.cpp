#pragma once
#include "mesh.h"
#include "ComputingCoordinates.h"
#include <iostream>
#include <chrono>

/**
 MeshTest:
 1. Load an OBJ file, 
 2. Modify some of the vertex coordinates 
 3. Write the modified mesh in a new OBJ file
*/

void main() {
	std::string name = "teapot"; 
	std::string fileIn = name + ".obj";
	std::string fileOut = name + "Mod.obj";

	Mesh mesh;
	std::vector<glm::vec3> vertexData;
	std::vector<glm::i32vec3> faceData;

	//-----------------------------------------------------
	//  Load a 3D mesh from an OBJ file, 
	//  modify the position of the vertex
	//  and write a new OBJ file.
	//-----------------------------------------------------

	mesh.loadObjModel(fileIn);//load a 3d model in obj format
	vertexData = mesh.returnMeshVertices();
	faceData = mesh.returnMeshFaces();
	//vertexData = model.vertices;

	//std::cout << "Number of vertices = " << vertexData.size() << std::endl;
	//	std::cout << "Number of vertices = " << model.OBJIndices.size() << std::endl;
	//for (int i = 0; i < vertexData.size(); i++)
		//std::cout << i << "  " << vertexData[i].x << "  " << vertexData[i].y << "  " << vertexData[i].z << std::endl;

	//std::cout << "Number of faces = " << faceData.size() << std::endl;
	//for (int i = 0; i < faceData.size(); i++)
		//std::cout << i << "  " << faceData[i].x << "  " << faceData[i].y << "  " << faceData[i].z << std::endl;
	//system("PAUSE");

	//As exemple we modify one mesh vertex
	//vertexData[5].x = 2.0f;
	//vertexData[5].y = 2.0f;
	//vertexData[5].z = 2.0f;
	// or the same way:	
	//mesh.setVertex(5, 2.0f, 2.0f, 2.0f);

	// Find Aligned Bounding Box (ABB)
	float maxX = -100000.f; //very small value
	float maxY = -100000.f;
	float maxZ = -100000.f;
	float minX = 100000.f;  //very big value
	float minY = 100000.f;
	float minZ = 100000.f;
	int ndivx = 15;
	int ndivy = 15;
	int ndivz = 15;
	float l, m, n, dx, dy, dz, s, t, u, scaleTaper;
	std::vector<glm::vec3> grid;
	glm::vec3 Q, U, V, W;
	const int HEIGHT = 15;
	const int WIDTH = 15;
	const int DEPTH = 15;
	for (int i = 0; i < vertexData.size(); i++)	{
		if (vertexData[i].x > maxX) maxX = vertexData[i].x;
		if (vertexData[i].y > maxY) maxY = vertexData[i].y;
		if (vertexData[i].z > maxZ) maxZ = vertexData[i].z;
		if (vertexData[i].x < minX) minX = vertexData[i].x;
		if (vertexData[i].y < minY) minY = vertexData[i].y;
		if (vertexData[i].z < minZ) minZ = vertexData[i].z;
	}

	std::cout << "maxims = " << maxX << " " << maxY << " " << maxZ << std::endl;
	std::cout << "minims = " << minX << " " << minY << " " << minZ << std::endl;
	

	maxX++; maxY++; maxZ++;
	minX--; minY--; minZ--;

	l = maxX - minX;
	m = maxY - minY;
	n = maxZ - minZ;

	dx = l / ndivx;
	dy = m / ndivy;
	dz = n / ndivz;

	Q =	glm::vec3(minX, minY, minZ);

	grid.resize((ndivx + 1) * (ndivy + 1) * (ndivz + 1));
	for (float i = 0.f; i <= ndivx; i++) {
		for (float j = 0.f; j <= ndivy; j++) {
			for (float k = 0; k <= ndivz; k++) {
				grid[i].x = Q.x + (i / ndivx);
				grid[i].y = Q.y + (j / ndivy);
				grid[i].z = Q.z + (k / ndivz);
			}
		}
	}

// Convertir a coord locales de la box original
	for (int i = 0; i < vertexData.size(); i++) {
		vertexData[i].x = (vertexData[i].x - Q.x);
		vertexData[i].y = (vertexData[i].y - Q.y);
		vertexData[i].z = (vertexData[i].z - Q.z);
	}


	//Deformar la box
	//Taper
	for (int i = 0; i < grid.size(); i++) {
		if (grid[i].x == minX ) {
			scaleTaper = 1.0f;
		}
		else if (grid[i].x == maxX){
			scaleTaper = 0.5f;
			grid[i].y = grid[i].y * scaleTaper;
			grid[i].z = grid[i].z * scaleTaper;
		}
		else {
			scaleTaper = 1.0f - 0.5f * ((grid[i].x - minX) / (maxX - minX));
			grid[i].y = grid[i].y * scaleTaper;
			grid[i].z = grid[i].z * scaleTaper;
		}
	}
	
	//Interpolarlas con la box deformada

	for (int i = 0; i < vertexData.size(); i++)
	{
		
		int index1 = floor(vertexData[i].x / dx);
		int index2 = floor(vertexData[i].y / dy);
		int index3 = floor(vertexData[i].z / dz);

		
		float xD = ((vertexData[i].x - index1 * dx) / dx);
		float yD = ((vertexData[i].y - index2 * dy) / dy);
		float zD = ((vertexData[i].z - index3 * dz) / dz);
		
		int ind1 = (index1*(HEIGHT + 1) + index2)*(WIDTH + 1) + index3;
		int ind2 = ((index1 + 1)*(HEIGHT + 1) + index2)*(WIDTH + 1) + index3;
		int ind3 = (index1*(HEIGHT + 1) + (index2 + 1))*(WIDTH + 1) + index3;
		int ind4 = ((index1 + 1)*(HEIGHT + 1) + (index2 + 1))*(WIDTH + 1) + index3;

		
		glm::vec3 C000 = grid[ind1];
		glm::vec3 C100 = grid[ind2];
		glm::vec3 C010 = grid[ind3];
		glm::vec3 C110 = grid[ind4];
		
		glm::vec3 C001 = grid[ind1];
		glm::vec3 C101 = grid[ind2];
		glm::vec3 C011 = grid[ind3];
		glm::vec3 C111 = grid[ind4];

		 
		glm::vec3 C00 = C000 * (1 - xD) + C100 * xD;
		glm::vec3 C10 = C010 * (1 - xD) + C110 * xD;
		glm::vec3 C01 = C001 * (1 - xD) + C101 * xD;
		glm::vec3 C11 = C011 * (1 - xD) + C111 * xD;
		glm::vec3 C0 = C00 * (1 - yD) + C10 * yD;
		glm::vec3 C1 = C01 * (1 - yD) + C11 * yD;
		glm::vec3 C = C0 * (1 - zD) + C1 * zD;

	
		vertexData[i] = C;

	}

	




/*
	//As exemple we modify a set of mesh vertices along x-axis
	float rank = 0.5*maxX; //half part of the model
	for (int i = 0; i < vertexData.size(); i++)
	{
		if(vertexData[i].x > rank) //scale vertex
		{
			vertexData[i].x = vertexData[i].x * 2.0f;
			vertexData[i].y = vertexData[i].y * 2.0f;
			vertexData[i].z = vertexData[i].z * 2.0f;
		}
	}
	
*/
	// write the new mesh vertex
	for (int i = 0; i < vertexData.size(); i++) {
		mesh.setVertex(i, vertexData[i].x, vertexData[i].y, vertexData[i].z);
	}
	// create a new file with the modified mesh
	mesh.printObjModel(mesh, fileOut);
	system("PAUSE");
/*
	std::chrono::high_resolution_clock::time_point tini, tfin;
	const int HEIGHT = 200;
	const int WIDTH = 200;
	const int DEPTH = 200;

	/*--------------------------------------
	Vector Version of the 3D grid
	----------------------------------------
	int ind;
	tini = std::chrono::high_resolution_clock::now();
	std::vector<glm::vec3> grid3Dv;

	grid3Dv.resize(HEIGHT*WIDTH*DEPTH);

	// Assign values
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			for (int k = 0; k < DEPTH; ++k) {
				ind = (i*HEIGHT + j)*WIDTH + k;
				grid3Dv[ind].x = i;
				grid3Dv[ind].y = j;
				grid3Dv[ind].z = k;
			}
		}
	}

	tfin = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(tfin - tini).count();
	std::cout << "Time using vector  " << duration << "ms" << std::endl;
	//verify input values
	ind = (100 * HEIGHT + 99)*WIDTH + 98;
	std::cout << "valor al lloc [i][j][k] =  " <<
		grid3Dv[ind].x << " " << grid3Dv[ind].y << " " << grid3Dv[ind].z << std::endl;

	system("PAUSE");
	*/
}