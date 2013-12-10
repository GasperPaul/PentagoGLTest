/*
 *  Created on: 9 груд. 2013
 *      Author: Gasper
 */

#ifndef UTILS_H
#define UTILS_H

#include <GL/glu.h>

Point3D UnProj(Point2D cursorPosition) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)cursorPosition.x;
	winY = (float)viewport[3] - (float)cursorPosition.y;
	glReadPixels(cursorPosition.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return { posX, posY, posZ };
}

#endif /* UTILS_H */
