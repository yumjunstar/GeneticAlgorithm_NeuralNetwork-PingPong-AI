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

	//�Ű�� ����
	nn = new NeuralNetwork[blades_count];
	for (int i = 0; i < blades_count; i++) {
		nn[i].make_neural_network(NeuralShape, NeuralLayerCount);
		nn[i].all_weight_reset_random();
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
	if ((GameTries > 20) || (ppg->GetMaxScoreForLearn() > 15))
	{

		if (ppg->GetAIBladeScore(blade_id) < (size_t)5)
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
void GeneticAlgorithm::play()//���� �����ϰ� ���� �Ű���� ���ؼ� ����
{
	//������ ��� �÷��� �ϰ� �ұ�
	//�� ����� 100�� ���� �÷��� �ϰ� �ϱ�

	//��� Ź��ä���� ���� ����


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
		double ball_x = BallCoor.x / (double)SIZE_OF_COL_SCREEN;
		double ball_y = BallCoor.y / (double)SIZE_OF_ROW_SCREEN;
		double ball_dir = (int)ball_direction / (double)DIRECTION_COUNT;

		//��� Ź��ä���� ��ġ ��ȯ
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

	//�򰡵� ������ ����
	AllBladeScoreVector = ppg->GetAllBladesScores();





}
void GeneticAlgorithm::choice()//�Ű�� ����
{


	sort(AllBladeScoreVector.begin(), AllBladeScoreVector.end(), [](Blade_Info& a, Blade_Info& b)->bool {return a.score > b.score; });
	//���ﲨ�ϱ� �̸� ������ ���´�.
	MatrixXd* first = DNN_Copy(nn[AllBladeScoreVector[0].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount);
	MatrixXd* second = DNN_Copy(nn[AllBladeScoreVector[1].ID_index].ReturnAllWeightMatrix(), WeightMatrixCount);
	//����Ʈ �ΰ�

	for (int i = 0; i < EliteNeuralWeightMatrixVector.size(); i++)
	{
		delete[] EliteNeuralWeightMatrixVector[i];
	}
	EliteNeuralWeightMatrixVector.clear();

	EliteNeuralWeightMatrixVector.push_back(first);
	EliteNeuralWeightMatrixVector.push_back(second);
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
	int EliteCount = EliteNeuralWeightMatrixVector.size();
	assert(EliteCount > 0);
	//����Ʈ ����ġ���� �� ���ؼ� ����� �� ���̴�.
	//����Ʈ ����ġ 1�� -> �������� ��� ����

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

void GeneticAlgorithm::mutation()//�Ű�� ����
{


	OneDNNWeights temp = EliteNeuralWeightMatrixVector.back();
	//�� �ڿ��� �̾Ƽ�
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