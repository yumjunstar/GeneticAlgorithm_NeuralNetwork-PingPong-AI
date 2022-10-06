#pragma once
#include "Common_Define.h"
class Ball {
private:
	int x, y, center_x, center_y; // to hold postions (current and center)
	int speed;
	Ball_Direction direction; // to hold direction of motion of ball
public:
	// constructor
	Ball(int x, int y) { // initial position of the ball
		this->center_x = x; // center_X and center_y store the initial value
		this->center_y = y;
		this->x = x; // x and y store current value of the ball position
		this->y = y;
		this->speed = 1;
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
	pair<int, int> get_ball_coordinate() const {
		return make_pair(x, y);
	}


	// function to bring the ball back to its starting position (center)

	// function to move ball around the screen
	void move_ball() { // x increases along right
		switch (direction) { // y increases along bottom
		case STOP:
			break;
		case LEFT:
			x -= speed;

			break;
		case RIGHT:
			x += speed;

			break;
		case UPLEFT:
			x -= speed;
			y -= speed;
			break;
		case DOWNLEFT:
			x -= speed;
			y += speed;
			break;
		case UPRIGHT:
			x += speed;
			y -= speed;
			break;
		case DOWNRIGHT:
			x += speed;
			y += speed;
			break;
		default: // in case direction take execptional value, ignore it.
			break;
		}
		assert(x >= 0);
		assert(x < SIZE_OF_COLUMN_SCREEN);
		assert(y >= 0);
		assert(y < SIZE_OF_ROW_SCREEN);
	}
}; // end of Ball class
