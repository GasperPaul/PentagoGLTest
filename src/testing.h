/*
 *  Created on: 15 груд. 2013
 *      Author: Gasper
 */

#ifndef TESTING_H
#define TESTING_H

static int board[6][6];
static int counter = 1;

enum RotateDirection { Left, Right };

static void Rotate(short quadrant, RotateDirection direction) {
	unsigned i = quadrant == 0 || quadrant == 1 ? 0 : 3;
	unsigned j = quadrant == 0 || quadrant == 3 ? 3 : 0;
	short tmp;

	switch (direction) {
	case Left:
		tmp = board[i][j];
		board[i][j] = board[i][j + 2];
		board[i][j + 2] = board[i + 2][j + 2];
		board[i + 2][j + 2] = board[i + 2][j];
		board[i + 2][j] = tmp;
		tmp = board[i][j + 1];
		board[i][j + 1] = board[i + 1][j + 2];
		board[i + 1][j + 2] = board[i + 2][j + 1];
		board[i + 2][j + 1] = board[i + 1][j];
		board[i + 1][j] = tmp;
		break;
	case Right:
		tmp = board[i][j];
		board[i][j] = board[i + 2][j];
		board[i + 2][j] = board[i + 2][j + 2];
		board[i + 2][j + 2] = board[i][j + 2];
		board[i][j + 2] = tmp;
		tmp = board[i][j + 1];
		board[i][j + 1] = board[i + 1][j];
		board[i + 1][j] = board[i + 2][j + 1];
		board[i + 2][j + 1] = board[i + 1][j + 2];
		board[i + 1][j + 2] = tmp;
		break;
	}
}

#endif /* TESTING_H */
