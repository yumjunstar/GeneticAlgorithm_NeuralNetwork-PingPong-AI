#include "PingPong.h"

// constructor 	

PingPong::~PingPong() {
	delete blades_left_player;
	delete ball;

	for (size_t i = 0; i < blades_right_ai.size(); i++) {
		delete blades_right_ai[i];
	}

}

void PingPong::SetDrawScreen_Pointer(DrawScreen* ds)
{
	ds_p = ds;
}

void PingPong::SetFileManage_Pointer(FileManage* fm)
{
	fm_p = fm;
	InitAI();
}

void PingPong::Reset()
{
	GameTries = 0;
	ClearAllBladesScore();
}
// AI의 가중치를 불러와서 설정
void PingPong::InitAI()
{
	OneDNNWeights_Include_Info info_of_dnn = fm_p->Read_OneDNNWeights_FromFile();


	ai_nn = new NeuralNetwork;

	ai_nn->make_neural_network(info_of_dnn.NeuralShape);
	ai_nn->set_weight(info_of_dnn.weights, info_of_dnn.weights_count);
}

void PingPong::LearnMode_WhenBallHitAIBlade(int blade_index)
{
	this->blades_right_ai[blade_index]->add_score(1);
	size_t ai_score = this->blades_right_ai[blade_index]->GetCountinusMAXScore();
	if (ai_score > this->LearnMode_MaxAIScore) this->LearnMode_MaxAIScore = ai_score;

	//(Ball_Direction)((rand() % 6) + 1);
	////enum Ball_Direction { STOP, LEFT, UPLEFT, DOWNLEFT, RIGHT, UPRIGHT, DOWNRIGHT };
	if (RandomBallDirection_WhenHitBlade) ball->SetBallDirection((Ball_Direction)((rand()%3) + 1));
	else ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? UPLEFT : DOWNLEFT);
}

void PingPong::LearnMode_WhenBallHitRightWall()
{
	ball->reset_ball();
	if (RandomBallPos_WhenRespawn) ball->randomize_ball_pos();
	if (RandomBallDirection_WhenRespawn) ball->randomize_ball_direction();
	else ball->change_ball_direction(DefaultBallDirection_WhenRespawn);


	for (size_t i = 0; i < RightAIBladeRemainCount; i++) {
		//탁구채의 위치를 정렬하고 현재 점수를 0으로 초기화 한다. (연속 최대 점수가 있으므로)
		//blades_right_ai[i]->SetRandomizeBlade_InitialYPos();
		//탁구채의 위치를 랜덤으로 설정하였지만 운에 따라 점수가 결정 되는 듯 해서 처음에 한곳으로 모을려고 한다.
		blades_right_ai[i]->blade_reset();
		blades_right_ai[i]->set_score(0);
	}

}

PingPong::PingPong(DrawScreen* ds_p, bool LearnMode, size_t AI_Blade_Count,
	size_t Current_Generation, size_t BladeSpeed, size_t BladeSize, size_t BallSpeed)
{
	//GA에서 값을 가져오기위해 LearnMode만듬
	//BladeBothSide는 blade를 양쪽에 놓을껀지 한쪽에 놓을건지 결정하는 매개변수
	//AmountBladeEach는 Blade를 몇개씩 놓을껀지 (BladeBothSide가 false일때만 쓸 수 있도록 하기?)
	this->LearnMode = LearnMode;
	assert(BladeSize <= SIZE_OF_ROW_SCREEN);
	this->BladeSize = BladeSize;
	//종료하려면 이 변수가 true가 되어야 한다.
	this->terminate = false;

	this->blades_left_player = nullptr;
	this->LearnMode_MaxAIScore = 0;
	this->GameTries = 0;
	this->HideUnnecessaryBlade = false;
	this->FastMode = false;
	
	this->ai_nn = nullptr;
	this->fm_p = nullptr;
	this->ds_p = nullptr;
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
	assert(ball);


	int blade_range_y_start = 0 + 1;
	int blade_range_y_end = SIZE_OF_ROW_SCREEN - 1 - 1;
	//LearnMode에서는 blade가 한쪽씩만 추가된다.
	//오른쪽 blade만 추가 LearnMode
	int right_blade_col_x = SIZE_OF_COL_SCREEN - 1 - 1;
	int left_blade_col = 0 + 1;
	if (LearnMode) {//학습 모드
		for (int id = 0; id < (int)RightAIBladeRemainCount; id++) {
			//탁구채의 위치를 랜덤으로 설정하니 점수를 얻는 것도 랜덤처럼 되어 제대로 학습이 되지 않았다.
			//움직여야 학습 하도록 만드는 것이 모든 탁구채를 모으는 것이다. 같은 기회
			int pos_y = SIZE_OF_ROW_SCREEN / 2;//;rand() % SIZE_OF_ROW_SCREEN;
			Blade* temp = new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
			assert(temp);
			blades_right_ai.push_back(temp);
		}
	}
	else {//경쟁 모드라면
		int id = 0;
		int pos_y = SIZE_OF_ROW_SCREEN / 2;
		blades_left_player = new Blade(left_blade_col, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
		assert(blades_left_player);
		Blade* temp_right_ai = new Blade(right_blade_col_x, pos_y, id, blade_range_y_start, blade_range_y_end, BladeSize, BladeSpeed);
		assert(temp_right_ai);
		blades_right_ai.push_back(temp_right_ai);
	}





}

// Incrementing score

void PingPong::CompeteMode_WhenBallHitWall(Blade* player) {
	//경쟁 모드에서만 사용
	//공과 탁구채들의 위치를 초기화 하고 점수를 올리는 함수
	assert(!LearnMode);
	player->add_score(1);
	ball->reset_ball();


	assert(RightAIBladeRemainCount == 1);
	CompeteMode_LeftPlayerScore = blades_left_player->get_score();
	CompeteMode_RightAIScore = blades_right_ai[0]->get_score();
	blades_right_ai[0]->blade_reset();
	blades_left_player->blade_reset();
}

void PingPong::ApplySizeInMapArr(ICON_NUMBER map_arr[][SIZE_OF_COL_SCREEN], ICON_NUMBER Fill_Icon, int start_x, int end_x, int start_y, int end_y)
{
	for (int x = start_x; x <= end_x; x++)
	{
		for (int y = start_y; y <= end_y; y++)
		{
			map_arr[y][x] = Fill_Icon;
		}
	}
}

void PingPong::AI_Recognize_Circum_And_Move()
{
	Coor ball_coor = ball->GetBallCoordinate();
	double input_arr[9] = { (double)ball_coor.x, (double)ball_coor.y };
	OneHotEncoding(input_arr, 2, ball->get_ball_direction());
	Coor ai_BladeCoor = blades_right_ai[0]->GetBladeCoordinate();
	input_arr[8] = ai_BladeCoor.y;

	size_t command = ai_nn->query(input_arr, 9);
	switch (command)
	{
	case NNOUT_DIRECTION::UP:
		blades_right_ai[0]->blade_move_up();
		break;
	case NNOUT_DIRECTION::DOWN:
		blades_right_ai[0]->blade_move_down();
		break;
	case NNOUT_DIRECTION::STOP_Neural:
		break;
	default:
		cout << "Error: AI가 방향을 알 수 없음" << endl;
	}
}

// Drawing the board (at each moment -- this will explain the blips)

void PingPong::draw_game_layout() {
	ICON_NUMBER map_arr[SIZE_OF_ROW_SCREEN][SIZE_OF_COL_SCREEN] = { BLANK_SYMBOL, };
	Coor blades_cor;
	if (!LearnMode) {//경쟁 모드면 LEFT BLADE 설치
		blades_cor = blades_left_player->GetBladeCoordinate();
		pair<int, int> range = blades_left_player->GetBladeRange();
		ApplySizeInMapArr(map_arr, LEFT_BLADE_SYMBOL, blades_cor.x, blades_cor.x,
			range.first, range.second);
	}
	for (size_t i = 0; i < blades_right_ai.size(); i++) {//AI의 BLADE
		blades_cor = blades_right_ai[i]->GetBladeCoordinate();
		pair<int, int> range = blades_right_ai[i]->GetBladeRange();
		ApplySizeInMapArr(map_arr, RIGHT_BLADE_SYMBOL, blades_cor.x, blades_cor.x,
			range.first, range.second);

	}

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



	ds_p->update_info(LearnMode, this->GameTries, this->LearnMode_MaxAIScore, CompeteMode_RightAIScore, CompeteMode_LeftPlayerScore,
		this->LearnMode_CurrentGeneration, this->FastMode, this->HideUnnecessaryBlade);

	ds_p->draw_layout(map_arr, SIZE_OF_ROW_SCREEN);
}

// function to respond to player inputs

void PingPong::play() {
	int delay;
	if (FastMode) delay = 0;
	else delay = DELAY_PER_FRAME;
	ball->move_ball();
	if (_kbhit()) { // if any key is pressed : take action
		char key = _getch(); // get the pressed key character

		switch (key) {
		case PLAYER_UP_KEY:
			if (blades_left_player)
				blades_left_player->blade_move_up();
			break;
		case PLAYER_DOWN_KEY:
			if (blades_left_player)
				blades_left_player->blade_move_down();
			break;
		case 'r':
			ball->randomize_ball_direction();
			break;
		case 'f':
			FastMode = !FastMode;
			break;
		case 'h':
			HideUnnecessaryBlade = !HideUnnecessaryBlade;
		case 'm':
			break;
		case 't':
			terminate = !terminate;
			break;
		}

		// if it's new game move the ball in random directions				
		if (ball->get_ball_direction() == STOP) ball->randomize_ball_direction();
	}

	Sleep(delay);
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

	static int Position_Move_Times = 0;
	if (RandomBallDirection_WhenRepeat) 
	{
		if (ball_x < SIZE_OF_COL_SCREEN)
		{
			Position_Move_Times++;
			if (Position_Move_Times > 1000)
			{
				ball->randomize_ball_direction();
				ball->randomize_ball_direction();
				ball->randomize_ball_direction();
				ball->randomize_ball_direction();
				ball->randomize_ball_direction();
				Position_Move_Times = 0;
			}
		}
	}

	// 바닥 경계에 닿았을때
	if (ball_y == BottomWall_Coor_y)
	{
		ball->change_ball_direction(ball->get_ball_direction() == DOWNRIGHT ? UPRIGHT : UPLEFT);
	}

	// 천장에 닿았을때
	if (ball_y == TopWall_Coor_y)
	{
		ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
	}


	// 오른쪽 벽에 닿았을때
	if (ball_x == RightWall_Coor_x)
	{
		//사망 횟수 의미
		GameTries++;
		//학습 모드
		if (LearnMode) 
		{
			//학습 모드에서 공을 막지 못해 오른쪽 벽에 공이 닿았을때 게임 다시 시작
			//공 원 위치, 탁구채 원위치, GameTries 증가
			LearnMode_WhenBallHitRightWall();
		}
		//경쟁 모드
		else 
		{
			//경쟁 모드일때 AI가 공을 치지 못하고 오른쪽 벽에 닿으면 상대편(player) 점수 올리고 초기화
			CompeteMode_WhenBallHitWall(blades_left_player);
		}

	}
	// 왼쪽 벽에 닿았을때
	if (ball_x == LeftWall_Coor_x)
	{
		if (LearnMode) ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
		else
		{
			GameTries++;
			//경쟁 모드일때는 BLADE 갯수가 반드시 한개여야 한다.
			assert(RightAIBladeRemainCount == 1);

			//경쟁 모드일때 플레이어가 공을 막지 못해 왼쪽 벽에 닿았을때 상대편(ai) 점수를 올리고 위치 초기화
			CompeteMode_WhenBallHitWall(blades_right_ai[0]);

		}

	}

	//공이 탁구채에 부딪친 경우에 대해서 설정한다.

	for (size_t i = 0; i < RightAIBladeRemainCount; i++)
	{
		//AI Blade와 공이 부딫쳤을때
		if (blades_right_ai[i]->is_hit_blade(ball_x, ball_y))
		{
			if (LearnMode) LearnMode_WhenBallHitAIBlade(i);
			else ball->change_ball_direction(ball->get_ball_direction() == UPRIGHT ? UPLEFT : DOWNLEFT);

		}
	}

	//Player Blade와 공이 부딫쳤을때
	//정의 되어 있을때만
	if (blades_left_player)
	{
		if (blades_left_player->is_hit_blade(ball_x, ball_y))
			ball->change_ball_direction(ball->get_ball_direction() == UPLEFT ? UPRIGHT : DOWNRIGHT);
	}
}

void PingPong::lets_ping_pong_compete_mode() {
	// 먼저 불러오고
	// weights를 가져오고

	while (!terminate) {
		draw_game_layout();
		play();
		monitor_ball();
		AI_Recognize_Circum_And_Move();
		Sleep(DELAY_PER_FRAME);
	}
}
bool PingPong::ShouldWeHideBlade()
{
	return this->HideUnnecessaryBlade;
}
void PingPong::OneHotEncoding(double input_arr[], int start_index, Ball_Direction dir)
{
	int end_index = start_index + DIRECTION_COUNT;
	for (int i = start_index; i < end_index; i++)
	{
		input_arr[i] = 0;
	}
	switch (dir)
	{
	case Ball_Direction::STOP:
		break;
	case LEFT:
		input_arr[start_index + LEFT - 1] = SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;
	case UPLEFT:
		input_arr[start_index + UPLEFT - 1] = SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;
	case DOWNLEFT:
		input_arr[start_index + DOWNLEFT - 1] = SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case RIGHT:
		input_arr[start_index + RIGHT - 1] = SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case UPRIGHT:
		input_arr[start_index + UPRIGHT - 1] = SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case DOWNRIGHT:
		input_arr[start_index + DOWNRIGHT - 1] = SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;

	}
}
size_t PingPong::CurrentBladeCount()
{
	return this->RightAIBladeRemainCount;
}
Coor PingPong::GetLeftPlayerBladeCoor()
{
	assert(!LearnMode);
	Coor loc = {};
	if (blades_left_player)
	{
		loc = blades_left_player->GetBladeCoordinate();
	}
	return loc;

}
vector<Coor> PingPong::GetAllRightAIBladeCoor()
{
	vector <Coor> temp;
	for (size_t i = 0; i < this->RightAIBladeRemainCount; i++) {
		temp.push_back(blades_right_ai[i]->GetBladeCoordinate());
	}
	return temp;
}

//모든 탁구채들의 점수 반환

//탁구공이 오른쪽 벽에 부딪쳐 다시 시작하기 전 연속 탁구 점수
//탁구공이 벽에 부딪치면 score는 0으로 초기화 한다. MAXScore은 score의 최대 값이다.
//한 세대가 완전히 끝나면 MAXScore 점수도 초기화 한다.
//얻는 값은 MAXScore 점수이다.
vector<Blade_Info> PingPong::GetAllBladesCountinusScoresMAX()
{
	vector <Blade_Info> temp;
	for (size_t i = 0; i < this->RightAIBladeRemainCount; i++) {
		Blade_Info temp_struct{};
		temp_struct.ID_index = blades_right_ai[i]->GetID();
		temp_struct.score = blades_right_ai[i]->GetCountinusMAXScore();
		temp.push_back(temp_struct);
	}
	return temp;
}
//탁구채들의 현재 점수 반환
vector<Blade_Info> PingPong::GetAllBladesCurrentScores()
{
	vector <Blade_Info> temp;
	for (size_t i = 0; i < this->RightAIBladeRemainCount; i++) {
		Blade_Info temp_struct;
		temp_struct.ID_index = blades_right_ai[i]->GetID();
		temp_struct.score = blades_right_ai[i]->get_score();
		temp.push_back(temp_struct);
	}
	return temp;
}
size_t PingPong::GetAIBladeScore(size_t index)
{
	assert(index < RightAIBladeRemainCount);
	return blades_right_ai[index]->get_score();
}

size_t PingPong::GetMaxScoreForLearn()
{
	//모든 탁구채들의 연속 탁구 최대 기록 중 가장 큰 값
	return LearnMode_MaxAIScore;
}

void PingPong::ClearAllBladesScore()
{
	LearnMode_MaxAIScore = 0;
	for (size_t i = 0; i < RightAIBladeRemainCount; i++)
	{
		blades_right_ai[i]->set_score(0);
		blades_right_ai[i]->CountinusMAXScore_SetScore(0);
	}
}

Coor PingPong::GetBallCoor()
{
	return ball->GetBallCoordinate();
}

void PingPong::SetRandomBallDirection()
{
	ball->randomize_ball_direction();
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

void PingPong::key_up(bool objects[], size_t size)
{
	assert(size == this->RightAIBladeRemainCount);
	for (size_t i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_up();
	}
}
void PingPong::key_down(bool objects[], size_t size) 
{
	assert(size == this->RightAIBladeRemainCount);
	for (size_t i = 0; i < size; i++) {
		if (objects[i]) blades_right_ai[i]->blade_move_down();
	}
}
