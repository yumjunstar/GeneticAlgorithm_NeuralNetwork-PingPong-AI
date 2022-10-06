#pragma once
#include "Common_Define.h"

// Blade Class
class Blade {
private:
	int x, y, initial_x, initial_y; // to hold position of blades (current and default)
	int blade_size;
	int id;
	int speed;
	int score;
	int start_y;
	int end_y;
public:
	// constructor
	Blade(int x, int y, int ID, int size, int speed, int start_y, int end_y) {
		this->start_y = 0;
		this->end_y = 0;
		this->initial_x = x;
		this->initial_y = y;
		this->x = x;
		this->y = y;
		this->blade_size = size;
		this->id = ID;
		this->speed = speed;
		this->score = 0;
		this->start_y = start_y;
		this->end_y = end_y;
	}
	// to reset blade positions while Ping_Pong restarting
	void blade_reset() {
		x = initial_x;
		y = initial_y;
	}

	// to move balde up
	void blade_move_up() {//일정 한도부터는 못가게 또는 뱅글 뱅글 돌게 해도 괜찮을 것 같은데
		int sum = y-this->speed;
		if (sum >= start_y) y = sum;
	}

	// to move balde down
	void blade_move_down() {
		int sum = y + this->speed;
		if (sum <= end_y) y = sum;
	}
	void add_score(int score) {
		this->score += score;
	}
	void set_score(int score) {
		this->score = score;
	}
	int get_score() {
		return this->score;
	}
	pair <int, int> get_blade_coordinate() {
		return make_pair(x, y);
	}
}; // end of Blade class