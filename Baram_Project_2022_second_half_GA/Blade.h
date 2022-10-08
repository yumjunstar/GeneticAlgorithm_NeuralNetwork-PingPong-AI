#pragma once
#include "Common_Define.h"

// Blade Class
class Blade {
private:
	int x, y, initial_x, initial_y; // to hold position of blades (current and default)
	static constexpr size_t DefaultBladeSize = 1;
	static constexpr size_t DefaultBladeSpeed = 1;

	int blade_size;
	int id;
	int speed;
	int score;
	int MoveRangeStart_y;
	int MoveRangeEnd_y;
public:
	// constructor
	Blade(int x, int y, int ID, int BladeMoveRangeStart_y, int BladeMoveRangeEnd_y,
	int size = DefaultBladeSize, int speed = DefaultBladeSpeed);
	// to reset blade positions while Ping_Pong restarting
	void blade_reset();

	// to move balde up
	void blade_move_up();
	void blade_move_down();
	bool is_hit_blade(int check_x, int check_y);
	// to move balde down

	void add_score(int score);
	void set_score(int score);
	int get_score();
	size_t get_blade_size();
	Coor GetBladeCoordinate();
}; // end of Blade class