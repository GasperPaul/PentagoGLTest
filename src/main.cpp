#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#include "Button.h"
#include "Stone.h"
#include "callbacks.h"
#include "GLutils.h"
#include <GL/glut.h>
#include <string>

#include <windows.h>

typedef void (*DrawingFunction)();
DrawingFunction Redraw;
GLFWwindow* window;

int texture;

// for testing
int board[6][6];
int counter = 1;

static void stoneClick(GameObject* sender){
	int i = (((Stone*)sender)->GetCenter().x - 120)/80;
	int j = (((Stone*)sender)->GetCenter().y - 40)/80;
	if (!board[i][j]) {
		board[i][j] = counter%2 == 0 ? -counter : counter;
		counter++;
	}
}

void MainMenuDraw() {
	for (auto i : menuButtons)
	{
		i->Draw();
	}
}

void GameRender() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0); glVertex2d(-1.f,-1.f);
		glTexCoord2d(1.0,0.0); glVertex2d(1.f,-1.f);
		glTexCoord2d(1.0,1.0); glVertex2d(1.f,1.f);
		glTexCoord2d(0.0,1.0); glVertex2d(-1.f,1.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	for (auto i : objects)
	{
		int x = (((Stone*)i)->GetCenter().x - 120)/80;
		int y = (((Stone*)i)->GetCenter().y - 40)/80;

		((Stone*)i)->IsSet(board[x][y] > 0 ? Stone::Type::White : (board[x][y] < 0 ? Stone::Type::Black : Stone::Type::Empty));
		i->Draw();
	}
}

static void newGameBtnClick(GameObject* sender) {
	gameMode = PlayerStep;
}

static void exitBtnClick(GameObject* sender) {
	glfwSetWindowShouldClose(window, 1);
}

void InitializeInterface() {
	for (int i = 0; i<6; i++)
		for (int j = 0; j<6; j++) {
			Stone* stn = new Stone({ double(120+i*80), double(40+j*80) }, 30);
			*stn += stoneClick;
			objects.push_back(stn);
		}

	Button* menuBtn = new Button({ 150, 120 }, { 230, 150 });
	*menuBtn += newGameBtnClick;
	menuButtons.push_back(menuBtn);

	Button* exitBtn = new Button({ 150, 170 }, { 230, 200 });
	*exitBtn += exitBtnClick;
	menuButtons.push_back(exitBtn);

	gameMode = MainMenu;
}

void print(){

}

int main() {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) return -1;
	//glfwWindowHint(GLFW_RESIZABLE, false);
	window = glfwCreateWindow(640, 480, "Pentago", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetMouseButtonCallback(window, GameMouseCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

	InitializeInterface();

	texture = GLutils::LoadTexture("board.raw", 1024, 1024, true);

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glColor3f(1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		switch(gameMode){
		case PlayerStep:
		case OponentStep:
			Redraw = GameRender;
			break;
		case MainMenu:
			Redraw = MainMenuDraw;
			break;
		}

		Redraw();

		/* End render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	for(GameObject* obj : objects)
		delete obj;
	return 0;
}
