#pragma once
#include "Common_Define.h"
#include "Blade.h"
#include "Ball.h"
#include "DrawScreen.h"
#include "FileManage.h"
// Ping_Pong Class


class PingPong {
private:

	static constexpr char PLAYER_UP_KEY = 'w';
	static constexpr char PLAYER_DOWN_KEY = 's';

	//Default Mode

	static constexpr bool Default_LearnMode = true;
	static constexpr size_t Default_AI_BladeCount = SIZE_OF_ROW_SCREEN - 10;
	static constexpr size_t Default_Blade_Speed = 1;
	static constexpr size_t Default_Blade_Size = 3;
	static constexpr size_t Default_Ball_Speed = 1;
	static constexpr size_t Default_CurrentGeneration = 0;

	//Option settings

	static constexpr bool RandomBallPos_WhenRespawn = true;
	static constexpr bool RandomBallDirection_WhenRespawn = true;
	static constexpr Ball_Direction DefaultBallDirection_WhenRespawn = Ball_Direction::RIGHT;
	static constexpr bool RandomBallDirection_WhenHitBlade = true;
	static constexpr bool RandomBallDirection_WhenRepeat = true;

	//ó�� ���� �ʹ� ��ư� ����� ����� �н� �ȵ�



	//�н�������� Ȯ��
	bool LearnMode;

	//�÷��̾��� Ź��ä ����
	size_t LeftPlayerBladeRemainCount;

	//�н� ��忡�� AI Ź��ä�� �ִ� ���� ���� ���ó�� ���
	size_t RightAIBladeRemainCount;

	//�н� ��忡�� AI Ź��ä ���� �� �ִ� ����
	size_t LearnMode_MaxAIScore;

	//���� ��忡�� �÷��̾��� ����
	size_t CompeteMode_LeftPlayerScore;
	//���� ��忡�� AI�� ����
	size_t CompeteMode_RightAIScore;

	//Ź��ä ũ��
	size_t BladeSize;

	size_t GameTries;

	//�н���忡�� ���缼��
	size_t LearnMode_CurrentGeneration;

	//���� ���� ����
	bool terminate;

	//���� ���µǴ� Ź��ä�� ��� ���� ���ϱ�
	bool HideUnnecessaryBlade;

	//���Ӹ��
	bool FastMode;
	//Blade ��ü ����
	Blade* blades_left_player;
	vector<Blade*> blades_right_ai;
	Ball* ball;



	// ȭ�� �׸��� Ŭ����
	DrawScreen* ds_p;

	// ���� ������ϴ� Ŭ����
	FileManage* fm_p;

	// AI�� �Ű��
	NeuralNetwork* ai_nn;

	void InitAI();

	void LearnMode_WhenBallHitAIBlade(int blade_index);
	void LearnMode_WhenBallHitRightWall();

	// Incrementing score
	void CompeteMode_WhenBallHitWall(Blade* player);
	void ApplySizeInMapArr(ICON_NUMBER map_arr[][SIZE_OF_COL_SCREEN], 
		ICON_NUMBER Fill_Icon,int start_x, int end_x, int start_y, int end_y);

	void AI_Recognize_Circum_And_Move();
public:

	// constructor �н��ϴ� �� �����ϰ� �ʱ�ȭ

	PingPong(DrawScreen* ds_p, bool LearnMode = Default_LearnMode,
		size_t AI_BladeCount = Default_AI_BladeCount,
		size_t Current_Generation = Default_CurrentGeneration,
		size_t BladeSpeed = Default_Blade_Speed, 
		size_t BladeSize = Default_Blade_Size, 
		size_t BallSpeed = Default_Ball_Speed);
	~PingPong();

	void SetDrawScreen_Pointer(DrawScreen* ds);
	void SetFileManage_Pointer(FileManage* fm);

	void Reset();
	//��� �ִ� Ź��ä ���� ��ȯ
	size_t CurrentBladeCount();

	//�÷��̾� Ź��ä�� ��ġ ��ȯ
	Coor GetLeftPlayerBladeCoor();

	//��� AIŹ��ä���� ��ġ ��ȯ
	vector <Coor> GetAllRightAIBladeCoor();

	//��� Ź��ä���� ���� ��ȯ
	vector<Blade_Info> GetAllBladesCountinusScoresMAX();
	vector<Blade_Info> GetAllBladesCurrentScores();

	size_t GetAIBladeScore(size_t index);
	//�ִ� ���� ��ȯ �н��� ����
	size_t GetMaxScoreForLearn();
	void ClearAllBladesScore();
	//���� ���� ��ġ ��ȯ
	Coor GetBallCoor();

	void SetRandomBallDirection();
	Ball_Direction GetBallDirection();
	//���� Ƚ�� ��ȯ �н��� ����
	size_t GetGameTries();
	void SetGameTries(size_t value);

	void SetGeneration(size_t gen);
	//Ź��ä�� ����
	void key_up(bool objects[], size_t size);

	//Ź��ä�� �Ʒ���
	void key_down(bool objects[], size_t size);



	// Drawing the board (at each moment -- this will explain the blips)
	void draw_game_layout(); // end of Draw function

	// function to respond to player inputs
	void play(); // end of input

	// function to moniter ball position
	void monitor_ball();

	//���� ���� 
	void lets_ping_pong_compete_mode();
	bool ShouldWeHideBlade();



	void OneHotEncoding(double input_arr[], int start_index, Ball_Direction dir);
};