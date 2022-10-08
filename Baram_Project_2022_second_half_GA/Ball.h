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
	Ball(int x, int y, int speed);
	Ball_Direction get_ball_direction() const;
	// 공을 원점으로 보내는 함수
	void reset_ball();


	// 공의 방향을 바꾸는 함수
	void change_ball_direction(Ball_Direction d);
	void change_ball_speed(int speed);
	// function to initialize the direction of ball randomely
	void randomize_ball_direction();

	// function to move ball around the screen
	Coor GetBallCoordinate() const;


	// function to bring the ball back to its starting position (center)

	// function to move ball around the screen
	void move_ball();
}; // end of Ball class
