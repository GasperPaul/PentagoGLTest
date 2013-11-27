#ifndef FIGURES_H
#define FIGURES_H

#include <cmath>
#include <cstring>
#include <stdarg.h>

#define PI 3.141592653589793238462

struct Point {
	double x;
	double y;
};

class Figure {
public:
	virtual double Area() = 0;
	virtual ~Figure(){};
};

class Circle : public Figure {
private:
	double radius;
	Point center;

public:
	Circle (double x, double y, double _radius) : radius(_radius) { center = { x, y }; }
	Circle (Point _center, double _radius) : radius(_radius), center(_center) {	}

	double Area() { return PI * std::pow(radius, 2); }
};

class Triangle : public Figure {
private:
	Point vertex[3];

public:
	Triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
		vertex[0] = { x1, y1 };
		vertex[1] = { x2, y2 };
		vertex[2] = { x3, y3 };
	}

	Triangle(Point v1, Point v2, Point v3) : vertex { v1, v2, v3 } { }

	Triangle(Point _vertex[3]) { std::memcpy(vertex, _vertex, 3*sizeof(Point)); }

	double Area() { return std::abs(vertex[0].x * vertex[2].y - vertex[0].x * vertex[1].y +
									vertex[1].x * vertex[0].y - vertex[1].x * vertex[2].y +
									vertex[2].x * vertex[1].y - vertex[2].x * vertex[0].y)/2; }
};

class Rectangle : public Figure {
private:
	Point upperleft, dimensions;
public:
	Rectangle (double x, double y, double width, double height) {
		upperleft = { x, y };
		dimensions = { width, height };
	}

	Rectangle (Point _upperleft, double width, double height) : upperleft(_upperleft) {
		dimensions = { width, height };
	}

	Rectangle (Point _upperleft, Point dim) : upperleft(_upperleft), dimensions(dim) { }

	double Area() { return dimensions.x * dimensions.y; }
};

class Pentagon : public Figure {
private:
	Point center;
	double outerRadius;
public:
	Pentagon(double x, double y, double _outerRadius) : outerRadius(_outerRadius) { center = { x, y }; }

	Pentagon(Point _center, double _outerRadius) : center(_center), outerRadius(_outerRadius) { }

	double Area() { return 5 * std::tan(54*PI/180) * std::pow(outerRadius * std::sin(PI/5), 2); }
};

class Polygon : public Figure {
private:
	unsigned vertexNum;
	Point* verteces;

public:
	Polygon(unsigned _vertexNum, Point p1, ...) : vertexNum(_vertexNum) {
		if (_vertexNum<2) return;
		verteces = new Point[vertexNum];
		va_list args;
		va_start(args, p1);
		verteces[0] = p1;
		for (unsigned i = 1; i < _vertexNum; i++)
			verteces[i] = va_arg(args, Point);
		va_end(args);
	}

	Polygon(unsigned _vertexNum, double x1, double y1, ...) : vertexNum(_vertexNum) {
		if (_vertexNum<2) return;
		verteces = new Point[vertexNum];
		va_list args;
		va_start(args, y1);
		verteces[0] = { x1, y1 };
		for (unsigned i = 1; i < _vertexNum; i++) {
			verteces[i].x = va_arg(args, double);
			verteces[i].y = va_arg(args, double);
		}
		va_end(args);
	}

	Polygon(unsigned _vertexNum, Point* _verteces) : vertexNum(_vertexNum) {
		if (_vertexNum<2) return;
		verteces = new Point[vertexNum];
		std::memcpy(verteces, _verteces, vertexNum*sizeof(Point));
	}

	Polygon(unsigned _vertexNum, double* _verteces) : vertexNum(_vertexNum) {
		if (_vertexNum<2) return;
		verteces = new Point[vertexNum];
		std::memcpy(verteces, _verteces, 2*vertexNum*sizeof(double));
	}

	~Polygon() { delete[] verteces; }

	double Area() {
		double result = 0;
		for (unsigned i = 0; i < vertexNum - 1; i++)
			result += (verteces[i].x * verteces[i+1].y - verteces[i+1].x * verteces[i].y) / 2.0;
		result += (verteces[vertexNum - 1].x * verteces[0].y - verteces[0].x * verteces[vertexNum - 1].y) / 2.0;
		return std::abs(result);
	}
};

#endif /* FIGURES_H */
