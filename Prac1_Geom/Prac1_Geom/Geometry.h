#pragma once
#include <glm\glm.hpp>
#include <iostream>
struct Geometry{
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
	glm::vec3 normal;
	float dconst;
	Plane(){};
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
	glm::vec3 vertex1, vertex2, vertex3;
	Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
	~Triangle() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	float CalculateTriangleArea(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3);
};
/*
struct Sphere : public Geometry {
	glm::vec3 center;
	float radi;
	Sphere(const glm::vec3& point, const float& radious);
	~Sphere() {};
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};
*/