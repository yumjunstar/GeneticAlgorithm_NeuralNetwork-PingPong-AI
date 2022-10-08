#pragma once
#include "Common_Define.h"
class Ball {
private:
	int x, y, center_x, center_y; // to hold postions (current and center)
	static constexpr int BallMAXMoveRange_x = SIZE_OF_COL_SCREEN - 1;
	static constexpr int BallMAXMoveRange_y = SIZE_OF_ROW_SCREEN - 1;
	static constexpr int BallMINMoveRange_x = 0;//MIN_X_CAN_GO;
	static constexpr int BallMINMoveRange_y = 0;//MIN_Y_CAN_GO;
	int speed;
	Ball_Direction direction; // to hold direction of motion of ball
public:
	// constructor
	Ball(int x, int y, int speed) { // initial position of the ball
		this->center_x = x; // center_X and center_y store the initial value
		this->center_y = y;
		this->x = x; // x and y store current value of the ball position
		this->y = y;
		this->speed = speed;
		this->direction = STOP; // ball is initialized static
	}
	Ball_Direction get_ball_direction() const{
		return direction;
	}
	// 공을 원점으로 보내는 함수
	void reset_ball() {
		x = center_x;
		y = center_y;
		direction = STOP; // to stop the motion of ball
	}


	// 공의 방향을 바꾸는 함수
	void change_ball_direction(Ball_Direction d) {
		direction = d;
	}
	void change_ball_speed(int speed) {
		this->speed = speed;
	}
	// function to initialize the direction of ball randomely
	void randomize_ball_direction() {
		direction = (Ball_Direction)((rand() % 6) + 1);
	}

	// function to move ball around the screen
	Coor GetBallCoordinate() const {
		return Coor({ x, y });
	}


	// function to bring the ball back to its starting position (center)

	// function to move ball around the screen
	void move_ball() { // x increases along right
		int sum_x = 0;
		int sum_y = 0;
		if (y <= 2)
		{
			int a;
		}
		switch (direction) { // y increases along bottom
		case STOP:
			break;
		case LEFT:
			sum_x = x - this->speed;

			if (sum_x >= BallMINMoveRange_x) x = sum_x;
			else x = BallMINMoveRange_x;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		case RIGHT:
			sum_x = x + this->speed;

			if (sum_x <= BallMAXMoveRange_x) x = sum_x;
			else x = BallMAXMoveRange_x;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		case UPLEFT:
			sum_x = x - this->speed;
			sum_y = y - this->speed;

			if (sum_x >= BallMINMoveRange_x) x = sum_x;
			else x = BallMINMoveRange_x;

			if (sum_y >= BallMINMoveRange_y) y = sum_y;
			else y = BallMINMoveRange_y;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		case DOWNLEFT:
			sum_x = x - this->speed;
			sum_y = y + this->speed;

			if (sum_x >= BallMINMoveRange_x) x = sum_x;
			else x = BallMINMoveRange_x;

			if (sum_y <= BallMAXMoveRange_y) y = sum_y;
			else y = BallMAXMoveRange_y;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		case UPRIGHT:
			sum_x = x + this->speed;
			sum_y = y - this->speed;

			if (sum_x <= BallMAXMoveRange_x) x = sum_x;
			else x = BallMAXMoveRange_x;

			if (sum_y >= BallMINMoveRange_y) y = sum_y;
			else y = BallMINMoveRange_y;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		case DOWNRIGHT:
			sum_x = x + this->speed;
			sum_y = y + this->speed;

			if (sum_x <= BallMAXMoveRange_x) x = sum_x;
			else x = BallMAXMoveRange_x;

			if (sum_y <= BallMAXMoveRange_y) y = sum_y;
			else y = BallMAXMoveRange_y;
			assert(x >= 0);
			assert(x < SIZE_OF_COL_SCREEN);
			assert(y >= 0);
			assert(y < SIZE_OF_ROW_SCREEN);
			break;
		default: // in case direction take execptional value, ignore it.
			break;
		}
	}
}; // end of Ball class
