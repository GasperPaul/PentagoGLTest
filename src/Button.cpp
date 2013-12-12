/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#include "Button.h"

Button::Button(Point2D* _verteces) : verteces { _verteces[0], _verteces[1] } { }

Button::Button(Point2D a, Point2D b) : verteces { a, b } { }

Button::~Button() { }

void Button::Draw() {
	Point3D tmp[2] = { GLutils::UnProject(verteces[0]), GLutils::UnProject(verteces[1]) };
	glBegin(GL_QUADS);
		glVertex2d(tmp[0].x, tmp[0].y);
		glVertex2d(tmp[1].x, tmp[0].y);
		glVertex2d(tmp[1].x, tmp[1].y);
		glVertex2d(tmp[0].x, tmp[1].y);
	glEnd();
}

void Button::Clicked(Point2D m_coord) {
	if (m_coord.x < verteces[1].x && m_coord.x > verteces[0].x && m_coord.y < verteces[1].y && m_coord.y > verteces[0].y)
		for(auto event : OnClick)
			event(this);
}

Button Button::operator+=(EventCallback e) {
	OnClick.push_back(e);
	return *this;
}
