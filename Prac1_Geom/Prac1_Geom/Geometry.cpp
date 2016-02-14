#pragma once
#include "Geometry.h"

//****************************************************
// Point
//****************************************************

Point::Point(){
};

Point::Point(const float& x, const float& y, const float& z){
	position.x = x;
	position.y = y;
	position.z = z;
};

Point::Point(const glm::vec3& newPos){
	position = newPos;
};

void Point::setPosition(const glm::vec3& newPos){
	position = newPos;

};

void Point::setPosition(const float& x, const float& y, const float& z){
	position.x = x;
	position.y = y;
	position.z = z;
};

float Point::distPoint2Point(const Point& punt){
	float dist;
	return dist = glm::length(this->position - punt.position);
}

bool Point::isInside(const glm::vec3& point){ return false; };

Point Point::pointInSegment(const Point& q, const float& alfa){
	Point r = (1 - alfa)*(this->position) + alfa*q.position;
	return r;
};


//****************************************************
// Line
//****************************************************

Line::Line()
{
};

Line::Line(const glm::vec3& origen, const glm::vec3& vector){
	point.position = origen;
	direction = vector;
};

Line::Line(const Point& origen, const Point& extrem){
	point.position = origen.position;
	direction = extrem.position - origen.position;
};

void Line::setPosition(const glm::vec3& newPos){
	point.position = newPos;
};

void Line::setDirection(const glm::vec3& newDir){
	direction = newDir;
};

bool Line::isInside(const Point& point){ //not yet implemented

};

bool Line::isInside(const glm::vec3& punt){ //not yet implemented

};

float Line::distLine2Point(const Point& point){

};

glm::vec3 Line::closestPointInLine(const Point& point){


};

float Line::distLine2Line(const Line& line){

};

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect){
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos){
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){

};

float Plane::distPoint2Plane(const glm::vec3& point){ //return dist with sign

};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){

};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){

};

bool Plane::intersecLinePlane(const Line& line, glm::vec3& pTall) {

};




//****************************************************
// Triangle
//****************************************************

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex2));
};

void Triangle::setPosition(const glm::vec3& newPos){
	glm::vec3 baryc;
	baryc = (vertex1 + vertex2 + vertex3) / 3.0f;     //displacement of the barycenter 
	glm::vec3 disp = newPos - baryc; //is only considered
	vertex1 = vertex1 + disp;
	vertex2 = vertex2 + disp;
	vertex3 = vertex3 + disp;
};

bool Triangle::isInside(const glm::vec3& point){

};

bool Triangle::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){

};

//****************************************************
// Sphere
//****************************************************

Sphere::Sphere(const glm::vec3& point, const float& radious){
	center = point;
	radi = radious;
};

void Sphere::setPosition(const glm::vec3& newPos){
	center = newPos;
};

bool Sphere::isInside(const glm::vec3& point){
	float dist = glm::length(point - center);
	if (dist > radi) return false;
	return true;
};

bool Sphere::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){

};
