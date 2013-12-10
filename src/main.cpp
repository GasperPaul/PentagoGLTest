#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "Stone.h"
#include <iostream>
#include <fstream>
using namespace std;

Point3D translate = { 0, 0, 0 };
Point2D oldPos;
tcursor cursor;
std::vector<GameObject*> objects;

// for testing
int board[6][6];
int counter = 1;

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

static void mouseCallback(GLFWwindow* window, int button, int action, int mods){
	glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch(action) {
		case GLFW_PRESS:
			glfwGetCursorPos(window, &cursor.position.x, &cursor.position.y);
			for(GameObject* obj : objects)
				obj->Clicked(cursor.position);
			break;
		case GLFW_RELEASE:
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

void btn_click(GameObject* sender){
	int i = (((Stone*)sender)->GetCenter().x - 120)/80;
	int j = (((Stone*)sender)->GetCenter().y - 40)/80;
	board[i][j] = counter%2 == 0 ? -counter : counter;
	counter++;
}

void InitializeInterface() {
	for (int i = 0; i<6; i++)
		for (int j = 0; j<6; j++) {
			Stone* stn = new Stone({ double(120+i*80), double(40+j*80) }, 30);
			*stn += btn_click;
			objects.push_back(stn);
		}
}

GLuint LoadTexture(const char* filename, int width, int height, int wrap)
{
	ifstream file;
	file.open(filename, ios::binary);
	char* data = new char[width * height * 3];
	file.read(data, width * height * 3);
	file.close();

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete[] data;
	return texture;
}

int main() {
	glfwSetErrorCallback(errorCallback);

	GLFWwindow* window;
	if (!glfwInit()) return -1;
	window = glfwCreateWindow(640, 480, "Pentago", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

	InitializeInterface();

	int texture = LoadTexture("board.raw", 1024, 1024, true);

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

	    glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
		      glTexCoord2d(0.0,0.0); glVertex2d(-1.f,-1.f);
		      glTexCoord2d(1.0,0.0); glVertex2d(1.f,-1.f);
		      glTexCoord2d(1.0,1.0); glVertex2d(1.f,1.f);
		      glTexCoord2d(0.0,1.0); glVertex2d(-1.f,1.f);
       glEnd();
       glDisable(GL_TEXTURE_2D);

       for (auto i = objects.begin(); i != objects.end(); i++)
       {
    	   int x = (((Stone*)(*i))->GetCenter().x - 120)/80;
    	   int y = (((Stone*)(*i))->GetCenter().y - 40)/80;

    	   ((Stone*)(*i))->IsSet(board[x][y] > 0 ? Stone::Type::White
    			   	   	   : (board[x][y] < 0 ? Stone::Type::Black : Stone::Type::Empty));
    	   (*i)->Draw();
       }

		/* End render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	for(GameObject* obj : objects)
		delete obj;
	return 0;
}
