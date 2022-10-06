#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//이 한 세대이다.
private:
	NeuralNetwork* nn;//신경망
	PingPong* ppg;//게임
	size_t all_blades_count;
	int Generation;
public:
	GeneticAlgorithm(size_t blades_count);
	~GeneticAlgorithm();
	void init(size_t blades_count);//초기 집단 설정
	void play();//게임 시작하고 여러 신경망을 평가
	void choice();//신경망 고르기
	void crossover(); //신경망 교차
	void mutation();//신경망 변이
};

