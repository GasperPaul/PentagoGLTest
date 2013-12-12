/*
 *  Created on: 11 груд. 2013
 *      Author: Gasper
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#include "Stone.h"

enum GameMode { MainMenu, PlayerStep, OponentStep };
GameMode gameMode;

Point3D translate = { 0, 0, 0 };
Point2D oldPos;
tcursor cursor;
std::vector<GameObject*> objects;
std::vector<GameObject*> menuButtons;

static void errorCallback(int error, const char* errorText) {
	cout << "Error: " << errorText << endl;
}

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key){
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
	}
}

static void GameMouseCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			for(GameObject* obj : gameMode == MainMenu ? menuButtons : objects)
				obj->Clicked(cursor.position);
			break;
		}
	}
}

static void mouseMoveCallback(GLFWwindow* window, double x, double y) {
	if(cursor.moving) {
		oldPos = cursor.position;
		glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
		translate.x += (cursor.position.x - oldPos.x) / 100;
		translate.y += (oldPos.y - cursor.position.y) / 100;
	}
}

#endif /* CALLBACKS_H */
