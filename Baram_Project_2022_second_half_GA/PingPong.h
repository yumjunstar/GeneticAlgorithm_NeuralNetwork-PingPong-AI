#pragma once
#include "Common_Define.h"
#include "Blade.h"
#include "Ball.h"
#include "DrawScreen.h"

// Ping_Pong Class
class PingPong {
private:
	int BladeCountEach;
	//int BallCount;
	int BladeSize;
	int BladeRemainCount;
	bool terminate;
	bool LearnMode;
	static constexpr char player_up_key = 'w';
	static constexpr char player_down_key = 's';
	//Blade 객체 생성
	Blade* blades_left_player;
	vector<Blade*> blades_right_ai;
	Ball* ball;
	DrawScreen ds;
	//PingPong에서 화면 그리는 클래스
	//DrawScreen* ds;
public:

	// constructor 	
	PingPong(bool LearnMode = true, int AmountBladeEach = 100, int BladeSpeed = 1, int BladeSize = 3, int BallSpeed = 2);
	~PingPong();
	//살아 있는 탁구채 개수 반환
	int current_blade_count();

	//탁구채들의 위치 반환
	void get_all_blade_abs_coor(pair<int, int>& left_blade_player_coor, pair<int, int> all_right_blade_ai_coor[], int& RightBladeSize);
	//현재 공의 위치 반환
	pair<int, int> GetBallCoor();
	void key_up(bool objects[], int size);
	void key_down(bool objects[], int size);

	// Incrementing score
	void increment_score(Blade* player);

	// Drawing the board (at each moment -- this will explain the blips)
	void draw_game_layout(); // end of Draw function

	// function to respond to player inputs
	void play(); // end of input

	// function to moniter ball position
	void monitor_ball();
	void lets_ping_pong();
};