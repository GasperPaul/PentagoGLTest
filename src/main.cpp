#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "Figures.h"
#include <iostream>
using namespace std;

struct Point3D{
	float x;
	float y;
	float z;
} translate = {0, 0, 0};

struct Point2D {
	double x;
	double y;
} oldPos;

struct cursor {
	Point2D position;
	bool moving;
} cursor;

static void errorCallback(int error, const char* errorText) {
	cout << "Error: " << errorText << endl;
}

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key){
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		translate.y -= 0.05;
		break;
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		translate.y += 0.05;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		translate.x += 0.05;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		translate.x -= 0.05;
		break;
	}
}

Point3D UnProject(Point2D cursorPosition) {
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

static void mouseCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			{
				Point3D tmp = UnProject(cursor.position);
				Triangle a(-0.6, -0.4, 0.6, -0.4, tmp.x, tmp.y);
				Triangle b(0.0, 0.6, 0.6, -0.4, tmp.x, tmp.y);
				Triangle c(-0.6, -0.4, 0.0, 0.6, tmp.x, tmp.y);
				Triangle d(-0.6, -0.4, 0.6, -0.4, 0.0, 0.6);
				if (a.Area()+b.Area()+c.Area() == d.Area())
					cursor.moving = true;
			}
			break;
		case GLFW_RELEASE:
			cursor.moving = false;
			break;
		}
	}
}

static void mouseMoveCallback(GLFWwindow* window, double x, double y) {
	if(cursor.moving) {
		oldPos = cursor.position;
		glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
		translate.x += (cursor.position.x - oldPos.x)/100;
		translate.y += (oldPos.y - cursor.position.y)/100;
	}
}

int main() {
	glfwSetErrorCallback(errorCallback);

	GLFWwindow* window;
	if (!glfwInit()) return -1;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

	while (!glfwWindowShouldClose(window)) {
		/* Render here */

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(translate.x, translate.y, 0);
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		/* End render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
