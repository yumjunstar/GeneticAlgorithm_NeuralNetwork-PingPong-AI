#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//�� �� �����̴�.
private:
	// �Ű������ ��� ó������ UP�� DOWN�� STOP
	enum NNOUT_DIRECTION { UP, DOWN, STOP };
	//�Ű��
	NeuralNetwork* nn;
	//����
	PingPong* ppg;
	size_t all_blades_count;
	DrawScreen* ds_p;
	int Generation;
public:
	GeneticAlgorithm(DrawScreen* ds, size_t blades_count);
	~GeneticAlgorithm();
	void init(size_t blades_count);
	void play();
	void choice();
	void crossover();
	void mutation();
	void apply();
	void LetsLearn();

};

