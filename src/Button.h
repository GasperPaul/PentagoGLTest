/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include "GameObject.h"

class Button : public GameObject {
public:
	Button(Point2D*);
	Button(Point2D, Point2D);
	Button operator+=(EventCallback);
	virtual void Draw();
	virtual void Clicked(Point2D);
	virtual ~Button();
private:
	std::vector<EventCallback> OnClick;
	Point2D verteces[2];
};

#endif /* BUTTON_H */
