#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	this->Generation = 0;
	this->ds_p = ds;
	this->AllAIBladesCount = blades_count;
	this->ppg = new PingPong(ds, true, blades_count);
	LoopRepeat = 0;

	Output_CMD_Array = new size_t[AllAIBladesCount];
	KeyUp_Binary_CMD = new bool[AllAIBladesCount];
	KeyDown_Binary_CMD = new bool[AllAIBladesCount];

	//신경망 생성
	nn = new NeuralNetwork[blades_count];
	for (int i = 0; i < blades_count; i++) {
		nn[i].make_neural_network(NeuralShape, NeuralLayerCount);
		nn[i].all_weight_reset_random();
	}

	//변수 초기화
	init();
}
GeneticAlgorithm::~GeneticAlgorithm()
{
	delete[] Output_CMD_Array;
	delete[] KeyDown_Binary_CMD;
	delete[] KeyUp_Binary_CMD;

	delete[] nn;
	delete ppg;
}
void GeneticAlgorithm::CleanUpBladesForVisability(int GameTries, int blade_id)
{
	if ((GameTries > 20) || (ppg->GetMaxScoreForLearn() > 15))
	{

		if (ppg->GetAIBladeScore(blade_id) < (size_t)5)
		{
			KeyUp_Binary_CMD[blade_id] = true;
			KeyDown_Binary_CMD[blade_id] = false;
		}
	}
}
//초기 집단 설정 신경망 만들기 각 세대마다 초기화 반복 아니면 setweight 만 하든지 둘중에 하나
void GeneticAlgorithm::init()
{

	AllBladeCoorVector.clear();
	AllBladeScoreVector.clear();

}
OneDNNWeights GeneticAlgorithm::DNN_Copy(OneDNNWeights source, int size)
{
	assert(size == WeightMatrixCount);
	OneDNNWeights temp = new MatrixXd[size];
	for (int i = 0; i < size; i++)
	{
		temp[i] = source[i];
	}
	return temp;
}
void GeneticAlgorithm::play()//게임 시작하고 여러 신경망을 평가해서 저장
{
	//게임을 몇번 플레이 하게 할까
	//한 세대당 100번 정도 플레이 하게 하기

	//모든 탁구채들의 점수 묶음


	size_t GameTries = 0;

	while (GameTries < PerGenerationGameTries)
	{

		GameTries = ppg->GetGameTries();
		ppg->draw_game_layout();
		ppg->monitor_ball();
		ppg->play();

		//공의 위치와 방향
		Coor BallCoor = ppg->GetBallCoor();
		Ball_Direction ball_direction = ppg->GetBallDirection();
		double ball_x = BallCoor.x / (double)SIZE_OF_COL_SCREEN;
		double ball_y = BallCoor.y / (double)SIZE_OF_ROW_SCREEN;
		double ball_dir = (int)ball_direction / (double)DIRECTION_COUNT;

		//모든 탁구채들의 위치 반환
		AllBladeCoorVector = ppg->GetAllRightAIBladeCoor();

		for (int blade_id = 0; blade_id < AllAIBladesCount; blade_id++)
		{
			double blade_x = AllBladeCoorVector[blade_id].x / (double) SIZE_OF_COL_SCREEN;
			double blade_y = AllBladeCoorVector[blade_id].y / (double) SIZE_OF_ROW_SCREEN;

			double input_arr[5] = { ball_x, ball_y, ball_dir, blade_x, blade_y };

			Output_CMD_Array[blade_id] = nn[blade_id].query(input_arr, 5);
			SetBladeDirection((NNOUT_DIRECTION)Output_CMD_Array[blade_id], blade_id);
			//CleanUpBladesForVisability(GameTries, blade_id);

		}
		this->ChangeRandomDirectionForPerfectLearn(LoopRepeat);
		ppg->key_up(KeyUp_Binary_CMD, AllAIBladesCount);
		ppg->key_down(KeyDown_Binary_CMD, AllAIBladesCount);
		LoopRepeat = (LoopRepeat + 1) % 1000000;
	}

	//평가된 점수를 저장
	AllBladeScoreVector = ppg->GetAllBladesScores();





}
void GeneticAlgorithm::choice()//신경망 고르기
{


	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool {return a.score > b.score; });
	//지울꺼니깐 미리 복사해 놓는다.
	MatrixXd* first = DNN_Copy(nn[AllBladeScoreVector[0].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount);
	MatrixXd* second = DNN_Copy(nn[AllBladeScoreVector[1].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount);
	//엘리트 두개

	for (int i = 0; i < EliteNeuralWeightMatrixVector.size(); i++)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();

	EliteNeuralWeightMatrixVector.push_back(first);
	EliteNeuralWeightMatrixVector.push_back(second);
	//포인터를 사용하여 걱정하였으나 중간에 할당 해제 과정없이 Setweight 만 하면 되므로 메모리 누수 걱정은 하지 않아도 된다.
	//점수들 중에서 가장 높은거 두개
}
void GeneticAlgorithm::SetBladeDirection(NNOUT_DIRECTION dir, int blade_id)
{
	switch ((NNOUT_DIRECTION)Output_CMD_Array[blade_id])
	{
	case UP:
		KeyUp_Binary_CMD[blade_id] = true;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	case DOWN:
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = true;
		break;
	case STOP:
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	default:
		printf("경고 GeneticAlgorithm.cpp::play\n");
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	}
}
void GeneticAlgorithm::crossover()//신경망 교차
{
	//한개의 신경망을 위한 가중치들의 묶음 값이다.
	MatrixXd* EliteWeightCrossOverForOneNN = new MatrixXd[WeightMatrixCount];



	//엘리트 가중치 묶음의 묶음 개수 vector<MatrixXd*>
	int EliteCount = EliteNeuralWeightMatrixVector.size();
	assert(EliteCount > 0);
	//엘리트 가중치끼리 다 더해서 평균을 낼 것이다.
	//엘리트 가중치 1개 -> 여러개의 행렬 덧셈

	for (int i = 0; i < WeightMatrixCount; ++i)
	{
		int rows = EliteNeuralWeightMatrixVector[0][i].rows();
		int cols = EliteNeuralWeightMatrixVector[0][i].cols();

		EliteWeightCrossOverForOneNN[i].resize(rows, cols);
		EliteWeightCrossOverForOneNN[i].setZero();
		EliteWeightCrossOverForOneNN[i] = (EliteNeuralWeightMatrixVector[0][i] + EliteNeuralWeightMatrixVector[1][i]) / 2.;

	}

	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN);
}

void GeneticAlgorithm::mutation()//신경망 변이
{


	OneDNNWeights temp = EliteNeuralWeightMatrixVector.back();
	//맨 뒤에껄 뽑아서
	for (int i = 0; EliteNeuralWeightMatrixVector.size() < AllAIBladesCount; i++)
	{
		OneDNNWeights new_temp = AddNormalDistribution(temp);
		EliteNeuralWeightMatrixVector.push_back(new_temp);
	}


}
void GeneticAlgorithm::apply()
{
	int SonCount = EliteNeuralWeightMatrixVector.size();
	assert(SonCount == AllAIBladesCount);
	for (int i = 0; i < SonCount; i++)
	{
		nn[i].set_weight(EliteNeuralWeightMatrixVector[i], WeightMatrixCount);
	}
}
OneDNNWeights GeneticAlgorithm::AddNormalDistribution(OneDNNWeights standard)
{
	random_device rd;
	mt19937_64 mt(rd());
	normal_distribution<double> d{ 0, 5 };
	OneDNNWeights copy_standard = DNN_Copy(standard, WeightMatrixCount);
	for (int i = 0; i < WeightMatrixCount; i++)
	{
		int rows = copy_standard[i].rows();
		int	cols = copy_standard[i].cols();
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < cols; k++)
			{
				copy_standard[i](j, k) += d(mt);
			}
		}
	}
	return copy_standard;
}
void GeneticAlgorithm::ChangeRandomDirectionForPerfectLearn(int RepeatLoop)
{
	if (RepeatLoop % 100 == 1)
	{
		for (int i = 0;i<10;i++) ppg->SetRandomBallDirection();
	}

}
void GeneticAlgorithm::LetsLearn()
{
	while (this->Generation <= 1000)
	{
		ppg->Reset();
		init();

		play();
		choice();
		crossover();
		mutation();
		apply();
		ppg->SetGeneration(++Generation);
	}

}