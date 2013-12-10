/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#include "Stone.h"
#include "utils.h"

Stone::Stone(Point2D _center, double _radius) : center(_center), radius(_radius) { }

Stone::Stone(double x, double y, double _r) : center { x, y }, radius(_r) { }

Stone::~Stone() { }

void Stone::Draw(){
	Point3D tmp = UnProj(center);

	float theta = 2 * 3.1415926 / 360.0f;
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = UnProj({ center.x + radius, 0}).x - tmp.x;
	float y = 0;

	if (isSet) {
		glBegin(GL_TRIANGLE_FAN);
		isSet == White ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
	} else {
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
	}
		for(int i = 0; i < 360; i++)
		{
			glVertex2f(x + tmp.x, y + tmp.y);

			//apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
	glEnd();
}

void Stone::Clicked(Point2D cursorCoordinates) {
	double distance = std::sqrt(std::pow(center.x - cursorCoordinates.x, 2) + std::pow(center.y - cursorCoordinates.y, 2));
	if (distance <= radius)
		for(EventCallback event : OnClick)
			event(this);
}

Stone Stone::operator+=(EventCallback click){
	OnClick.push_back(click);
	return *this;
}

Point2D Stone::GetCenter() { return center; }
double Stone::GetRadius() { return radius; }
void Stone::IsSet(Type flag) { isSet = flag; }
