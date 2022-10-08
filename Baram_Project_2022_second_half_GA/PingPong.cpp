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
	//GA에서 값을 가져오기위해 LearnMode만듬
	//BladeBothSide는 blade를 양쪽에 놓을껀지 한쪽에 놓을건지 결정하는 매개변수
	//AmountBladeEach는 Blade를 몇개씩 놓을껀지 (BladeBothSide가 false일때만 쓸 수 있도록 하기?)
	this->ds_p = ds;
	this->LearnMode = LearnMode;
	assert(BladeSize <= SIZE_OF_ROW_SCREEN);
	this->BladeSize = BladeSize;
	//종료하려면 이 변수가 true가 되어야 한다.
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

		//AI Blade의 개수가 화면 크기보다 크면 안되므로 이렇게 설정한다.
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
	//LearnMode에서는 blade가 한쪽씩만 추가된다.
	//오른쪽 blade만 추가 LearnMode
	int right_blade_col_x = SIZE_OF_COL_SCREEN - 1 - 1;
	int left_blade_col = 0 + 1;
	if (LearnMode) {//학습 모드
		for (int id = 0; id < RightAIBladeRemainCount; id++) {
			//Blade(int x, int y, int ID, int size);
			int pos_y = 1 + (id % (SIZE_OF_ROW_SCREEN - 1));
			Blade* temp = new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
			blades_right_ai.push_back(temp);
		}
	}
	else {//경쟁 모드라면
		int id = 0;
		int pos_y = SIZE_OF_ROW_SCREEN / 2;
		blades_left_player = new Blade(left_blade_col, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
		blades_right_ai.push_back(new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed));
	}





}

// Incrementing score

void PingPong::increment_score(Blade* player) {
	//경쟁 모드에서만 사용
	//공과 탁구채들의 위치를 초기화 하고 점수를 올리는 함수
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
	//blade와 공의 정보를 불러와서 위치에 놓기
	Coor ball_cor = ball->GetBallCoordinate();
	map_arr[ball_cor.y][ball_cor.x] = BALL_SYMBOL;

	if (!LearnMode) {//경쟁 모드면 LEFT BLADE 설치
		Coor blade_left_player_cor = blades_left_player->GetBladeCoordinate();
		map_arr[blade_left_player_cor.y][blade_left_player_cor.x] = LEFT_BLADE_SYMBOL;
	}


	for (int i = 0; i < blades_right_ai.size(); i++) {//AI의 BLADE
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
	//밑에
	if (ball_y == BottomWall_Coor_y)
		ball->change_ball_direction(ball->get_ball_direction() == DOWNRIGHT ? UPRIGHT : UPLEFT);

	// if ball hits top wall
	if (ball_y == TopWall_Coor_y)
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

	// if ball hits right wall --> player wins
	if (ball_x == RightWall_Coor_x)
	{
		GameTries++;
		//학습 모드
		if (LearnMode) 
		{
			//학습 모드에서 공을 막지 못해 오른쪽 벽에 공이 닿았을때 게임 다시 시작
			//공 원 위치, 탁구채 원위치, GameTries 증가
			ball->reset_ball();
			for (int i = 0; i < RightAIBladeRemainCount; i++) blades_right_ai[i]->blade_reset();

		}
		//경쟁 모드
		else 
		{
			//경쟁 모드일때 AI가 공을 치지 못하고 오른쪽 벽에 닿으면 상대편(player) 점수 올리고 초기화
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
			//경쟁 모드일때는 BLADE 갯수가 반드시 한개여야 한다.
			assert(RightAIBladeRemainCount == 1);

			//경쟁 모드일때 플레이어가 공을 막지 못해 왼쪽 벽에 닿았을때 상대편(ai) 점수를 올리고 위치 초기화
			increment_score(blades_right_ai[0]);
		}
		ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
	}

	//공이 탁구채에 부딪친 경우에 대해서 설정한다.

	for (int i = 0; i < RightAIBladeRemainCount; i++)
	{
		//AI Blade와 공이 부딫쳤을때
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

	//Player Blade와 공이 부딫쳤을때
	//정의 되어 있을때만
	if (blades_left_player)
	{
		if (blades_left_player->is_hit_blade(ball_x, ball_y))
		{
			ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
		}
	}
}

void PingPong::lets_ping_pong() {
	//속도 때문에 while문을 나눴다.
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

//모든 탁구채들의 점수 반환

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
