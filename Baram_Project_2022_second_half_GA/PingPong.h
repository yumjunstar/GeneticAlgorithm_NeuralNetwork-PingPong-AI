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
	//Blade ��ü ����
	Blade* blades_left_player;
	vector<Blade*> blades_right_ai;
	Ball* ball;
	DrawScreen ds;
	//PingPong���� ȭ�� �׸��� Ŭ����
	//DrawScreen* ds;
public:

	// constructor 	
	PingPong(bool LearnMode = true, int AmountBladeEach = 100, int BladeSpeed = 1, int BladeSize = 3, int BallSpeed = 2);
	~PingPong();
	//��� �ִ� Ź��ä ���� ��ȯ
	int current_blade_count();

	//Ź��ä���� ��ġ ��ȯ
	void get_all_blade_abs_coor(pair<int, int>& left_blade_player_coor, pair<int, int> all_right_blade_ai_coor[], int& RightBladeSize);
	//���� ���� ��ġ ��ȯ
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