#pragma once
#include <glm\glm.hpp>
#include <iostream>
#include <vector> 
struct Geometry {
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
};

struct Point : public Geometry {
	glm::vec3 position;
	Point();
	~Point() {};
	Point(const float& x, const float& y, const float& z);
	Point(const glm::vec3& newPos);

	void setPosition(const glm::vec3& newPos);
	void setPosition(const float& x, const float& y, const float& z);
	bool isInside(const glm::vec3& point);
	float distPoint2Point(const Point& punt);
	Point pointInSegment(const Point& q, const float& alfa);
};

struct Line : public Geometry {
	Point point;
	glm::vec3 direction;
	Line();
	~Line() {};
	Line(const glm::vec3& origin, const glm::vec3& vector);
	Line(const Point& origen, const Point& extrem);


	void setPosition(const glm::vec3& newPos);
	void setDirection(const glm::vec3& newDir);
	bool isInside(const Point& punt);
	bool isInside(const glm::vec3& punt);
	float distLine2Point(const Point& punt);
	glm::vec3 closestPointInLine(const Point& punt);
	float distLine2Line(const Line& line);
	float projectPointToLine(const Point& point);
};

struct Plane : public Geometry {
	glm::vec3 normal, punt;
	float dconst;
	glm::vec3 puntTall;
	Plane() {};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect);
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& punt1, const glm::vec3& punt2, glm::vec3& pTall);
	bool intersecLinePlane(const Line& line, glm::vec3& pTall);
};

struct Triangle : public Plane {
	glm::vec3 vertex1, vertex2, vertex3, puntTall;
	Triangle() {};
	Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
	~Triangle() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	float CalculateTriangleArea(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3);
};

struct Sphere : public Geometry {
	glm::vec3 center;
	glm::vec3 puntTall;
	float radi;
	Sphere() {};
	Sphere(const glm::vec3& point, const float& radious);
	~Sphere() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};

struct Cylinder : public Geometry {
	glm::vec3 topPoint, bottomPoint, director;
	float radi;
	glm::vec3 puntTall;
	Cylinder() {};
	Cylinder(const float& radious, glm::vec3& point1, glm::vec3& point2);
	~Cylinder() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& ptall);
};

struct Capsule : public Cylinder {
	glm::vec3 puntTall;
	Capsule() {};
	Capsule(const float& radious, glm::vec3& point1, glm::vec3& point2);
	~Capsule() {};
	void setPosition(const glm::vec3& newPosTop, const glm::vec3& newPosBottom);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& ptall);
};

struct Box : public Plane {
	glm::vec3 normal1, normal2, normal3, normal4, normal5, normal6;
	glm::vec3 vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7, vertex8;
	Plane pla1, pla2, pla3, pla4, pla5, pla6;
	int faceCut;
	Box() {};
	Box(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& point4, const glm::vec3& vector); //els punts 1, 2, 3, 4 defineixen la base inferior (definits en sentit antihorari) i el vector sumat als punts defineix els altres 4 vertex.
	~Box() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& ptall);
	bool projectsInsidePlane(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3, const glm::vec3& cutPoint);
};

struct Tetrahedron : public Triangle {
	glm::vec3 vertex1, vertex2, vertex3, vertex4, vector, barycenter1, barycenter2, barycenter3, barycenter4, barycenter5;
	Tetrahedron() {};
	Tetrahedron(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& vector);
	~Tetrahedron() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersectSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};