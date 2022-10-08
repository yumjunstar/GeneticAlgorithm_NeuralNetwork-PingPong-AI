#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	ds_p = ds;
	Generation = 0;




	ppg = new PingPong(ds, true, blades_count);
	
	//처음에 게임 시작
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
	//게임을 몇번 플레이 하게 할까
	//한 세대당 100번 정도 플레이 하게 하기
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
void GeneticAlgorithm::choice()//신경망 고르기
{

}
void GeneticAlgorithm::crossover()//신경망 교차
{

}
void GeneticAlgorithm::mutation()//신경망 변이
{

}