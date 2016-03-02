#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
This class defines a 3D Mesh object loaded from an OBJ file format 
*/


class Mesh
{
    public:
        Mesh();
		//setters
		void setVertex(const int i, const float& x, const float& y, const float& z);
		//getters
		std::vector<glm::vec3> returnMeshVertices();  //return the vertices of mesh data
		std::vector<glm::vec3> returnMeshNormals();  //return the normals of mesh data
		std::vector<glm::vec2> returnMeshTextures();  //return the normals of mesh data
		std::vector<glm::i32vec3> returnMeshFaces();  //return the face indices of mesh data
		// methods
		void loadObjModel(std::string filename);  //method to load a model in OBJ format
		void printObjModel(Mesh &mesh, std::string filename);  //method to print an OBJ model
private:
	std::vector<glm::vec3> vertices;//to store vertex information of 3D model started with v
	std::vector<glm::vec3> normals; //to store normal information of 3D model started with vn
	std::vector<glm::vec2> textures; //to store texture information of 3D model started with vn
	std::vector<glm::i32vec3> faceIndex;//to store the number of face index started with f

};

