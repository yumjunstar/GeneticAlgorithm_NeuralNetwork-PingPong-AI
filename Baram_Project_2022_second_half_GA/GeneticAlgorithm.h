#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//이 한 세대이다.
private:
	//신경망
	NeuralNetwork* nn;
	//게임
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
};

