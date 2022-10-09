#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
typedef MatrixXd* OneDNNWeights;
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//이 한 세대이다.
private:
	// 신경망에서 출력 처음부터 UP과 DOWN과 STOP
	enum NNOUT_DIRECTION { UP, DOWN, STOP };
	static constexpr int NeuralShape[] = { 9, 50, 50, 3 };
	static constexpr int NeuralLayerCount = 4;
	//원래는 공의 위치 x,y 탁구채의 위치 x, y, 공의 방향이었다.
	//공의 x위치
	//공의 y위치
	//공의 방향1
	//공의 방향2
	//공의 방향3
	//공의 방향4
	//공의 방향5
	//공의 방향6(다꺼지면 stop)
	//탁구채의 y 위치
	//

	static constexpr int WeightMatrixCount = NeuralLayerCount - 1;
	static constexpr double MutationMeanValue = 0;
	static constexpr double MutationSigmaValue = 0.01;
	static constexpr double MutationSigmaValue_BeforeGetScore = 10;
	static constexpr int PerGenerationGameTries = 100;



	//동적 배열 만들고 할당 해제하는게 감당 안되서 이중 vector로 사용하고자 함

	//신경망
	NeuralNetwork* nn;
	//게임
	PingPong* ppg;
	size_t AllAIBladesCount;
	DrawScreen* ds_p;
	int Generation;


	vector <Blade_Info> AllBladeScoreVector;
	vector <Coor> AllBladeCoorVector;

	vector <OneDNNWeights> EliteNeuralWeightMatrixVector;

	OneDNNWeights DNN_Copy(OneDNNWeights source, int size);
	size_t* Output_CMD_Array;
	bool* KeyUp_Binary_CMD;
	bool* KeyDown_Binary_CMD;


	size_t LoopRepeat;

	void CleanUpBladesForVisability(int GameTries, int blade_id);
	void ChangeRandomDirectionForPerfectLearn(int RepeatLoop);
	void SetBladeDirection(NNOUT_DIRECTION direction, int blade_id);
	OneDNNWeights AddNormalDistribution(OneDNNWeights standard);
	void OneHotEncoding(double input_arr[], int start_index, Ball_Direction dir);
public:
	GeneticAlgorithm(DrawScreen* ds, size_t blades_count);
	~GeneticAlgorithm();

	void init();
	void play();
	void choice();
	void crossover();
	void mutation();
	void apply();
	void LetsLearn();

};

