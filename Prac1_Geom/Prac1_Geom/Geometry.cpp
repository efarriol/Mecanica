#pragma once
#include "Geometry.h"

//****************************************************
// Point
//****************************************************

Point::Point() {
};

Point::Point(const float& x, const float& y, const float& z) {
	position.x = x;
	position.y = y;
	position.z = z;
};

Point::Point(const glm::vec3& newPos) {
	position = newPos;
};

void Point::setPosition(const glm::vec3& newPos) {
	position = newPos;

};

void Point::setPosition(const float& x, const float& y, const float& z) {
	position.x = x;
	position.y = y;
	position.z = z;
};

float Point::distPoint2Point(const Point& punt) {
	float dist;
	return dist = glm::length(this->position - punt.position);
}

bool Point::isInside(const glm::vec3& point) { return false; };

Point Point::pointInSegment(const Point& q, const float& alfa) {
	Point r = (1 - alfa)*(this->position) + alfa*q.position;
	return r;
};


//****************************************************
// Line
//****************************************************

Line::Line()
{
};

Line::Line(const glm::vec3& origen, const glm::vec3& vector) {
	point.position = origen;
	direction = vector;
};

Line::Line(const Point& origen, const Point& extrem) {
	point.position = origen.position;
	direction = extrem.position - origen.position;
};

void Line::setPosition(const glm::vec3& newPos) {
	point.position = newPos;
};

void Line::setDirection(const glm::vec3& newDir) {
	direction = newDir;
};

bool Line::isInside(const Point& point) {
	glm::vec3 Q = this->point.position + direction;
	glm::vec3 to_P = this->point.position - point.position;
	glm::vec3 to_Q = Q - point.position;
	float sumMod = glm::length(to_P) + glm::length(to_Q);
	float modPQ = glm::length(Q - point.position);
	if (sumMod > modPQ + glm::pow(-10.0f, -5.0f) && sumMod < modPQ + glm::pow(10.0f, -5.0f)) return true;
	return false;
};

bool Line::isInside(const glm::vec3& punt) {
	glm::vec3 Q = this->point.position + direction;
	glm::vec3 to_P = this->point.position - punt;
	glm::vec3 to_Q = Q - punt;
	float sumMod = glm::length(to_P) + glm::length(to_Q);
	float modPQ = glm::length(Q - punt);
	if (sumMod > modPQ + glm::pow(-10.0f, -5.0f) && sumMod < modPQ + glm::pow(10.0f, -5.0f)) return true;
	return false;
};

float Line::distLine2Point(const Point& point) {
	glm::vec3 vectorPA = this->point.position - point.position;
	float distance = (glm::length(glm::cross(vectorPA, direction)) / glm::length(direction));
	return distance;
};

glm::vec3 Line::closestPointInLine(const Point& point) {
	float alpha = projectPointToLine(point);
	glm::vec3 closestPoint = this->point.position + alpha * direction;

	return closestPoint;
};

float Line::distLine2Line(const Line& line) {
	glm::vec3 PQ = this->point.position - line.point.position;
	float distance = (glm::length(PQ * (glm::cross(direction, line.direction))) / glm::length(glm::cross(direction, line.direction)));
	return distance;
}
float Line::projectPointToLine(const Point & point) {
	glm::vec3 vecPQ = point.position - this->point.position;
	float alpha = glm::dot(direction, vecPQ) / glm::dot(direction, direction);
	return alpha;
};


//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect) {
	normal = glm::normalize(normalVect);
	punt = point;
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2) {
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos) {
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point) {
	float value = normal.x * point.x + normal.y * point.y + normal.z * point.z + dconst;
	if (value > glm::pow(-10.0f, -5.0f) && value < glm::pow(10.0f, -5.0f)) return true; //hay que poner que sea |en valor abs| < 10 ^-7
	return false;
};

float Plane::distPoint2Plane(const glm::vec3& point) { //return dist with sign	
	float distance = (normal.x * point.x + normal.y * point.y + normal.z * point.z + dconst) / (glm::length(normal));
	return distance;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point) {
	float alpha = ((-dconst - glm::dot(normal, point)) / glm::dot(normal, normal));
	glm::vec3 closestPoint = point + alpha*normal;
	return closestPoint;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	glm::vec3 vect1 = point1 - punt;
	glm::vec3 vect2 = point2 - punt;
	float direction1 = glm::dot(normal, vect1);
	float direction2 = glm::dot(normal, vect2);

	if (direction1 < 0 && direction2 >= 0 || direction1 >= 0 && direction2 < 0) {
		glm::vec3 vectorP1P2 = point1 - point2;
		float alpha = ((-dconst - glm::dot(normal, point1)) / glm::dot(normal, vectorP1P2));
		pTall = point1 + alpha*vectorP1P2;
		puntTall = pTall;
		return true;
	}
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

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2) {
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex2));
}

void Triangle::setPosition(const glm::vec3& newPos) {
	glm::vec3 baryc;
	baryc = (vertex1 + vertex2 + vertex3) / 3.0f;     //displacement of the barycenter 
	glm::vec3 disp = newPos - baryc; //is only considered
	vertex1 = vertex1 + disp;
	vertex2 = vertex2 + disp;
	vertex3 = vertex3 + disp;
};

bool Triangle::isInside(const glm::vec3& point) {
	float areaTriangle = CalculateTriangleArea(vertex1, vertex2, vertex3);
	float area1 = CalculateTriangleArea(point, vertex1, vertex2);
	float area2 = CalculateTriangleArea(point, vertex2, vertex3);
	float area3 = CalculateTriangleArea(point, vertex3, vertex1);
	float totalArea = area1 + area2 + area3;

	if (totalArea - areaTriangle > glm::pow(-10.0f, -5.0f) && totalArea < glm::pow(10.0f, -5.0f)) return true;
	return false;
};

bool Triangle::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	Plane trianglePlane(vertex1, vertex2, vertex3);
	if (trianglePlane.intersecSegment(point1, point2, pTall)) {
		if (isInside(pTall)) {
			puntTall = pTall;
			return true;
		}
	}
	return false;
};

float Triangle::CalculateTriangleArea(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3) {
	glm::vec3 vector_v = vertex2 - vertex1;
	glm::vec3 vector_u = vertex3 - vertex1;
	float areaTriangle = (0.5f) * glm::length(glm::cross(vector_v, vector_u));
	return areaTriangle;
};

//****************************************************
// Sphere
//****************************************************

Sphere::Sphere(const glm::vec3& point, const float& radious) {
	center = point;
	radi = radious;
};

void Sphere::setPosition(const glm::vec3& newPos) {
	center = newPos;
};

bool Sphere::isInside(const glm::vec3& point) {
	float dist = glm::length(point - center);
	if (dist > radi) return false;
	return true;
};

bool Sphere::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {

	glm::vec3 vect1 = center - point1;
	glm::vec3 vect2 = center - point2;
	if (glm::length(vect1) <= radi && glm::length(vect2) > radi || glm::length(vect1) >= radi && glm::length(vect2) < radi) {
		vect1 = glm::normalize(point2 - point1);
		float a = glm::dot(vect1, vect1);
		float b = glm::dot(glm::vec3(2)*vect1, (point1 - center));
		float c = glm::dot(center, center) + glm::dot(point1, point1) - glm::dot(glm::vec3(2)*point1, center) - glm::pow(radi, 2.0f);

		if ((glm::pow(b, 2.0f) - 4 * a*c) / (2 * a) >= 0 && a != 0) {
			float result1 = (-b + glm::sqrt(glm::pow(b, 2.0f) - 4.0f * a*c)) / (2.0f * a);
			float result2 = (-b - glm::sqrt(glm::pow(b, 2.0f) - 4.0f * a*c)) / (2.0f * a);
			glm::vec3 intersectPoint1 = (1 - result1)*point1 + result1*vect1;
			glm::vec3 intersectPoint2 = (1 - result2)*point1 + result2*vect1;
			pTall = intersectPoint1;
			puntTall = pTall;
			return true;
		}
	}
	return false;
};

//****************************************************
// Cylinder
//****************************************************


Cylinder::Cylinder(const float & radious, glm::vec3 & point1, glm::vec3 & point2) {
	radi = radious;
	topPoint = point1;
	bottomPoint = point2;
	director = topPoint - bottomPoint;
}

void Cylinder::setPosition(const glm::vec3 & newPosTop, const glm::vec3& newPosBottom) {
	topPoint = newPosTop;
	bottomPoint = newPosBottom;
}

bool Cylinder::isInside(const glm::vec3 & point) {
	glm::vec3 vect1 = topPoint - point;
	glm::vec3 vect2 = bottomPoint - point;
	float tester = glm::dot(vect1, vect2);
	if (tester < 0) {
		float distance = glm::length(glm::cross(topPoint - point, director)) / glm::length(director);
		if (distance <= radi) return true;
	}
	return false;
}

bool Cylinder::intersecSegment(const glm::vec3 & point1, const glm::vec3 & point2, glm::vec3 & ptall) {

	return false;
}


//****************************************************
// Capsule
//****************************************************


Capsule::Capsule(const float & radious, glm::vec3 & point1, glm::vec3 & point2) {
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
	if (Cylinder::isInside(point)) return true;

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


//****************************************************
// Box
//****************************************************


Box::Box(const glm::vec3 & point1, const glm::vec3 & point2, const glm::vec3 & point3, const glm::vec3 & point4, const glm::vec3 & vector){
	vertex1 = point1;
	vertex2 = point2;
	vertex3 = point3;
	vertex4 = point4;
	vertex5 = point1 + vector;
	vertex6 = point2 + vector;
	vertex7 = point3 + vector;
	vertex8 = point4 + vector;
	
	normal1 = glm::cross(vertex4 - vertex1, vertex2 - vertex1);		//normal del plano 1,2,3,4
	normal2 = glm::cross(vertex5 - vertex1, vertex4 - vertex1);		//normal del plano 1,4,5,8
	normal3 = glm::cross(vertex2 - vertex1, vertex5 - vertex1);		//normal del plano 1,2,5,6
	normal4 = glm::cross(vertex3 - vertex2, vertex6 - vertex2);		//normal del plano 2,3,7,6
	normal5 = glm::cross(vertex4 - vertex3, vertex7 - vertex3);		//normal del plano 3,4,7,8
	normal6 = glm::cross(vertex6 - vertex5, vertex8 - vertex5);		//normal del plano 5,6,7,8

	Plane plane1(vertex1, normal1), plane2(vertex1, normal2),
		plane3(vertex1, normal3), plane4(vertex2, normal4),
		plane5(vertex3, normal5), plane6(vertex5, normal6);
	pla1 = plane1;
	pla2 = plane2;
	pla3 = plane3;
	pla4 = plane4;
	pla5 = plane5;
	pla6 = plane6;

	faceCut = 0;


}

void Box::setPosition(const glm::vec3 & newPos){
	vertex1 = vertex1 + newPos;
	vertex2 = vertex2 + newPos;
	vertex3 = vertex3 + newPos;
	vertex4 = vertex4 + newPos;
	vertex5 = vertex5 + newPos;
	vertex6 = vertex6 + newPos;
	vertex7 = vertex7 + newPos;
	vertex8 = vertex8 + newPos;
}

bool Box::isInside(const glm::vec3 & point) {
	glm::vec3 direction1 = vertex1 - point; //cara d'abaix
	glm::vec3 direction2 = vertex5 - point; //cara de dalt
	glm::vec3 direction3 = vertex1 - point; //cara izq
	glm::vec3 direction4 = vertex2 - point; //cara der
	glm::vec3 direction5 = vertex1 - point; //cara front
	glm::vec3 direction6 = vertex4 - point; //cara post
	if (glm::dot(direction1, direction2) < 0 && glm::dot(direction3, direction4) < 0 && glm::dot(direction5, direction6) < 0) return true;
	return false;
}

bool Box::intersecSegment(const glm::vec3 & point1, const glm::vec3 & point2, glm::vec3 & ptall){
	

	faceCut = 0;
	if(pla1.intersecSegment(point1,point2, ptall)){  
		if (projectsInsidePlane(vertex1, vertex2, vertex4, pla1.puntTall)) {
			faceCut = 1;
			return true;
		}
	}

	if (pla2.intersecSegment(point1, point2, ptall)){
		if (projectsInsidePlane(vertex4, vertex8, vertex1, pla2.puntTall)) {
			faceCut = 2;
			return true;
		}
	}

	if (pla3.intersecSegment(point1, point2, ptall)) {
		if (projectsInsidePlane(vertex1, vertex2, vertex5, pla3.puntTall)) {
			faceCut = 3;
			return true;
		}
	}

	if (pla4.intersecSegment(point1, point2, ptall)) {
		if (projectsInsidePlane(vertex2, vertex3, vertex6, pla4.puntTall)) {
			faceCut = 4;
			return true;
		}
	}

	if (pla5.intersecSegment(point1, point2, ptall)) {
		if (projectsInsidePlane(vertex3, vertex4, vertex7, pla5.puntTall)) {
			faceCut = 5;
			return true;
		}
	}

	if (pla6.intersecSegment(point1, point2, ptall)) {
		if (projectsInsidePlane(vertex5, vertex6, vertex8, pla6.puntTall)) {
			faceCut = 6;
			return true;
		}
	}

	return false;
}

void Box::printSolution(int counter, Box box, std::vector<Point> punts, Plane cutPlane){

	std::cout << "Punt interseccio entre: " << std::endl;
	std::cout << "	x: " << punts[counter - 1].position.x << " i x2: " << punts[counter].position.x << std::endl;
	std::cout << "	y: " << punts[counter - 1].position.y << " i y2: " << punts[counter].position.y << std::endl;
	std::cout << "	z: " << punts[counter - 1].position.z << " i z2: " << punts[counter].position.z << std::endl;
	std::cout << "\nPunt interseccio exacte en la cara de la box: " << box.faceCut << std::endl;
	std::cout << "	x: " << cutPlane.puntTall.x << std::endl;
	std::cout << "	y: " << cutPlane.puntTall.y << std::endl;
	std::cout << "	z: " << cutPlane.puntTall.z << std::endl;

}

bool Box::projectsInsidePlane(const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, const glm::vec3 & cutPoint){
	glm::vec3 vertexVector = vertex2 - vertex1;
	glm::vec3 vertexVector2 = vertex3 - vertex1;
	float alpha = glm::dot(vertexVector, cutPoint - vertex1) / glm::dot(vertexVector, vertexVector);
	if (alpha >= 0 && alpha <= 1) {
		alpha = glm::dot(vertexVector2, cutPoint - vertex1) / glm::dot(vertexVector2, vertexVector2);
		if (alpha >= 0 && alpha <= 1) return true;
	}
	return false;
}	