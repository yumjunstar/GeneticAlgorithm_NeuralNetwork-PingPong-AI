#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	//1��, 2��, �׸��� ������ �� �ϳ� �� 3���� �⺻������ �׻� �Ҵ� �Ǿ������Ƿ� ���̰� �Ͼ���� �ּ� 4���� �Ǿ�� �Ѵ�.
	assert(blades_count >= 4);

	this->Generation = 0;
	this->ds_p = ds;
	this->AllAIBladesCount = blades_count;
	this->ppg = new PingPong(ds, true, blades_count);
	LoopRepeat = 0;

	Output_CMD_Array = new size_t[AllAIBladesCount];
	KeyUp_Binary_CMD = new bool[AllAIBladesCount];
	KeyDown_Binary_CMD = new bool[AllAIBladesCount];

	//�Ű�� ����
	nn = new NeuralNetwork[blades_count];
	for (int i = 0; i < blades_count; i++) {
		nn[i].make_neural_network(NeuralShape);
		nn[i].all_weight_reset_random();
		nn[i].SetActivationFunction("relu");
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
//�ʱ� ���� ���� �Ű�� ����� �� ���븶�� �ʱ�ȭ �ݺ� �ƴϸ� setweight �� �ϵ��� ���߿� �ϳ�
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
	while (GameTries < PerGenerationGameTries)
	{

		GameTries = ppg->GetGameTries();
		ppg->draw_game_layout();
		ppg->monitor_ball();
		ppg->play();

		//���� ��ġ�� ����
		Coor BallCoor = ppg->GetBallCoor();
		Ball_Direction ball_direction = ppg->GetBallDirection();


		double input_arr[9] = { (double)BallCoor.x, (double)BallCoor.y };// �Ǽ��� ��Ȯ���� �������� ���������� �ִ°� ���� �� ����
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

	//�򰡵� ������ ����
	AllBladeScoreVector = ppg->GetAllBladesCountinusScoresMAX();
}
void GeneticAlgorithm::choice()//�Ű�� ����
{
	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool {return a.score > b.score; });


	//10%�� ����Ʈ ���� �̾� �״�� �ִ´�.
	//2���� ���� ��쿡�� �׳� 2���� �̴´�.
	
	int elite_number = ((this->AllAIBladesCount * ChoiceProcessPercentage) < 2) ? 2 : (int)this->AllAIBladesCount * ChoiceProcessPercentage;

	//����Ʈ�鸸 �̸� �̾� ���� �س��´�. ���� �Ű������ ���ﲨ�� �����̴�. �ƴϸ� Ư�� id�� ���� �����.
	vector<OneDNNWeights> elite_dnn;
	for (int i = 0; i < elite_number; i++) 
	{
		elite_dnn.push_back(DNN_Copy(nn[AllBladeScoreVector[i].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount));
	}

	//����Ʈ �ΰ�
	for (int i = 0; i < EliteNeuralWeightMatrixVector.size(); i++)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();
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
	case STOP:
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	default:
		printf("��� GeneticAlgorithm.cpp::play\n");
		KeyUp_Binary_CMD[blade_id] = false;
		KeyDown_Binary_CMD[blade_id] = false;
		break;
	}
}
void GeneticAlgorithm::crossover()//�Ű�� ����
{
	//�Ѱ��� �Ű���� ���� ����ġ���� ���� ���̴�.
	MatrixXd* EliteWeightCrossOverForOneNN = new MatrixXd[WeightMatrixCount];



	//����Ʈ ����ġ ������ ���� ���� vector<MatrixXd*>
	assert (EliteNeuralWeightMatrixVector.size() > 1);
	//����Ʈ ����ġ���� �� ���ؼ� ����� �� ���̴�.
	//����Ʈ ����ġ 1�� -> �������� ��� ����

	for (int i = 0; i < WeightMatrixCount; ++i)
	{
		EliteWeightCrossOverForOneNN[i] = (EliteNeuralWeightMatrixVector[0][i] + EliteNeuralWeightMatrixVector[1][i]) / 2.;
	}

	EliteNeuralWeightMatrixVector.push_back(EliteWeightCrossOverForOneNN);
}

void GeneticAlgorithm::mutation()//�Ű�� ����
{


	OneDNNWeights temp = EliteNeuralWeightMatrixVector.back(); 
	for (int i = 0; EliteNeuralWeightMatrixVector.size() < AllAIBladesCount; i++)
	{
		OneDNNWeights new_temp = AddNormalDistribution(temp);
		EliteNeuralWeightMatrixVector.push_back(new_temp);
	}
	//��� �н��Ϸ��� 4���̻�

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
		//��ǥ �������� ũ�� ���Ӹ� ��� �ݺ�
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