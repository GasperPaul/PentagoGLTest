/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#ifndef STONE_H
#define STONE_H

#include <vector>
#include <cmath>
#include "GameObject.h"

typedef void (*EventCallback)(GameObject* sender);

class Stone : public GameObject {
public:
	enum Type { Empty, White, Black=-1 };

	Stone(Point2D, double);
	Stone(double, double, double);
	virtual ~Stone();

	virtual void Draw();
	virtual void Clicked(Point2D);
	Stone operator+=(EventCallback click);
	Point2D GetCenter();
	double GetRadius();
	void IsSet(Type);
private:
	Point2D center;
	double radius;
	std::vector<EventCallback> OnClick;
	Type isSet = Empty;
};

#endif /* STONE_H */
