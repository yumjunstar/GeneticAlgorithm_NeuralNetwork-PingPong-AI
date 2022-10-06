#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(size_t blades_count) 
{
	Generation = 0;
	ppg = new PingPong;//처음에 게임 시작
	init(blades_count);//신경망 만들기
}
GeneticAlgorithm::~GeneticAlgorithm()
{
	delete[] nn;
	delete[] ppg;
}
//초기 집단 설정 신경망 만들기 각 세대마다 초기화 반복 아니면 setweight 만 하든지 둘중에 하나
void GeneticAlgorithm::init(size_t blades_count)
{
	if (nn) delete[] nn;
	nn = new NeuralNetwork[blades_count];
	this->all_blades_count = blades_count;
}
void GeneticAlgorithm::play()//게임 시작하고 여러 신경망을 평가
{
	while (Generation <= 1000) {
		ppg->lets_ping_pong();
		Generation++;
	}

}
void GeneticAlgorithm::choice()//신경망 고르기
{

}
void GeneticAlgorithm::crossover()//신경망 교차
{

}
void GeneticAlgorithm::mutation()//신경망 변이
{

}