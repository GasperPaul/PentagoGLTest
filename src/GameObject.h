/*
 *  Created on: 9 груд. 2013
 *      Author: Павло
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "structs.h"

class GameObject {
public:
	virtual void Draw() = 0;
	virtual void Clicked(Point2D) = 0;
	virtual ~GameObject() { };
};


#endif /* GAMEOBJECT_H */
