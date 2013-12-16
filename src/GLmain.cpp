#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Button.h"
#include "Stone.h"
#include "Controls.h"
#include "GLutils.h"

// for testing
#include "testing.h"

typedef void (*DrawingFunction)();
DrawingFunction Redraw;

static GLFWwindow* window;
static std::vector<int> texture;

static void MainMenuDraw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0); glVertex2d(-2.f,-2.f);
		glTexCoord2d(1.0,0.0); glVertex2d(2.f,-2.f);
		glTexCoord2d(1.0,1.0); glVertex2d(2.f,2.f);
		glTexCoord2d(0.0,1.0); glVertex2d(-2.f,2.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	for (auto i : menuButtons)
		i->Draw();
}

static void GameRender() {
	glEnable(GL_TEXTURE_2D);
	// background
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0); glVertex2d(-2.f,-2.f);
		glTexCoord2d(1.0,0.0); glVertex2d(2.f,-2.f);
		glTexCoord2d(1.0,1.0); glVertex2d(2.f,2.f);
		glTexCoord2d(0.0,1.0); glVertex2d(-2.f,2.f);
	glEnd();
	// board
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0); glVertex2d(-1.f,-1.f);
		glTexCoord2d(1.0,0.0); glVertex2d(1.f,-1.f);
		glTexCoord2d(1.0,1.0); glVertex2d(1.f,1.f);
		glTexCoord2d(0.0,1.0); glVertex2d(-1.f,1.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	for (auto i : stones)
	{
		int x = (static_cast<Stone*>(i)->GetCenter().x - 120)/80;
		int y = (static_cast<Stone*>(i)->GetCenter().y - 40)/80;

		static_cast<Stone*>(i)->IsSet(board[x][y] > 0 ? Stone::Type::White
				: (board[x][y] < 0 ? Stone::Type::Black : Stone::Type::Empty));
		i->Draw();
	}

	if (gameMode == Rotation)
		for (auto btn : rotationButtons)
			btn->Draw();
}

static void stoneClick(GameObject* sender){
	int i = (static_cast<Stone*>(sender)->GetCenter().x - 120)/80;
	int j = (static_cast<Stone*>(sender)->GetCenter().y - 40)/80;
	if (!board[i][j] && gameMode == PlayerStep) {
		board[i][j] = counter%2 == 0 ? -counter : counter;
		counter++;
		gameMode = Rotation;
	}
}

static void rotateClick(GameObject* sender) {
	Point2D t = static_cast<Button*>(sender)->GetVerteces()[0];

	short quadrant = t.x == 0 ? (t.y < 100 ? 1 : 0) : (t.y < 100 ? 2 : 3);
	RotateDirection direction = t.y == 0 || t.y == 370
								? ((quadrant == 2 || quadrant == 3) ? RotateDirection::Right : RotateDirection::Left)
								: ((quadrant == 2 || quadrant == 3) ? RotateDirection::Left : RotateDirection::Right);
	Rotate(quadrant, direction);

	gameMode = PlayerStep;
}

static void InitializeInterface() {
	// loading textures
	texture.push_back(GLutils::LoadTexture("assets/background.raw", 1024, 1024, true));
	texture.push_back(GLutils::LoadTexture("assets/board.raw", 1024, 1024, true));
	texture.push_back(GLutils::LoadTexture("assets/L1.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/R1.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/L2.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/R2.raw", 256, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/new.raw", 1024, 256, true));
	texture.push_back(GLutils::LoadTexture("assets/exit.raw", 1024, 256, true));

	// stones
	for (int i = 0; i<6; i++)
		for (int j = 0; j<6; j++) {
			Stone* stn = new Stone({ double(120+i*80), double(40+j*80) }, 30);
			*stn += stoneClick;
			stones.push_back(stn);
		}

	// main menu
	Button* menuBtn[] = {
			new Button({ 200, 120 }, { 456, 184 }, texture[6]),
			new Button({ 200, 200 }, { 456, 264 }, texture[7])
	};
	*menuBtn[0] += [](GameObject*) { gameMode = PlayerStep; }; // NewGame;
	*menuBtn[1] += [](GameObject*) { glfwSetWindowShouldClose(window, 1); }; // Exit;
	for(auto i : menuBtn)
		menuButtons.push_back(i);

	// rotation buttons
	Button* rotBtn[] = {
		new Button({ 0, 0 }, { 50, 50 }, texture[3]),
		new Button({ 0, 60 }, { 50, 110 }, texture[2]),
		new Button({ 0, 370 }, { 50, 420 }, texture[3]),
		new Button({ 0, 430 }, { 50, 480 }, texture[2]),
		new Button({ 590, 0 }, { 640, 50 }, texture[4]),
		new Button({ 590, 60 }, { 640, 110 }, texture[5]),
		new Button({ 590, 370 }, { 640, 420 }, texture[4]),
		new Button({ 590, 430 }, { 640, 480 }, texture[5])
	};

	for(auto i : rotBtn) {
		*i += rotateClick;
		rotationButtons.push_back(i);
	}

	gameMode = MainMenu;
}

int main() {
	glfwSetErrorCallback(Controls::errorCallback);

	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_RESIZABLE, false);
	window = glfwCreateWindow(640, 480, "Pentago", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, Controls::keyboardCallback);
	glfwSetMouseButtonCallback(window, Controls::GameMouseCallback);
	glfwSetCursorPosCallback(window, Controls::mouseMoveCallback);

	InitializeInterface();

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
		case Rotation:
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

	for(GameObject* obj : stones) delete obj;
	for(GameObject* obj : menuButtons) delete obj;
	for(GameObject* obj : rotationButtons) delete obj;
	return 0;
}
