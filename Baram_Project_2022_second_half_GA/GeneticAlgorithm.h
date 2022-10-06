#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
class GeneticAlgorithm
{
//init->play->choice->crossover->mutation
//�� �� �����̴�.
private:
	NeuralNetwork* nn;//�Ű��
	PingPong* ppg;//����
	size_t all_blades_count;
	int Generation;
public:
	GeneticAlgorithm(size_t blades_count);
	~GeneticAlgorithm();
	void init(size_t blades_count);//�ʱ� ���� ����
	void play();//���� �����ϰ� ���� �Ű���� ��
	void choice();//�Ű�� ����
	void crossover(); //�Ű�� ����
	void mutation();//�Ű�� ����
};

