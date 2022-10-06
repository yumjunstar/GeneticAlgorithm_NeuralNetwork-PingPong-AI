#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(size_t blades_count) 
{
	Generation = 0;
	ppg = new PingPong;//ó���� ���� ����
	init(blades_count);//�Ű�� �����
}
GeneticAlgorithm::~GeneticAlgorithm()
{
	delete[] nn;
	delete[] ppg;
}
//�ʱ� ���� ���� �Ű�� ����� �� ���븶�� �ʱ�ȭ �ݺ� �ƴϸ� setweight �� �ϵ��� ���߿� �ϳ�
void GeneticAlgorithm::init(size_t blades_count)
{
	if (nn) delete[] nn;
	nn = new NeuralNetwork[blades_count];
	this->all_blades_count = blades_count;
}
void GeneticAlgorithm::play()//���� �����ϰ� ���� �Ű���� ��
{
	while (Generation <= 1000) {
		ppg->lets_ping_pong();
		Generation++;
	}

}
void GeneticAlgorithm::choice()//�Ű�� ����
{

}
void GeneticAlgorithm::crossover()//�Ű�� ����
{

}
void GeneticAlgorithm::mutation()//�Ű�� ����
{

}