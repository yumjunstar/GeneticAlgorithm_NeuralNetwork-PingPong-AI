#include "PingPong.h"

// constructor 	

PingPong::~PingPong() {
	if (blades_left_player != nullptr) delete blades_left_player;
	if (ball != nullptr) delete ball;

	for (int i = 0; i < blades_right_ai.size(); i++) {
		if (blades_right_ai[i] != nullptr) delete blades_right_ai[i];
	}

}

PingPong::PingPong(bool LearnMode, int AmountBladeEach, int BladeSpeed, int BladeSize, int BallSpeed) {
	//GA���� ���� ������������ LearnMode����
	//BladeBothSide�� blade�� ���ʿ� �������� ���ʿ� �������� �����ϴ� �Ű�����
	//AmountBladeEach�� Blade�� ��� �������� (BladeBothSide�� false�϶��� �� �� �ֵ��� �ϱ�?)
	//this->ds = ds;
	this->BladeSize = BladeSize;
	this->LearnMode = LearnMode;
	this->terminate = false;//���� �Ǵ��� Ȯ��
	this->BladeCountEach = AmountBladeEach;//������ �ʱ��� Ź��ä ����
	this->BladeRemainCount = AmountBladeEach;//���� ��� ���� Ź��ä�� ����
	this->blades_left_player = nullptr;
	ball = new Ball(SIZE_OF_COLUMN_SCREEN / 2, SIZE_OF_ROW_SCREEN / 2);
	//Blade(int x, int y, int ID, int size, int speed)



	int blade_range_y_start = 0 + 1;
	int blade_range_y_end = SIZE_OF_ROW_SCREEN - 1 - 1;
	//LearnMode������ blade�� ���ʾ��� �߰��ȴ�.
	//������ blade�� �߰� LearnMode
	int right_blade_col = SIZE_OF_COLUMN_SCREEN - 1 - 1;
	int left_blade_col = 0 + 1;
	if (LearnMode) {//�н� ���
		for (int id = 0; id < AmountBladeEach; id++) {
			//Blade(int x, int y, int ID, int size);
			int pos = 1 + (id % (SIZE_OF_ROW_SCREEN - 1));

			Blade* temp = new Blade(right_blade_col, pos, id, BladeSize, BladeSpeed, blade_range_y_start, blade_range_y_end);
			blades_right_ai.push_back(temp);
		}
	}
	else {//�÷��� �����
		int id = 0;
		int pos_y = SIZE_OF_ROW_SCREEN / 2;
		blades_left_player = new Blade(left_blade_col, pos_y, id, BladeSize, BladeSpeed, blade_range_y_start, blade_range_y_end);
		blades_right_ai.push_back(new Blade(right_blade_col, pos_y, id, BladeSize, BladeSpeed, blade_range_y_start, blade_range_y_end));
	}





}

// Incrementing score

void PingPong::increment_score(Blade* player) {
	player->add_score(1);

	//������ �ø��� �ø���.
	if (LearnMode) {//�н� ����� ��� AI�� ���� �ø��� Ź��ä�� ĥ������ ���� ����
		player->add_score(1);
	}
	else {//�н� ��尡 �ƴϰ� 1:1 ����� ��� ���ΰ��� ���� �ø��� ���� �ε������
		assert(blades_right_ai.size() == 1);//���� ��忡�� AI�� ������ �׻� 1�̾�� �Ѵ�.

											//���� ���ϱ� ���� �Ѵ� ��ġ �ʱ�ȭ
		blades_left_player->blade_reset();
		blades_right_ai[0]->blade_reset();
		ball->reset_ball();
	}

}

// Drawing the board (at each moment -- this will explain the blips)

void PingPong::draw_game_layout() {
	ICON_NUMBER map_arr[SIZE_OF_ROW_SCREEN][SIZE_OF_COLUMN_SCREEN] = { BLANK_SYMBOL, };
	for (int j = 0; j < SIZE_OF_COLUMN_SCREEN; j++) {
		map_arr[0][j] = TOP_WALL_SYMBOL;
		map_arr[SIZE_OF_ROW_SCREEN - 1][j] = BUTTOM_WALL_SYMBOL;
	}
	for (int i = 0; i < SIZE_OF_ROW_SCREEN; i++) {
		map_arr[i][SIZE_OF_COLUMN_SCREEN - 1] = RIGHT_WALL_SYMBOL;
		map_arr[i][0] = LEFT_WALL_SYMBOL;
	}
	//blade�� ���� ������ �ҷ��ͼ� ��ġ�� ����
	pair<int, int> ball_cor = ball->get_ball_coordinate();
	int ball_x = ball_cor.first;
	int ball_y = ball_cor.second;
	map_arr[ball_y][ball_x] = BALL_SYMBOL;

	if (!LearnMode) {//���� ���� LEFT BLADE ��ġ
		pair<int, int> blade_left_player_cor = blades_left_player->get_blade_coordinate();
		int blade_left_x = blade_left_player_cor.first;
		int blade_left_y = blade_left_player_cor.second;
		map_arr[blade_left_y][blade_left_x] = LEFT_BLADE_SYMBOL;
	}


	for (int i = 0; i < blades_right_ai.size(); i++) {//AI�� BLADE
		pair<int, int> blade_right_ai_cor = blades_right_ai[i]->get_blade_coordinate();
		int blade_right_x = blade_right_ai_cor.first;
		int blade_right_y = blade_right_ai_cor.second;
		map_arr[blade_right_y][blade_right_x] = RIGHT_BLADE_SYMBOL;
	}
	ds.draw_layout(map_arr, SIZE_OF_ROW_SCREEN);
	//Sleep(1000);
}

// function to respond to player inputs

void PingPong::play() {
	ball->move_ball(); // to move the ball in direction specified by 'direction'

	if (_kbhit()) { // if any key is pressed : take action
		char key = _getch(); // get the pressed key character

		switch (key) {
		case player_up_key:
			blades_left_player->blade_move_up();
			break;
		case player_down_key:
			blades_left_player->blade_move_down();
			break;
		case 't':
			terminate = true;
			break;
		}

		// if it's new game move the ball in random directions				
		if (ball->get_ball_direction() == STOP) ball->randomize_ball_direction();
	}

}

// function to moniter ball position

void PingPong::monitor_ball() {
	// if ball hits bottom wall
	pair<int, int> ball_cor = ball->get_ball_coordinate();
	int ball_x = ball_cor.first;
	int ball_y = ball_cor.second;
	//�ؿ�
	if (ball_y == SIZE_OF_ROW_SCREEN - 1)
		ball->change_ball_direction(ball->get_ball_direction() == DOWNRIGHT ? UPRIGHT : UPLEFT);

	// if ball hits top wall
	if (ball_y == 0)
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

	// if ball hits right wall --> player_1 wins
	if (ball_x == SIZE_OF_COLUMN_SCREEN - 1)
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? UPLEFT : DOWNLEFT);
	//increment_score(player_1);

	// if ball hits left wall --> player_2 wins
	if (ball_x == 0)
		ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
	//increment_score(player_2);
}

void PingPong::lets_ping_pong() {
	if (LearnMode) {
		draw_game_layout();
		monitor_ball();
	}
	else {
		while (!terminate) {
			draw_game_layout();
			play();
			monitor_ball();
		}
	}
}
int PingPong::current_blade_count() 
{
	return this->BladeRemainCount;
}
void PingPong::get_all_blade_abs_coor(pair<int, int>& left_blade_player_coor, pair<int, int> all_right_blade_ai_coor[], int& RightBladeSize)
{
	RightBladeSize = this->BladeRemainCount;
	if (blades_left_player)	left_blade_player_coor = blades_left_player->get_blade_coordinate();
	for (int i = 0; i < this->BladeRemainCount; i++) {
		all_right_blade_ai_coor[i] = blades_right_ai[i]->get_blade_coordinate();
	}
}

pair<int, int> PingPong::GetBallCoor()
{
	return ball->get_ball_coordinate();
}

void PingPong::key_up(bool objects[], int size)
{
	assert(size == this->BladeRemainCount);
	for (int i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_up();
	}
}
void PingPong::key_down(bool objects[], int size) 
{
	assert(size == this->BladeRemainCount);
	for (int i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_down();
	}
}
