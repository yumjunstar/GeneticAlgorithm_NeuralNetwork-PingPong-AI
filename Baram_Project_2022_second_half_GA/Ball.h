#pragma once
#include "Common_Define.h"
class Ball {
public:
	static constexpr int BallMAXMoveRange_x = SIZE_OF_COL_SCREEN - 1;
	static constexpr int BallMAXMoveRange_y = SIZE_OF_ROW_SCREEN - 1;
	static constexpr int BallMINMoveRange_x = 0;
	static constexpr int BallMINMoveRange_y = 0;


	// constructor
	Ball(int x, int y, int speed);
	Ball_Direction get_ball_direction() const;

	void reset_ball();

	void change_ball_direction(Ball_Direction d);
	void change_ball_speed(int speed);
	void randomize_ball_direction();
	void randomize_ball_pos();
	Coor GetBallCoordinate() const;
	void SetBallDirection(Ball_Direction dir);

	void move_ball();

private:
	int x, y, center_x, center_y; // to hold postions (current and center)

	int speed;
	Ball_Direction direction; // to hold direction of motion of ball
};
