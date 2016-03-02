#include "Mesh.h"

Mesh::Mesh(){

}

void Mesh::loadObjModel(std::string filename) {
	int a, b, c;
	std::ifstream inputFile(filename, std::ios::in);
	if (!inputFile)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}
	std::string line;
	while (std::getline(inputFile, line))
	{
		if (line.substr(0, 2) == "v "){
			std::istringstream v(line.substr(2));
			glm::vec3 vert;
			double x, y, z;
			v >> x; v >> y; v >> z;
			vert = glm::vec3(x, y, z);
			vertices.push_back(vert);
		}
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream v(line.substr(2));
			glm::vec3 normal;
			double x, y, z;
			v >> x; v >> y; v >> z;
			normal = glm::vec3(x, y, z);
			normals.push_back(normal);
		}
		else if (line.substr(0, 3) == "vt ") {
			std::istringstream v(line.substr(2));
			glm::vec2 text;
			double x, y;
			v >> x; v >> y;
			text = glm::vec2(x, y);
			textures.push_back(text);
		}
		else if (line.substr(0, 2) == "f ") {
			std::string lineVals = line.substr(2);
			std::string val0 = lineVals.substr(0, lineVals.find_first_of(' '));
				// Get first group of values
				std::string g1 = val0.substr(0, val0.find(' '));
				std::istringstream v1(g1);
				v1 >> a;
				// Get second group of values
				std::string g2 = line.substr(line.find(' ') + 2);
				g2 = g2.substr(g2.find(' ') + 1);
				std::istringstream v2(g2);
				v2 >> b;
				// Get third group of values
				g2 = g2.substr(0, g2.find(' '));
				std::string g3 = line.substr(line.find_last_of(' ') + 1);
				std::istringstream v3(g3);
				v3 >> c;
    			faceIndex.push_back(glm::i32vec3(a, b, c)); 
		}
	}
}

void  Mesh::setVertex(const int i, const float& x, const float& y, const float& z){
	vertices[i].x = x;
	vertices[i].y = y;
	vertices[i].z = z;
}

std::vector<glm::vec3> Mesh::returnMeshVertices() {
	return vertices;
}

std::vector<glm::vec3> Mesh::returnMeshNormals() {
	return normals;
}

std::vector<glm::vec2> Mesh::returnMeshTextures() {
	return textures;
}

std::vector<glm::i32vec3> Mesh::returnMeshFaces() {  //T
	return faceIndex;
}

void Mesh::printObjModel(Mesh &mesh, std::string filename){ //T
	std::ofstream fileOut(filename, std::ios::out);
	if (!fileOut)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}
	fileOut << "#Output OBJ file \n";
	for (int i = 0; i < mesh.vertices.size(); i++) {
		fileOut << "v " << mesh.vertices[i].x << "   " << mesh.vertices[i].y << "   " << mesh.vertices[i].z << "\n";
	}
	for (int i = 0; i < mesh.textures.size(); i++) {
		fileOut << "vt " << mesh.textures[i].x << "   " << mesh.textures[i].y << "\n";
	}
	for (int i = 0; i < mesh.normals.size(); i++) {
		fileOut << "vn " << mesh.normals[i].x << "   " << mesh.normals[i].y << "   " << mesh.normals[i].z << "\n";
	}
	for (int i = 0; i < mesh.faceIndex.size(); i++){
		fileOut << "f " << mesh.faceIndex[i].x << "   " << mesh.faceIndex[i].y << "   " << mesh.faceIndex[i].z << "\n";
	}
	fileOut.close();
}