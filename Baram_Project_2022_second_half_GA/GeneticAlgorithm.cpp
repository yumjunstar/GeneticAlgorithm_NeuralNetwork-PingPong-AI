#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	ds_p = ds;
	Generation = 0;




	ppg = new PingPong(ds, true, blades_count);
	
	//ó���� ���� ����
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
	//������ ��� �÷��� �ϰ� �ұ�
	//�� ����� 100�� ���� �÷��� �ϰ� �ϱ�
	const int repeat = 100;
	vector <size_t> AllBladeScore;
	vector <Coor> AllBladeCoor;
	Coor BallCoor;
	while (ppg->GetGameTries() <= repeat) 
	{
		ppg->draw_game_layout();
		ppg->play();
		ppg->monitor_ball();
		AllBladeCoor = ppg->GetAllRightAIBladeCoor();
		BallCoor = ppg->GetBallCoor();
	}
	AllBladeScore = ppg->GetAllBladesScores();
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