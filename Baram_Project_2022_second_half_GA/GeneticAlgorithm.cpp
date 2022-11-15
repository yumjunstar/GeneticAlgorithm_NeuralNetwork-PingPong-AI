#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	//1등, 2등, 그리고 교배한 것 하나 총 3개가 기본적으로 항상 할당 되어있으므로 변이가 일어나려면 최소 4개가 되어야 한다.
	assert(blades_count >= 4);
	this->CurrentScoreHuddle = this->FirstScoreHurdle;
	this->Generation = 0;
	this->ds_p = ds;
	this->LoopRepeat = 0;
	this->AllAIBladesCount = blades_count;
	this->ppg = new PingPong(ds, true, blades_count);



	Output_CMD_Array = new size_t[AllAIBladesCount];
	KeyUp_Binary_CMD = new bool[AllAIBladesCount];
	KeyDown_Binary_CMD = new bool[AllAIBladesCount];
	assert(ppg);
	assert(Output_CMD_Array);
	assert(KeyUp_Binary_CMD);
	assert(KeyDown_Binary_CMD);
	assert(fm);
	//신경망 생성
	nn = new NeuralNetwork[blades_count];
	assert(nn);
	for (size_t i = 0; i < blades_count; ++i) {
		nn[i].make_neural_network(NeuralShape);
		if (this->ResetWeightsMode == UNIFORM_RANDOM)
		{
			nn[i].all_weight_reset_random();
		}
		else if (this->ResetWeightsMode == NORMAL_ZERO)
		{
			nn[i].all_weight_reset_normal(MutationMeanValue, MutationSigmaValue_BeforeGetScore);
		}
		nn[i].SetActivationFunction("relu");
		//EliteNeuralWeightMatrixVector.push_back(nn[i].ReturnAllWeightMatrix());
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
	delete fm_p;
}
void GeneticAlgorithm::CleanUpBladesForVisability(size_t GameTries, int blade_id)
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
	// 맨 처음에 학습
	AllBladeCoorVector.clear();
	AllBladeScoreVector.clear();

}
OneDNNWeights GeneticAlgorithm::DNN_Copy(OneDNNWeights source, size_t size)
{
	assert(size == WeightMatrixCount);
	OneDNNWeights temp = new MatrixXd[size];
	assert(temp);
	for (size_t i = 0; i < size; ++i)
	{
		temp[i] = source[i];
	}
	return temp;
}



void GeneticAlgorithm::SaveAllDNNWeightsIntoFile()
{
	//순서대로 신경망을 정렬하고 파일에 저장하는 함수
	size_t id_index = 0;
	size_t score = 0;
	double distance = 0;
	// 존재 해야 하니깐
	assert(AllBladeScoreVector());

	size_t top_score_id = AllBladeScoreVector[0].ID_index;
	size_t top_score = AllBladeScoreVector[0].score;
	fm_p->WriteCurrentGeneration(Generation);
	fm_p->Write_OneDNNWeights_IntoTopFile(Generation, NeuralShape, nn[top_score_id].ReturnAllWeightMatrix(), nn[top_score_id].GetWeightMatrixCount(), top_score);
	
	//OneDNNWeights_Include_Info abc = fm->Read_OneDNNWeights_FromFile();
	//fm->Write_OneDNNWeights_IntoTopFile(Generation + 10000, abc.NeuralShape, abc.weights, abc.weights_count, -1);
	for (size_t i = 0; i < AllAIBladesCount; ++i)
	{
		id_index = AllBladeScoreVector[i].ID_index;
		score = AllBladeScoreVector[i].score;
		distance = AllBladeScoreVector[i].last_distance_from_ball;
		fm_p->Write_OneDNNWeights_IntoSpecificFile(Generation, id_index, nn[id_index].ReturnAllWeightMatrix(),	nn[id_index].GetWeightMatrixCount(), score, distance);
	}
}

void GeneticAlgorithm::SaveStatistics()
{
	fm_p->Write_Statistics(Generation, ppg->GetGameTries(), ppg->GetMaxScoreForLearn());
}

void GeneticAlgorithm::CurrentHurdleUpdate()
{
	if (this->FirstScoreHurdle > 0)
	{
		if ((ppg->GetGameTries() == 0) && (ppg->GetMaxScoreForLearn() == this->CurrentScoreHuddle))
		{
			this->CurrentScoreHuddle += this->FirstScoreHurdle;
		}
	}
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
	while ((GameTries < PerGenerationGameTries)&& (ppg->GetMaxScoreForLearn() < CurrentScoreHuddle))
	{

		GameTries = ppg->GetGameTries();
		ppg->draw_game_layout();
		ppg->monitor_ball();
		ppg->play();

		//공의 위치와 방향
		Coor BallCoor = ppg->GetBallCoor();

		Ball_Direction ball_direction = ppg->GetBallDirection();


		double input_arr[9] = { (double)BallCoor.x/(SIZE_OF_COL_SCREEN/10.0), (double)BallCoor.y/(SIZE_OF_ROW_SCREEN/10.0) };// 실수의 정확도가 떨어져서 정수형으로 넣는게 나을 것 같음
	//	double input_arr[9] = { ((BallCoor.x / (double)SIZE_OF_COL_SCREEN) + 0.1) * MultipleNumberForNNInput
	//, ((BallCoor.y / (double)SIZE_OF_ROW_SCREEN) + 0.1) * MultipleNumberForNNInput };
		nn[0].OneHotEncoding(input_arr, 2, ball_direction);


		AllBladeCoorVector = ppg->GetAllRightAIBladeCoor();

		for (size_t blade_id = 0; blade_id < AllAIBladesCount; ++blade_id)
		{
			Coor BladeCoor = AllBladeCoorVector[blade_id];
			input_arr[8] = (double)BladeCoor.y/(SIZE_OF_ROW_SCREEN/10.0);
			//input_arr[9] = GetDistance(BallCoor.x, BallCoor.y, BladeCoor.x, BladeCoor.y);
			Output_CMD_Array[blade_id] = nn[blade_id].query(input_arr, InputNodeCount);
			SetBladeDirection((NNOUT_DIRECTION)Output_CMD_Array[blade_id], blade_id);
			if (ppg->ShouldWeHideBlade())
			{
				CleanUpBladesForVisability(GameTries, blade_id);
			}
		}




		ppg->key_up(KeyUp_Binary_CMD, AllAIBladesCount);
		ppg->key_down(KeyDown_Binary_CMD, AllAIBladesCount);
		this->ChangeRandomDirectionForPerfectLearn(BallCoor.x, LoopRepeat);
		LoopRepeat = (LoopRepeat + 1) % 1000000;
	}

	//평가된 점수를 저장
	AllBladeScoreVector = ppg->GetAllBladesCountinusScoresMAX();
}
void GeneticAlgorithm::choice()//신경망 고르기
{
	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool { 
		if (a.score > b.score)
		{
			return true;
		}
		else if (a.score == b.score)
		{
			if (a.last_distance_from_ball < b.last_distance_from_ball) // 점수가 같을때 더 가까운 곳에 떨어진 것이 점수가 더 높음
			{
				return true;
			}
		}
		return false;
		});


	//10%의 엘리트 들을 뽑아 그대로 넣는다.
	//2보다 작을 경우에는 그냥 2개를 뽑는다.
	
	size_t elite_number = ((this->AllAIBladesCount * Elite_Percentage) < 2) ? 2 : (size_t)(this->AllAIBladesCount * Elite_Percentage);

	//엘리트들만 미리 뽑아 복사 해놓는다. 이전 신경망들은 지울꺼기 때문이다. 아니면 특정 id만 빼고 지울까.
	vector<OneDNNWeights> elite_dnn;
	for (size_t i = 0; i < elite_number; ++i) 
	{
		//DNN_Copy는 신경망 하나를 DeepCopy 하는 함수이다. 이를 pushback 해서 다음 세대를 위해 채운다.
		//순서대로 push 한다. 점수가 가장 높은게 top 에 위치한다.
		elite_dnn.push_back(DNN_Copy(nn[AllBladeScoreVector[i].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount));
	}

	//엘리트 두개
	for (size_t i = 0; i < EliteNeuralWeightMatrixVector.size(); ++i)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();
	//벡터이므로 하나하나 할당 해제 하고 삭제
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
	case STOP_Neural:
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	default:
		printf("경고 GeneticAlgorithm.cpp::SetDirection\n");
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	}
}
void GeneticAlgorithm::crossover()//신경망 교차
{
	//한개의 신경망을 위한 가중치들의 묶음 값이다.
	MatrixXd* EliteWeightCrossOverForOneNN1 = new MatrixXd[WeightMatrixCount];
	MatrixXd* EliteWeightCrossOverForOneNN2 = new MatrixXd[WeightMatrixCount];
	assert(EliteWeightCrossOverForOneNN1);
	assert(EliteWeightCrossOverForOneNN2);

	//엘리트 가중치 묶음의 묶음 개수 vector<MatrixXd*>
	assert (EliteNeuralWeightMatrixVector.size() > 1);
	//엘리트 가중치끼리 다 더해서 평균을 낼 것이다.
	//엘리트 가중치 1개 -> 여러개의 행렬 덧셈
	size_t elite_count = EliteNeuralWeightMatrixVector.size();

	size_t choice_elite_one_index = rand() % elite_count;
	//엘리트트가 3개이면
	//3부터 총 탁구채 개수만큼
	size_t choice_notelite_one_index = rand() %(this->AllAIBladesCount - 1 - elite_count + 1) + elite_count;
	assert(choice_notelite_one_index == this->AllAIBladesCount);
	// a ~ b-1까지

	//엘리트와 평범 교배
	for (size_t i = 0; i < WeightMatrixCount; ++i) // 가중치 행렬의 개수
	{
		// 엘리트에서 하나 랜덤으로 고르고, 평범한테 랜덤으로 골라서 교배 시키자
		EliteWeightCrossOverForOneNN1[i] = (EliteNeuralWeightMatrixVector[choice_elite_one_index][i] 
			+ nn[AllBladeScoreVector[choice_notelite_one_index].ID_index].ReturnAllWeightMatrix()[i]) / 2.;
	}

	//엘리트와 엘리트들끼리 교배
	for (size_t i = 0; i < WeightMatrixCount; ++i) // 가중치 행렬의 개수
	{
		// 엘리트에서 하나 랜덤으로 고르고, 평범한테 랜덤으로 골라서 교배 시키자
		EliteWeightCrossOverForOneNN2[i] = (EliteNeuralWeightMatrixVector[0][i] + EliteNeuralWeightMatrixVector[1][i]) / 2.;
	}

	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN1);
	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN2);
}

void GeneticAlgorithm::mutation()//신경망 변이
{

	size_t size_of_elite = EliteNeuralWeightMatrixVector.size();

	OneDNNWeights temp1 = EliteNeuralWeightMatrixVector[size_of_elite - 1];
	OneDNNWeights temp2 = EliteNeuralWeightMatrixVector[size_of_elite - 2];

	// 뒤에서 한개 두개 뽑아서 (그게 교차 된거 두개 니깐)

	for (size_t i = 0; EliteNeuralWeightMatrixVector.size() < AllAIBladesCount; ++i)
	{
		// 번갈아가면서 넣기
		OneDNNWeights new_temp1 = AddNormalDistribution(temp1);
		EliteNeuralWeightMatrixVector.push_back(new_temp1);
		
		if (EliteNeuralWeightMatrixVector.size() < AllAIBladesCount) {
			OneDNNWeights new_temp2 = AddNormalDistribution(temp2);
			EliteNeuralWeightMatrixVector.push_back(new_temp2);
		}
	}
	assert(AllAIBladesCount == EliteNeuralWeightMatrixVector.size());
}
void GeneticAlgorithm::apply()
{
	size_t SonCount = EliteNeuralWeightMatrixVector.size();
	assert(SonCount == AllAIBladesCount);
	for (size_t  i = 0; i < SonCount; ++i)
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


	for (size_t i = 0; i < WeightMatrixCount; ++i)
	{
		size_t rows = (size_t)copy_standard[i].rows();
		size_t cols = (size_t)copy_standard[i].cols();
		for (size_t j = 0; j < rows; ++j)
		{
			for (size_t k = 0; k < cols; ++k)
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
		if (RepeatLoop % 1000 == 1)
		{
			for (int i = 0; i < 10; ++i) ppg->SetRandomBallDirection();
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
		SaveAllDNNWeightsIntoFile();
		crossover();
		mutation();
		apply();
		SaveStatistics();
		ppg->SetGeneration(++Generation);
		CurrentHurdleUpdate();
		assert(_CrtCheckMemory());
	}

}

void GeneticAlgorithm::SetFileManage_Pointer(FileManage* fm)
{
	fm_p = fm;
}

void GeneticAlgorithm::SetDrawScreen_Pointer(DrawScreen* ds)
{
	ds_p = ds;
}
