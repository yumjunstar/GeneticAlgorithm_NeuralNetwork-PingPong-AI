#include "PingPong.h"

// constructor 	

PingPong::~PingPong() {
	if (blades_left_player) delete blades_left_player;
	if (ball) delete ball;

	for (int i = 0; i < blades_right_ai.size(); i++) {
		if (blades_right_ai[i] != nullptr) delete blades_right_ai[i];
	}

}

void PingPong::Reset()
{
	GameTries = 0;
	LearnMode_MaxAIScore = 0;

	for (int i = 0; i < RightAIBladeRemainCount; i++) {
		blades_right_ai[i]->set_score(0);
	}
}

PingPong::PingPong(DrawScreen* ds, bool LearnMode, size_t AI_Blade_Count,
	size_t Current_Generation, size_t BladeSpeed, size_t BladeSize, size_t BallSpeed)
{
	//GA���� ���� ������������ LearnMode����
	//BladeBothSide�� blade�� ���ʿ� �������� ���ʿ� �������� �����ϴ� �Ű�����
	//AmountBladeEach�� Blade�� ��� �������� (BladeBothSide�� false�϶��� �� �� �ֵ��� �ϱ�?)
	this->ds_p = ds;
	this->LearnMode = LearnMode;
	assert(BladeSize <= SIZE_OF_ROW_SCREEN);
	this->BladeSize = BladeSize;
	//�����Ϸ��� �� ������ true�� �Ǿ�� �Ѵ�.
	this->terminate = false;

	this->blades_left_player = nullptr;
	this->LearnMode_MaxAIScore = 0;
	this->GameTries = 0;
	if (LearnMode)
	{
		this->LearnMode_CurrentGeneration = Current_Generation;

		this->CompeteMode_LeftPlayerScore = -1;
		this->CompeteMode_RightAIScore = -1;

		this->LearnMode_MaxAIScore = 0;

		//AI Blade�� ������ ȭ�� ũ�⺸�� ũ�� �ȵǹǷ� �̷��� �����Ѵ�.
		this->RightAIBladeRemainCount = AI_Blade_Count;
		this->LeftPlayerBladeRemainCount = 0;
	}
	else
	{
		this->LearnMode_CurrentGeneration = -1;

		this->CompeteMode_LeftPlayerScore = 0;
		this->CompeteMode_RightAIScore = 0;

		this->LearnMode_MaxAIScore = -1;

		this->LeftPlayerBladeRemainCount = 1;
		this->RightAIBladeRemainCount = 1;

	}





	ball = new Ball(SIZE_OF_COL_SCREEN / 2, SIZE_OF_ROW_SCREEN / 2, BallSpeed);



	int blade_range_y_start = 0 + 1;
	int blade_range_y_end = SIZE_OF_ROW_SCREEN - 1 - 1;
	//LearnMode������ blade�� ���ʾ��� �߰��ȴ�.
	//������ blade�� �߰� LearnMode
	int right_blade_col_x = SIZE_OF_COL_SCREEN - 1 - 1;
	int left_blade_col = 0 + 1;
	if (LearnMode) {//�н� ���
		for (int id = 0; id < RightAIBladeRemainCount; id++) {
			//Blade(int x, int y, int ID, int size);
			int pos_y = 1 + (id % (SIZE_OF_ROW_SCREEN - 1));
			Blade* temp = new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
			blades_right_ai.push_back(temp);
		}
	}
	else {//���� �����
		int id = 0;
		int pos_y = SIZE_OF_ROW_SCREEN / 2;
		blades_left_player = new Blade(left_blade_col, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
		blades_right_ai.push_back(new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed));
	}





}

// Incrementing score

void PingPong::increment_score(Blade* player) {
	//���� ��忡���� ���
	//���� Ź��ä���� ��ġ�� �ʱ�ȭ �ϰ� ������ �ø��� �Լ�
	assert(!LearnMode);
	player->add_score(1);
	ball->reset_ball();


	assert(RightAIBladeRemainCount == 1);
	blades_right_ai[0]->blade_reset();

	blades_left_player->blade_reset();
}

// Drawing the board (at each moment -- this will explain the blips)

void PingPong::draw_game_layout() {
	ICON_NUMBER map_arr[SIZE_OF_ROW_SCREEN][SIZE_OF_COL_SCREEN] = { BLANK_SYMBOL, };
	for (int j = 0; j < SIZE_OF_COL_SCREEN; j++) {
		map_arr[0][j] = TOP_WALL_SYMBOL;
		map_arr[SIZE_OF_ROW_SCREEN - 1][j] = BUTTOM_WALL_SYMBOL;
	}
	for (int i = 0; i < SIZE_OF_ROW_SCREEN; i++) {
		map_arr[i][SIZE_OF_COL_SCREEN - 1] = RIGHT_WALL_SYMBOL;
		map_arr[i][0] = LEFT_WALL_SYMBOL;
	}
	//blade�� ���� ������ �ҷ��ͼ� ��ġ�� ����
	Coor ball_cor = ball->GetBallCoordinate();
	map_arr[ball_cor.y][ball_cor.x] = BALL_SYMBOL;

	if (!LearnMode) {//���� ���� LEFT BLADE ��ġ
		Coor blade_left_player_cor = blades_left_player->GetBladeCoordinate();
		map_arr[blade_left_player_cor.y][blade_left_player_cor.x] = LEFT_BLADE_SYMBOL;
	}


	for (int i = 0; i < blades_right_ai.size(); i++) {//AI�� BLADE
		Coor blade_right_ai_cor = blades_right_ai[i]->GetBladeCoordinate();
		map_arr[blade_right_ai_cor.y][blade_right_ai_cor.x] = RIGHT_BLADE_SYMBOL;
	}
	if (LearnMode) 
	{
		ds_p->update_info(this->LearnMode_MaxAIScore, -1, this->LearnMode_CurrentGeneration);
	}
	else
	{
		ds_p->update_info(blades_right_ai[0]->get_score(), blades_left_player->get_score());
	}

	ds_p->draw_layout(map_arr, SIZE_OF_ROW_SCREEN);
}

// function to respond to player inputs

void PingPong::play() {
	ball->move_ball(); // to move the ball in direction specified by 'direction'
	if (LearnMode) 
	{
		
		if (_kbhit()) { // if any key is pressed : take action
			char key = _getch(); // get the pressed key character
			if (key == 't') terminate = true;
			if (ball->get_ball_direction() == STOP) ball->randomize_ball_direction();
		}
	}
	else
	{
		if (_kbhit()) { // if any key is pressed : take action
			char key = _getch(); // get the pressed key character

			switch (key) {
			case PLAYER_UP_KEY:
				blades_left_player->blade_move_up();
				break;
			case PLAYER_DOWN_KEY:
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


}

// function to moniter ball position

void PingPong::monitor_ball() {
	// if ball hits bottom wall
	Coor ball_cor = ball->GetBallCoordinate();
	int ball_x = ball_cor.x;
	int ball_y = ball_cor.y;

	int BottomWall_Coor_y = SIZE_OF_ROW_SCREEN - 1;
	int TopWall_Coor_y = 0;
	int RightWall_Coor_x = SIZE_OF_COL_SCREEN - 1;
	int LeftWall_Coor_x = 0;//MIN_X_CAN_GO;
	//�ؿ�
	if (ball_y == BottomWall_Coor_y)
		ball->change_ball_direction(ball->get_ball_direction() == DOWNRIGHT ? UPRIGHT : UPLEFT);

	// if ball hits top wall
	if (ball_y == TopWall_Coor_y)
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

	// if ball hits right wall --> player wins
	if (ball_x == RightWall_Coor_x)
	{
		GameTries++;
		//�н� ���
		if (LearnMode) 
		{
			//�н� ��忡�� ���� ���� ���� ������ ���� ���� ������� ���� �ٽ� ����
			//�� �� ��ġ, Ź��ä ����ġ, GameTries ����
			ball->reset_ball();
			for (int i = 0; i < RightAIBladeRemainCount; i++) blades_right_ai[i]->blade_reset();

		}
		//���� ���
		else 
		{
			//���� ����϶� AI�� ���� ġ�� ���ϰ� ������ ���� ������ �����(player) ���� �ø��� �ʱ�ȭ
			increment_score(blades_left_player);
		}
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? UPLEFT : DOWNLEFT);
	}
	//increment_score(player_1);

	// if ball hits left wall --> ai wins
	if (ball_x == LeftWall_Coor_x)
	{

		if (!LearnMode)
		{
			GameTries++;
			//���� ����϶��� BLADE ������ �ݵ�� �Ѱ����� �Ѵ�.
			assert(RightAIBladeRemainCount == 1);

			//���� ����϶� �÷��̾ ���� ���� ���� ���� ���� ������� �����(ai) ������ �ø��� ��ġ �ʱ�ȭ
			increment_score(blades_right_ai[0]);
		}
		ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
	}

	//���� Ź��ä�� �ε�ģ ��쿡 ���ؼ� �����Ѵ�.

	for (int i = 0; i < RightAIBladeRemainCount; i++)
	{
		//AI Blade�� ���� �΋H������
		if (blades_right_ai[i]->is_hit_blade(ball_x, ball_y))
		{
			if (LearnMode)
			{
				blades_right_ai[i]->add_score(1);
				size_t ai_score = blades_right_ai[i]->get_score();
				if (ai_score > LearnMode_MaxAIScore) LearnMode_MaxAIScore = ai_score;
			}
			ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? UPLEFT : DOWNLEFT);

		}
	}

	//Player Blade�� ���� �΋H������
	//���� �Ǿ� ��������
	if (blades_left_player)
	{
		if (blades_left_player->is_hit_blade(ball_x, ball_y))
		{
			ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
		}
	}
}

void PingPong::lets_ping_pong() {
	//�ӵ� ������ while���� ������.
	while (!terminate) {
		draw_game_layout();
		play();
		monitor_ball();
		Sleep(DELAY_PER_FRAME);
	}
}
void PingPong::lets_ping_pong(size_t repeat) 
{
	while (repeat > this->GameTries)
	{
		draw_game_layout();
		play();
		monitor_ball();
		Sleep(DELAY_PER_FRAME);
	}
}
size_t PingPong::CurrentBladeCount()
{
	return this->RightAIBladeRemainCount;
}
Coor PingPong::GetLeftPlayerBladeCoor()
{
	assert(!LearnMode);
	Coor loc;
	if (blades_left_player)
	{
		loc = blades_left_player->GetBladeCoordinate();
	}
	return loc;

}
vector<Coor> PingPong::GetAllRightAIBladeCoor()
{
	vector <Coor> temp;
	for (int i = 0; i < this->RightAIBladeRemainCount; i++) {
		temp.push_back(blades_right_ai[i]->GetBladeCoordinate());
	}
	return temp;
}

//��� Ź��ä���� ���� ��ȯ

vector<size_t> PingPong::GetAllBladesScores()
{
	vector <size_t> temp;
	for (int i = 0; i < this->RightAIBladeRemainCount; i++) {
		temp.push_back(blades_right_ai[i]->get_score());
	}
	return temp;
}
//void PingPong::GetAllBladesCoor(pair<int, int>& left_blade_player_coor, pair<int, int> all_right_blade_ai_coor[], int& RightBladeSize)
//{
//	RightBladeSize = this->RightAIBladeRemainCount;
//	if (blades_left_player)	left_blade_player_coor = blades_left_player->get_blade_coordinate();
//	for (int i = 0; i < this->RightAIBladeRemainCount; i++) {
//		all_right_blade_ai_coor[i] = blades_right_ai[i]->get_blade_coordinate();
//	}
//}

size_t PingPong::GetMaxScoreForLearn()
{
	return LearnMode_MaxAIScore;
}

Coor PingPong::GetBallCoor()
{
	return ball->GetBallCoordinate();
}

Ball_Direction PingPong::GetBallDirection()
{
	return ball->get_ball_direction();
}

size_t PingPong::GetGameTries()
{
	return GameTries;
}

void PingPong::SetGameTries(size_t value)
{
	this->GameTries = value;
}

void PingPong::SetGeneration(size_t gen)
{
	this->LearnMode_CurrentGeneration = gen;
}

void PingPong::key_up(bool objects[], int size)
{
	assert(size == this->RightAIBladeRemainCount);
	for (int i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_up();
	}
}
void PingPong::key_down(bool objects[], int size) 
{
	assert(size == this->RightAIBladeRemainCount);
	for (int i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_down();
	}
}
