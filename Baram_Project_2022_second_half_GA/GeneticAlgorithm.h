#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
typedef MatrixXd* OneDNNWeights;
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//�� �� �����̴�.
private:
	// �Ű������ ��� ó������ UP�� DOWN�� STOP
	enum NNOUT_DIRECTION { UP, DOWN, STOP };
	static constexpr int NeuralShape[] = { 5, 10, 20, 30, 20, 10, 3 };
	static constexpr int NeuralLayerCount = 7;
	static constexpr int WeightMatrixCount = NeuralLayerCount - 1;
	static constexpr int MutationMeanValue = 0;
	static constexpr int MutationSigmaValue = 10;
	static constexpr int PerGenerationGameTries = 100;



	//���� �迭 ����� �Ҵ� �����ϴ°� ���� �ȵǼ� ���� vector�� ����ϰ��� ��

	//�Ű��
	NeuralNetwork* nn;
	//����
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

