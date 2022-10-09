#pragma once
#include "Common_Define.h"
#include "Blade.h"
#include "Ball.h"
#include "DrawScreen.h"

// Ping_Pong Class
class PingPong {
private:

	static constexpr char PLAYER_UP_KEY = 'w';
	static constexpr char PLAYER_DOWN_KEY = 's';

	static constexpr bool Default_LearnMode = true;
	static constexpr size_t Default_AI_BladeCount = SIZE_OF_ROW_SCREEN - 10;
	static constexpr size_t Default_Blade_Speed = 1;
	static constexpr size_t Default_Blade_Size = 1;
	static constexpr size_t Default_Ball_Speed = 1;
	static constexpr size_t Default_CurrentGeneration = 0;


	//학습모드인지 확인
	bool LearnMode;

	//플레이어의 탁구채 개수
	size_t LeftPlayerBladeRemainCount;

	//학습 모드에서 AI 탁구채의 최대 개수 거의 상수처럼 사용
	size_t RightAIBladeRemainCount;

	//학습 모드에서 AI 탁구채 점수 중 최대 점수
	size_t LearnMode_MaxAIScore;

	//경쟁 모드에서 플레이어의 점수
	size_t CompeteMode_LeftPlayerScore;
	//경쟁 모드에서 AI의 점수
	size_t CompeteMode_RightAIScore;

	//탁구채 크기
	size_t BladeSize;

	size_t GameTries;

	//학습모드에서 현재세대
	size_t LearnMode_CurrentGeneration;

	//게임 종료 변수
	bool terminate;

	//거의 도태되는 탁구채는 어떻게 할지 정하기
	bool HideUnnecessaryBlade;

	//가속모드
	bool FastMode;
	//Blade 객체 생성
	Blade* blades_left_player;
	vector<Blade*> blades_right_ai;
	Ball* ball;
	DrawScreen* ds_p;
	void LearnMode_WhenBallHitAIBlade(int blade_index);
	void LearnMode_WhenBallHitRightWall();

	// Incrementing score
	void CompeteMode_WhenBallHitWall(Blade* player);
	void ApplySizeInMapArr(ICON_NUMBER map_arr[][SIZE_OF_COL_SCREEN], 
		ICON_NUMBER Fill_Icon,int start_x, int end_x, int start_y, int end_y);
public:

	// constructor 학습하는 걸 가정하고 초기화

	PingPong(DrawScreen* ds, bool LearnMode = Default_LearnMode,
		size_t AI_BladeCount = Default_AI_BladeCount,
		size_t Current_Generation = Default_CurrentGeneration,
		size_t BladeSpeed = Default_Blade_Speed, 
		size_t BladeSize = Default_Blade_Size, 
		size_t BallSpeed = Default_Ball_Speed);
	~PingPong();

	void Reset();
	//살아 있는 탁구채 개수 반환
	size_t CurrentBladeCount();

	//플레이어 탁구채의 위치 반환
	Coor GetLeftPlayerBladeCoor();

	//모든 AI탁구채들의 위치 반환
	vector <Coor> GetAllRightAIBladeCoor();

	//모든 탁구채들의 점수 반환
	vector<Blade_Info> GetAllBladesCountinusScoresMAX();
	vector<Blade_Info> GetAllBladesCurrentScores();

	size_t GetAIBladeScore(size_t index);
	//최대 점수 반환 학습을 위한
	size_t GetMaxScoreForLearn();
	void ClearAllBladesScore();
	//현재 공의 위치 반환
	Coor GetBallCoor();

	void SetRandomBallDirection();
	Ball_Direction GetBallDirection();
	//게임 횟수 반환 학습을 위한
	size_t GetGameTries();
	void SetGameTries(size_t value);

	void SetGeneration(size_t gen);
	//탁구채들 위로
	void key_up(bool objects[], int size);

	//탁구채들 아래로
	void key_down(bool objects[], int size);



	// Drawing the board (at each moment -- this will explain the blips)
	void draw_game_layout(); // end of Draw function

	// function to respond to player inputs
	void play(); // end of input

	// function to moniter ball position
	void monitor_ball();

	//게임 시작 
	void lets_ping_pong();
	void lets_ping_pong(size_t repeat);
	bool ShouldWeHideBlade();
};