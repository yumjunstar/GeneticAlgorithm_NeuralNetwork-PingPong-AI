#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"

typedef MatrixXd* OneDNNWeights;

// init->play->choice->crossover->mutation �� �� ����
class GeneticAlgorithm
{
public:
	enum NNOUT_DIRECTION { UP, DOWN, STOP };
	const vector<int> NeuralShape = { 9, 10 , 10, 10, 2 };
	//2�� ����ϸ� ���ʿ� ������ 3�� �ϸ� �� ������ ������ �ִ´�.
	// ���� �˰����� ����ϴ� ���� ū �Ű���� �������� ���� �� ����.
	// ���� ���� �� ���� ũ�� �ٲ������.
	// ��� ���� 1���� ���� ���� ���ع����� �ڷΰ� ���� �Ű���� ��������. �Ҽ� ��Ȯ���� ���ų�
	// 1���� ū ���� ���ؾ� �Ѵ�.
	const int InputNodeCount = NeuralShape[0];
	const int OutputNodeCount = NeuralShape[NeuralShape.size() - 1];
	const int NeuralLayerCount = NeuralShape.size();
	// ������ �Ű�� �Է� : ���� ��ġ x, y, Ź��ä�� ��ġ x, y, ���� ����
	// ���Ŀ� ���� �õ�
	// ���� x��ġ
	// ���� y��ġ
	// �Ʒ��� OneHotEncoding
	// ���� ����1
	// ���� ����2
	// ���� ����3
	// ���� ����4
	// ���� ����5
	// ���� ����6(�ٲ����� stop)
	// Ź��ä�� y ��ġ
	//

	// �ι�° �õ�
	// ���� ���� ��� int �״�� �Է��ϰ��� �Ѵ�. �̷��� �ϸ� sigmoid �� ����� �Ұ��� �ϰ� relu�� ������ ���̴�.
	// ���� �ߴ�. �� ���� �����⸸ �ϰ� ����� �۵� ���� �ʴ´� �Ƹ� ���� �ִ밪�� �޶� �׷� �� ����.

	//����° �õ� ���� Ź��ä���� �Ÿ��� ��� �밢�� �Ÿ�
	// ���� �̻��ϰ� �������� ������. �� ���� Ŀ�� �׷���
	const int WeightMatrixCount = NeuralLayerCount - 1;
	static constexpr double MutationMeanValue = 0;
	static constexpr double MutationSigmaValue = 0.01;
	static constexpr double MutationSigmaValue_BeforeGetScore = 0.05;
	static constexpr size_t MinimumScore_BeforeChangeSigma = 2;
	//���� ���� �ʹ� ũ�� �ϸ� �ȵȴ�. �׷� �Ű���� ���峭��.
	static constexpr double ChoiceProcessPercentage = 0.6;
	static constexpr int PerGenerationGameTries = 100;

	static constexpr size_t CleanUpStartScore = 20;
	static constexpr size_t CleanUpMinimumScore = 5;
	static constexpr int MultipleNumberForNNInput = 1;


	static constexpr int GoalScore = 200;
	GeneticAlgorithm(DrawScreen* ds, size_t blades_count);
	~GeneticAlgorithm();

	void init();
	void play();
	void choice();
	void crossover();
	void mutation();
	void apply();
	void LetsLearn();


private:


	//�Ű��
	NeuralNetwork* nn;
	PingPong* ppg;
	DrawScreen* ds_p;

	size_t AllAIBladesCount;
	size_t Generation;
	size_t LoopRepeat;

	vector <Blade_Info> AllBladeScoreVector;
	vector <Coor> AllBladeCoorVector;
	vector <OneDNNWeights> EliteNeuralWeightMatrixVector;

	size_t* Output_CMD_Array;
	bool* KeyUp_Binary_CMD;
	bool* KeyDown_Binary_CMD;




	OneDNNWeights DNN_Copy(OneDNNWeights source, int size);
	void CleanUpBladesForVisability(int GameTries, int blade_id);
	void SetBladeDirection(NNOUT_DIRECTION direction, int blade_id);
	OneDNNWeights AddNormalDistribution(OneDNNWeights standard);
	void ChangeRandomDirectionForPerfectLearn(int ball_x, int RepeatLoop);
	void OneHotEncoding(double input_arr[], int start_index, Ball_Direction dir);
	double GetDistance(int start_x, int start_y, int end_x, int end_y);
	
};

