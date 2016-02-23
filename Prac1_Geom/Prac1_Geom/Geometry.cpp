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

bool Line::isInside(const Point& point){ 
	glm::vec3 Q = this->point.position + direction;
	glm::vec3 to_P = this->point.position - point.position;
	glm::vec3 to_Q = Q - point.position;
	float sumMod = glm::length(to_P) + glm::length(to_Q);
	float modPQ = glm::length(Q - point.position);
	if (sumMod > modPQ + glm::pow(-10.0f, -7.0f) && sumMod < modPQ + glm::pow(10.0f, -7.0f)) return true;
	return false;
};

bool Line::isInside(const glm::vec3& punt){ 
	glm::vec3 Q = this->point.position + direction;
	glm::vec3 to_P = this->point.position - punt;
	glm::vec3 to_Q = Q - punt;
	float sumMod = glm::length(to_P) + glm::length(to_Q);
	float modPQ = glm::length(Q - punt);
	if (sumMod > modPQ + glm::pow(-10.0f,-7.0f) && sumMod < modPQ + glm::pow(10.0f, -7.0f)) return true;
	return false;
};

float Line::distLine2Point(const Point& point){
	glm::vec3 vectorPA = this->point.position - point.position;
	float distance = (glm::length(glm::cross(vectorPA, direction)) / glm::length(direction));
	return distance;
};

glm::vec3 Line::closestPointInLine(const Point& point){
	float alpha = projectPointToLine(point);
	glm::vec3 closestPoint = this->point.position + alpha * direction;

	return closestPoint;
};

float Line::distLine2Line(const Line& line){
	glm::vec3 PQ = this->point.position - line.point.position; 
	float distance = (glm::length(PQ * (glm::cross(direction, line.direction))) / glm::length(glm::cross(direction, line.direction)));
	return distance;
}
float Line::projectPointToLine(const Point & point){
	glm::vec3 vecPQ = point.position - this->point.position;
	float alpha = glm::dot(direction, vecPQ) / glm::dot(direction, direction);
	return alpha;
};


//****************************************************
// Plane
//****************************************************
Plane::Plane() {};

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
	float value = normal.x * point.x + normal.y * point.y + normal.z * point.z + dconst;
	if (value > glm::pow(-10.0f, -1.0f) && value < glm::pow(10.0f, -1.0f)) return true; //hay que poner que sea |en valor abs| < 10 ^-7
	return false;
};

float Plane::distPoint2Plane(const glm::vec3& point){ //return dist with sign	
	float distance = (normal.x * point.x + normal.y * point.y + normal.z * point.z + dconst) / (glm::length(normal));
	return distance;  
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
	float alpha = ((-dconst - glm::dot(normal, point)) / glm::dot(normal, normal));
	glm::vec3 closestPoint = point + alpha*normal;
	return closestPoint;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	glm::vec3 vectorP1P2 = point1 - point2;
	float alpha = ((-dconst - glm::dot(normal, point1)) / glm::dot(normal, vectorP1P2));
	glm::vec3 intersecPoint = point1 + alpha*vectorP1P2;
	glm::vec3 vectorOneSide = point1 - intersecPoint;
	glm::vec3 vectorOtherSide = point2 - intersecPoint;
	float tester = glm::dot(vectorOneSide, vectorOtherSide);
	if (tester < 0.0f) {  // si fem el escalar dels vectors P1 a intersecPoint i P2 a intersecPoint i dona < 0 significa que tenen sentit contrari i per tant, el punt es troba dins el segment.
		if (intersecPoint == pTall) return true;
	}
	pTall = intersecPoint;
	return false;
};

bool Plane::intersecLinePlane(const Line& line, glm::vec3& pTall) {
	glm::vec3 vectorP1P2 = line.point.position - (line.point.position + line.direction);
	float alpha = ((-dconst - glm::dot(normal, line.point.position)) / glm::dot(normal, vectorP1P2));
	glm::vec3 intersecPoint = line.point.position + alpha*vectorP1P2;
	
	if (intersecPoint == pTall)	return true;
	//pTall = intersecPoint;
	return false;
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
	float areaTriangle = CalculateTriangleArea(vertex1, vertex2, vertex3);
	float area1 = CalculateTriangleArea(point, vertex1, vertex2);
	float area2 = CalculateTriangleArea(point, vertex2, vertex3);
	float area3 = CalculateTriangleArea(point, vertex3, vertex1);
	float totalArea = area1 + area2 + area3;

	if (totalArea - areaTriangle > glm::pow(-10.0f, -1.0f) && totalArea < glm::pow(10.0f,-1.0f)) return true;
	return false;
};

bool Triangle::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	if (isInside(pTall)) {
		glm::vec3 normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
		float dConst = -glm::dot(vertex1, normal);
		glm::vec3 vectorP1P2 = point1 - point2;
		float alpha = ((-dconst - glm::dot(normal, point1)) / glm::dot(normal, vectorP1P2));
		glm::vec3 intersecPoint = point1 + alpha*vectorP1P2;
		glm::vec3 vectorOneSide = point1 - intersecPoint;
		glm::vec3 vectorOtherSide = point2 - intersecPoint;
		float tester = glm::dot(vectorOneSide, vectorOtherSide);
		if (tester < 0) {  // si fem el escalar dels vectors P1 a intersecPoint i P2 a intersecPoint i dona < 0 significa que tenen sentit contrari i per tant, el punt es troba dins el segment.
			if (intersecPoint == pTall) return true;
		}
		pTall = intersecPoint;
	}
	return false;
};

float Triangle::CalculateTriangleArea(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3){
	glm::vec3 vector_v = vertex2 - vertex1;
	glm::vec3 vector_u = vertex3 - vertex1;
	float areaTriangle = (0.5f) * glm::length(glm::cross(vector_v, vector_u));
	return areaTriangle;
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
	if (isInside(pTall)) {
		glm::vec3 vect1 = point2 - point1;
		float a = glm::dot(vect1, vect1);
		float b = glm::dot(glm::vec3(2)*vect1, (point1 - center));
		float c = glm::dot(center, center) + glm::dot(point1, point1) - glm::dot(glm::vec3(2)*point1, center) - glm::pow(radi, 2.0f);

		if ((glm::pow(b, 2.0f) - 4 * a*c) / (2 * a) >= 0 && a!= 0) {
			float result1 = (-b + glm::sqrt(glm::pow(b, 2.0f) - 4 * a*c) / (2 * a));
			float result2 = (-b - glm::sqrt(glm::pow(b, 2.0f) - 4 * a*c) / (2 * a));
			glm::vec3 intersectPoint1 = (1-result1)*point1 + result1*vect1;
			glm::vec3 intersectPoint2 = (1-result2)*point1 + result2*vect1;
			if (intersectPoint1 == pTall || intersectPoint2 == pTall) return true;
		} 
	}
	else return false; 
};

//****************************************************
// Cylinder
//****************************************************

Cylinder::Cylinder() {};

Cylinder::Cylinder(const float & radious, glm::vec3 & point1, glm::vec3 & point2){
	radi = radious;
	topPoint = point1;
	bottomPoint = point2;
	director = topPoint - bottomPoint;
}

void Cylinder::setPosition(const glm::vec3 & newPosTop, const glm::vec3& newPosBottom){
	topPoint = newPosTop;
	bottomPoint = newPosBottom;
}

bool Cylinder::isInside(const glm::vec3 & point){
	glm::vec3 vect1 = topPoint - point;
	glm::vec3 vect2 = bottomPoint - point;
	float tester = glm::dot(vect1, vect2);
	if (tester < 0) {
		float distance = glm::length(glm::cross(topPoint - point, director)) / glm::length(director);
		if (distance <= radi) return true;
	}
	return false;
}

bool Cylinder::intersecSegment(const glm::vec3 & point1, const glm::vec3 & point2, glm::vec3 & ptall){

	return false;
}


//****************************************************
// Capsule
//****************************************************

Capsule::Capsule() {};

Capsule::Capsule(const float & radious, glm::vec3 & point1, glm::vec3 & point2){
	radi = radious;
	topPoint = point1;
	bottomPoint = point2;
	director = topPoint - bottomPoint;
}

void Capsule::setPosition(const glm::vec3 & newPosTop, const glm::vec3 & newPosBottom)
{
	topPoint = newPosTop;
	bottomPoint = newPosBottom;
}

bool Capsule::isInside(const glm::vec3 & point)
{
	if(Cylinder::isInside(point)) return true;

	else {
		Sphere topSphere(topPoint, radi);
		Sphere bottomSphere(bottomPoint, radi);
		if (topSphere.isInside(point) || bottomSphere.isInside(point)) return true;
	}
	return false;
}

bool Capsule::intersecSegment(const glm::vec3 & point1, const glm::vec3 & point2, glm::vec3 & ptall)
{
	return false;
}
