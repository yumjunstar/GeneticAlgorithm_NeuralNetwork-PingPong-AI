#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	//1등, 2등, 그리고 교배한 것 하나 총 3개가 기본적으로 항상 할당 되어있으므로 변이가 일어나려면 최소 4개가 되어야 한다.
	assert(blades_count >= 4);

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
		nn[i].make_neural_network(NeuralShape);
		nn[i].all_weight_reset_random();
		nn[i].SetActivationFunction("relu");
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
	if (ppg->GetMaxScoreForLearn() > CleanUpStartScore)
	{

		if (ppg->GetAIBladeScore(blade_id) < CleanUpMinimumScore)
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
void GeneticAlgorithm::OneHotEncoding(double input_arr[], int start_index, Ball_Direction dir)
{
	int end_index = start_index + DIRECTION_COUNT;
	for (int i = start_index; i < end_index; i++)
	{
		input_arr[i] = 0;
	}
	switch (dir)
	{
	case Ball_Direction::STOP:
		break;
	case LEFT:
		input_arr[start_index + LEFT - 1] = 1 * MultipleNumberForNNInput;
		break;
	case UPLEFT:
		input_arr[start_index + UPLEFT - 1] = 1 * MultipleNumberForNNInput;
		break;
	case DOWNLEFT:
		input_arr[start_index + DOWNLEFT - 1] = 1 * MultipleNumberForNNInput;
		break;
	case RIGHT:
		input_arr[start_index + RIGHT - 1] = 1 * MultipleNumberForNNInput;
		break;
	case UPRIGHT:
		input_arr[start_index + UPRIGHT - 1] = 1 * MultipleNumberForNNInput;
		break;
	case DOWNRIGHT:
		input_arr[start_index + DOWNRIGHT - 1] = 1 * MultipleNumberForNNInput;
		break;

	}
}

double GeneticAlgorithm::GetDistance(int start_x, int start_y, int end_x, int end_y)
{
	return sqrt(pow(end_x - start_x, 2) + pow(end_y - start_y, 2));
}

void GeneticAlgorithm::play()//게임 시작하고 여러 신경망을 평가해서 저장
{
	//게임을 몇번 플레이 하게 할까
	//한 세대당 100번 정도 플레이 하게 하기

	//모든 탁구채들의 점수 묶음

	//죽기전 연속 최대 점수 찾기

	//학습을 제대로 할려면 (점수가 안오른다면 ) GameTreis 의 횟수를 올려야 함
	// 아니면 탁구채의 크기를 늘리던가
	// 소수로 바꾸니깐 정확도가 너무 낮아서 탁구채가 1인 것으로는 힘든 것 같음
	// 점수가 1인 것은 탁구채가 아래로 또는 위로만 움직이는 탁구채에게 이동하다가 우연히 친 것일 수도 있으므로
	// 게임 트라이 횟수를 올리는 것이 좋다.
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


		double input_arr[9] = { (double)BallCoor.x, (double)BallCoor.y };// 실수의 정확도가 떨어져서 정수형으로 넣는게 나을 것 같음
	//	double input_arr[9] = { ((BallCoor.x / (double)SIZE_OF_COL_SCREEN) + 0.1) * MultipleNumberForNNInput
	//, ((BallCoor.y / (double)SIZE_OF_ROW_SCREEN) + 0.1) * MultipleNumberForNNInput };
		OneHotEncoding(input_arr, 2, ball_direction);


		AllBladeCoorVector = ppg->GetAllRightAIBladeCoor();

		for (int blade_id = 0; blade_id < AllAIBladesCount; blade_id++)
		{
			Coor BladeCoor = AllBladeCoorVector[blade_id];
			input_arr[8] = BladeCoor.y;
			//input_arr[9] = GetDistance(BallCoor.x, BallCoor.y, BladeCoor.x, BladeCoor.y);
			Output_CMD_Array[blade_id] = nn[blade_id].query(input_arr, InputNodeCount);
			SetBladeDirection((NNOUT_DIRECTION)Output_CMD_Array[blade_id], blade_id);
			if (ppg->ShouldWeHideBlade())
			{
				CleanUpBladesForVisability(GameTries, blade_id);
			}
		}


		//this->ChangeRandomDirectionForPerfectLearn(BallCoor.x, LoopRepeat);

		ppg->key_up(KeyUp_Binary_CMD, AllAIBladesCount);
		ppg->key_down(KeyDown_Binary_CMD, AllAIBladesCount);

		LoopRepeat = (LoopRepeat + 1) % 1000000;
	}

	//평가된 점수를 저장
	AllBladeScoreVector = ppg->GetAllBladesCountinusScoresMAX();
}
void GeneticAlgorithm::choice()//신경망 고르기
{
	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool {return a.score > b.score; });


	//10%의 엘리트 들을 뽑아 그대로 넣는다.
	//2보다 작을 경우에는 그냥 2개를 뽑는다.
	
	int elite_number = ((this->AllAIBladesCount * ChoiceProcessPercentage) < 2) ? 2 : (int)this->AllAIBladesCount * ChoiceProcessPercentage;

	//엘리트들만 미리 뽑아 복사 해놓는다. 이전 신경망들은 지울꺼기 때문이다. 아니면 특정 id만 빼고 지울까.
	vector<OneDNNWeights> elite_dnn;
	for (int i = 0; i < elite_number; i++) 
	{
		elite_dnn.push_back(DNN_Copy(nn[AllBladeScoreVector[i].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount));
	}

	//엘리트 두개
	for (int i = 0; i < EliteNeuralWeightMatrixVector.size(); i++)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();
	EliteNeuralWeightMatrixVector = elite_dnn;
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
	assert (EliteNeuralWeightMatrixVector.size() > 1);
	//엘리트 가중치끼리 다 더해서 평균을 낼 것이다.
	//엘리트 가중치 1개 -> 여러개의 행렬 덧셈

	for (int i = 0; i < WeightMatrixCount; ++i)
	{
		EliteWeightCrossOverForOneNN[i] = (EliteNeuralWeightMatrixVector[0][i] + EliteNeuralWeightMatrixVector[1][i]) / 2.;
	}

	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN);
}

void GeneticAlgorithm::mutation()//신경망 변이
{


	OneDNNWeights temp = EliteNeuralWeightMatrixVector.back(); 
	for (int i = 0; EliteNeuralWeightMatrixVector.size() < AllAIBladesCount; i++)
	{
		OneDNNWeights new_temp = AddNormalDistribution(temp);
		EliteNeuralWeightMatrixVector.push_back(new_temp);
	}
	//적어도 학습하려면 4개이상

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
	size_t Generation_MAXScore = ppg->GetMaxScoreForLearn();
	double sigma;
	if (Generation_MAXScore < MinimumScore_BeforeChangeSigma) sigma = this->MutationSigmaValue_BeforeGetScore;
	else sigma = this->MutationSigmaValue;

	normal_distribution<double> d{ this->MutationMeanValue,  sigma};


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
void GeneticAlgorithm::ChangeRandomDirectionForPerfectLearn(int ball_x, int RepeatLoop)
{
	if (ball_x < (SIZE_OF_COL_SCREEN / 2))
	{
		if (RepeatLoop % 10 == 1)
		{
			for (int i = 0; i < 10; i++) ppg->SetRandomBallDirection();
		}
	}
}
void GeneticAlgorithm::LetsLearn()
{
	while (true)
	{
		ppg->Reset();
		init();
		//목표 점수보다 크면 게임만 계속 반복
		do {
			play();
		} while (ppg->GetMaxScoreForLearn() >= GoalScore);
		choice();
		crossover();
		mutation();
		apply();
		ppg->SetGeneration(++Generation);
	}

}