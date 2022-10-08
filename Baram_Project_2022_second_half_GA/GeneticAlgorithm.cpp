#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(DrawScreen* ds, size_t blades_count) 
{
	this->ds_p = ds;
	this->Generation = 0;
	this->ppg = new PingPong(ds, true, blades_count, this->Generation);
	
	//ó���� ���� ����
	init(blades_count);//�Ű�� �����
}
GeneticAlgorithm::~GeneticAlgorithm()
{
	delete[] nn;
	delete ppg;
}
//�ʱ� ���� ���� �Ű�� ����� �� ���븶�� �ʱ�ȭ �ݺ� �ƴϸ� setweight �� �ϵ��� ���߿� �ϳ�
void GeneticAlgorithm::init(size_t blades_count)
{
	if (nn) delete[] nn;
	nn = new NeuralNetwork[blades_count];
	for (int i = 0; i < blades_count; i++) {
		int neural[] = { 5, 3 };
		nn[i].make_neural_network(neural, 2);
		nn[i].all_weight_reset_random();
	}

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
	size_t* Output_CMD_Array = new size_t[all_blades_count];

	bool* KeyUp_Binary_CMD = new bool[all_blades_count];
	bool* KeyDown_Binary_CMD = new bool[all_blades_count];
	while (ppg->GetGameTries() <= repeat)
	{
		//Sleep(100);
		ppg->draw_game_layout();
		ppg->play();
		ppg->monitor_ball();


		AllBladeCoor = ppg->GetAllRightAIBladeCoor();
		BallCoor = ppg->GetBallCoor();
		Ball_Direction ball_direction = ppg->GetBallDirection();

		double ball_x = BallCoor.x / (double)SIZE_OF_COL_SCREEN;
		double ball_y = BallCoor.y / (double)SIZE_OF_ROW_SCREEN;
		double ball_dir = (int)ball_direction / (double)DIRECTION_COUNT;


		for (int i = 0; i < all_blades_count; i++)
		{
			double blade_x = AllBladeCoor[i].x / (double) SIZE_OF_COL_SCREEN;
			double blade_y = AllBladeCoor[i].y / (double) SIZE_OF_ROW_SCREEN;

			double input_arr[5] = { ball_x, ball_y, ball_direction, blade_x, blade_y };

			Output_CMD_Array[i] = nn[i].query(input_arr, 5);
		}

		for (int i = 0; i < all_blades_count; i++) 
		{
			switch ((NNOUT_DIRECTION)Output_CMD_Array[i])
			{
			case UP:
				KeyUp_Binary_CMD[i] = true;
				KeyDown_Binary_CMD[i] = false;
				break;
			case DOWN:
				KeyUp_Binary_CMD[i] = false;
				KeyDown_Binary_CMD[i] = true;
				break;
			case STOP:
				KeyUp_Binary_CMD[i] = false;
				KeyDown_Binary_CMD[i] = false;
				break;
			default:
				printf("��� GeneticAlgorithm.cpp::play\n");
				KeyUp_Binary_CMD[i] = false;
				KeyDown_Binary_CMD[i] = false;
				break;
			}
		}
		ppg->key_up(KeyUp_Binary_CMD, all_blades_count);
		ppg->key_down(KeyDown_Binary_CMD, all_blades_count);
	}
	AllBladeScore = ppg->GetAllBladesScores();
	delete[] Output_CMD_Array;
	delete[] KeyDown_Binary_CMD;
	delete[] KeyUp_Binary_CMD;
}
void GeneticAlgorithm::choice()//�Ű�� ����
{
	//������ �߿��� ���� ������ �ΰ�
}
void GeneticAlgorithm::crossover()//�Ű�� ����
{
	
}
void GeneticAlgorithm::mutation()//�Ű�� ����
{

}
void GeneticAlgorithm::apply()
{

}
void GeneticAlgorithm::LetsLearn()
{
	while (this->Generation <= 1000)
	{
		ppg->Reset();
		play();
		choice();
		crossover();
		mutation();
		apply();
		ppg->SetGeneration(++Generation);

	}

}