#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	//1��, 2��, �׸��� ������ �� �ϳ� �� 3���� �⺻������ �׻� �Ҵ� �Ǿ������Ƿ� ���̰� �Ͼ���� �ּ� 4���� �Ǿ�� �Ѵ�.
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
	//�Ű�� ����
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

	//���� �ʱ�ȭ
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
//�ʱ� ���� ���� �Ű�� ����� �� ���븶�� �ʱ�ȭ �ݺ� �ƴϸ� setweight �� �ϵ��� ���߿� �ϳ�
void GeneticAlgorithm::init()
{
	// �� ó���� �н�
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
	//������� �Ű���� �����ϰ� ���Ͽ� �����ϴ� �Լ�
	size_t id_index = 0;
	size_t score = 0;
	double distance = 0;
	// ���� �ؾ� �ϴϱ�
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

void GeneticAlgorithm::play()//���� �����ϰ� ���� �Ű���� ���ؼ� ����
{
	//������ ��� �÷��� �ϰ� �ұ�
	//�� ����� 100�� ���� �÷��� �ϰ� �ϱ�

	//��� Ź��ä���� ���� ����

	//�ױ��� ���� �ִ� ���� ã��

	//�н��� ����� �ҷ��� (������ �ȿ����ٸ� ) GameTreis �� Ƚ���� �÷��� ��
	// �ƴϸ� Ź��ä�� ũ�⸦ �ø�����
	// �Ҽ��� �ٲٴϱ� ��Ȯ���� �ʹ� ���Ƽ� Ź��ä�� 1�� �����δ� ���� �� ����
	// ������ 1�� ���� Ź��ä�� �Ʒ��� �Ǵ� ���θ� �����̴� Ź��ä���� �̵��ϴٰ� �쿬�� ģ ���� ���� �����Ƿ�
	// ���� Ʈ���� Ƚ���� �ø��� ���� ����.
	size_t GameTries = 0;
	while ((GameTries < PerGenerationGameTries)&& (ppg->GetMaxScoreForLearn() < CurrentScoreHuddle))
	{

		GameTries = ppg->GetGameTries();
		ppg->draw_game_layout();
		ppg->monitor_ball();
		ppg->play();

		//���� ��ġ�� ����
		Coor BallCoor = ppg->GetBallCoor();

		Ball_Direction ball_direction = ppg->GetBallDirection();


		double input_arr[9] = { (double)BallCoor.x/(SIZE_OF_COL_SCREEN/10.0), (double)BallCoor.y/(SIZE_OF_ROW_SCREEN/10.0) };// �Ǽ��� ��Ȯ���� �������� ���������� �ִ°� ���� �� ����
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

	//�򰡵� ������ ����
	AllBladeScoreVector = ppg->GetAllBladesCountinusScoresMAX();
}
void GeneticAlgorithm::choice()//�Ű�� ����
{
	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool { 
		if (a.score > b.score)
		{
			return true;
		}
		else if (a.score == b.score)
		{
			if (a.last_distance_from_ball < b.last_distance_from_ball) // ������ ������ �� ����� ���� ������ ���� ������ �� ����
			{
				return true;
			}
		}
		return false;
		});


	//10%�� ����Ʈ ���� �̾� �״�� �ִ´�.
	//2���� ���� ��쿡�� �׳� 2���� �̴´�.
	
	size_t elite_number = ((this->AllAIBladesCount * Elite_Percentage) < 2) ? 2 : (size_t)(this->AllAIBladesCount * Elite_Percentage);

	//����Ʈ�鸸 �̸� �̾� ���� �س��´�. ���� �Ű������ ���ﲨ�� �����̴�. �ƴϸ� Ư�� id�� ���� �����.
	vector<OneDNNWeights> elite_dnn;
	for (size_t i = 0; i < elite_number; ++i) 
	{
		//DNN_Copy�� �Ű�� �ϳ��� DeepCopy �ϴ� �Լ��̴�. �̸� pushback �ؼ� ���� ���븦 ���� ä���.
		//������� push �Ѵ�. ������ ���� ������ top �� ��ġ�Ѵ�.
		elite_dnn.push_back(DNN_Copy(nn[AllBladeScoreVector[i].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount));
	}

	//����Ʈ �ΰ�
	for (size_t i = 0; i < EliteNeuralWeightMatrixVector.size(); ++i)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();
	//�����̹Ƿ� �ϳ��ϳ� �Ҵ� ���� �ϰ� ����
	EliteNeuralWeightMatrixVector = elite_dnn;
	//�����͸� ����Ͽ� �����Ͽ����� �߰��� �Ҵ� ���� �������� Setweight �� �ϸ� �ǹǷ� �޸� ���� ������ ���� �ʾƵ� �ȴ�.
	//������ �߿��� ���� ������ �ΰ�
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
		printf("��� GeneticAlgorithm.cpp::SetDirection\n");
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	}
}
void GeneticAlgorithm::crossover()//�Ű�� ����
{
	//�Ѱ��� �Ű���� ���� ����ġ���� ���� ���̴�.
	MatrixXd* EliteWeightCrossOverForOneNN1 = new MatrixXd[WeightMatrixCount];
	MatrixXd* EliteWeightCrossOverForOneNN2 = new MatrixXd[WeightMatrixCount];
	assert(EliteWeightCrossOverForOneNN1);
	assert(EliteWeightCrossOverForOneNN2);

	//����Ʈ ����ġ ������ ���� ���� vector<MatrixXd*>
	assert (EliteNeuralWeightMatrixVector.size() > 1);
	//����Ʈ ����ġ���� �� ���ؼ� ����� �� ���̴�.
	//����Ʈ ����ġ 1�� -> �������� ��� ����
	size_t elite_count = EliteNeuralWeightMatrixVector.size();

	size_t choice_elite_one_index = rand() % elite_count;
	//����ƮƮ�� 3���̸�
	//3���� �� Ź��ä ������ŭ
	size_t choice_notelite_one_index = rand() %(this->AllAIBladesCount - 1 - elite_count + 1) + elite_count;
	assert(choice_notelite_one_index == this->AllAIBladesCount);
	// a ~ b-1����

	//����Ʈ�� ��� ����
	for (size_t i = 0; i < WeightMatrixCount; ++i) // ����ġ ����� ����
	{
		// ����Ʈ���� �ϳ� �������� ����, ������� �������� ��� ���� ��Ű��
		EliteWeightCrossOverForOneNN1[i] = (EliteNeuralWeightMatrixVector[choice_elite_one_index][i] 
			+ nn[AllBladeScoreVector[choice_notelite_one_index].ID_index].ReturnAllWeightMatrix()[i]) / 2.;
	}

	//����Ʈ�� ����Ʈ�鳢�� ����
	for (size_t i = 0; i < WeightMatrixCount; ++i) // ����ġ ����� ����
	{
		// ����Ʈ���� �ϳ� �������� ����, ������� �������� ��� ���� ��Ű��
		EliteWeightCrossOverForOneNN2[i] = (EliteNeuralWeightMatrixVector[0][i] + EliteNeuralWeightMatrixVector[1][i]) / 2.;
	}

	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN1);
	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN2);
}

void GeneticAlgorithm::mutation()//�Ű�� ����
{

	size_t size_of_elite = EliteNeuralWeightMatrixVector.size();

	OneDNNWeights temp1 = EliteNeuralWeightMatrixVector[size_of_elite - 1];
	OneDNNWeights temp2 = EliteNeuralWeightMatrixVector[size_of_elite - 2];

	// �ڿ��� �Ѱ� �ΰ� �̾Ƽ� (�װ� ���� �Ȱ� �ΰ� �ϱ�)

	for (size_t i = 0; EliteNeuralWeightMatrixVector.size() < AllAIBladesCount; ++i)
	{
		// �����ư��鼭 �ֱ�
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

		//��ǥ �������� ũ�� ���Ӹ� ��� �ݺ�
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
