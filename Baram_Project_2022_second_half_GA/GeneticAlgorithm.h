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
	static constexpr int NeuralShape[] = { 9, 50, 50, 3 };
	static constexpr int NeuralLayerCount = 4;
	//������ ���� ��ġ x,y Ź��ä�� ��ġ x, y, ���� �����̾���.
	//���� x��ġ
	//���� y��ġ
	//���� ����1
	//���� ����2
	//���� ����3
	//���� ����4
	//���� ����5
	//���� ����6(�ٲ����� stop)
	//Ź��ä�� y ��ġ
	//

	static constexpr int WeightMatrixCount = NeuralLayerCount - 1;
	static constexpr double MutationMeanValue = 0;
	static constexpr double MutationSigmaValue = 0.01;
	static constexpr double MutationSigmaValue_BeforeGetScore = 10;
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

