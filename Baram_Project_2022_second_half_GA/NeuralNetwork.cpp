#include "NeuralNetwork.h"

//����ġ ���� �ʱ�ȭ �Լ��� �̿��� �ʱ�ȭ ���� �ϰ�
//all weight reset random �Լ��� �� �� �ְ� �Ѵ�.



NeuralNetwork::NeuralNetwork() {
	matrix_weight_array = nullptr;
	weight_matrix_count = 0;
	input_node_count = 0;
	hidden_layer_count = 0;
	each_hidden_node_count = nullptr;
	output_node_count = 0;
}

NeuralNetwork::NeuralNetwork(const int each_layer_node_count[], const int all_layer_count) {
	make_neural_network(each_layer_node_count, all_layer_count);
}

//����ġ ���� �Լ�

void NeuralNetwork::make_neural_network()
{
	make_neural_network(DefaultLayerNodeCount, DefaultLayerCount);
}

void NeuralNetwork::make_neural_network(const int each_layer_node_count[], const int all_layer_count) {
	//each_layer_node_count���� ù��° ���̾�� ������ ���̾�� �Է� ������ ��� ������ �����̴�.
	assert(all_layer_count >= 2);//�ּ��� �Է� ������ ��� ������ �����Ͽ� 2�� �̻��� �Ǿ�� �Ѵ�.


								 //������ �Ҵ� �ڿ� �ٽ� ���� �� �� �����Ƿ�
	if (matrix_weight_array != nullptr) delete[] matrix_weight_array;
	if (each_hidden_node_count != nullptr) delete[] each_hidden_node_count;


	//�Է� ��� * ����ġ ��� = ��� ���
	int input_layer_pos = 0;
	int output_layer_pos = all_layer_count - 1;
	hidden_layer_count = all_layer_count - 2;

	input_node_count = each_layer_node_count[input_layer_pos];
	output_node_count = each_layer_node_count[output_layer_pos];
	//�������� ������ ������ ���� ������ �� ��
	each_hidden_node_count = new int[hidden_layer_count];
	for (int i = 0; i < hidden_layer_count; i++) {
		each_hidden_node_count[i] = each_layer_node_count[i + 1];
	}


	//����ġ ����� ������ �׻� ��� ���̾��� �������� �Ѱ��� �� ���̴�.
	weight_matrix_count = all_layer_count - 1;
	matrix_weight_array = new MatrixXd[weight_matrix_count];//����ġ ����� ������ŭ �Ҵ�


															//�� ����ġ�� ��� ���� ũ��� ���� ��Ģ�� ������. ���� �� ������ �ִ� ����� ����, ���� �� ������ �ִ� ����� ����
	for (int i = 0; i < weight_matrix_count; i++) {
		matrix_weight_array[i].resize(each_layer_node_count[i + 1], each_layer_node_count[i]);//�Էµ� ũ�⿡ �°� ����� ũ�⸦ �ٲ۴�.
		matrix_weight_array[i].setZero();//0���� �ʱ�ȭ
	}

	// ó���� �Է� ����� ���� 5�� ���� 1���̴�. 5x1
	// W2x5 * I5x1 = O2x1
	// ���� ���� ���� ����� ������ �����ϰ� ���� �Է��� ����� ���� ����
	// ���� ù��° hidden ��尡 3����
	// W3x5 �� �Ǿ�� �Ѵ�. �׷� O3x1 �� �ڿ�
	// 
	// ��� ��ĵ� �� 2�� �� 1��
}

NeuralNetwork::~NeuralNetwork() {
	if (matrix_weight_array != nullptr) delete[] matrix_weight_array;
	if (each_hidden_node_count != nullptr) delete[] each_hidden_node_count;
}

void NeuralNetwork::set_weight(MatrixXd value[], int size) {
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);//����� ������ ���¿����� ���� �Ǿ�� �Ѵ�.
																				//�̹� �ʱ�ȭ�� ������ �Ϸ��ϰ� weight ���鸸 �ٲٴ� ���̹Ƿ� ���� ������ �޾ƿ� �ʿ䰡 ����.
	assert(weight_matrix_count == size);
	for (int i = 0; i < weight_matrix_count; i++) {
		int input_rows = value[i].rows();
		int input_cols = value[i].cols();
		int origin_rows = this->matrix_weight_array[i].rows();
		int origin_cols = this->matrix_weight_array[i].cols();
		assert(input_rows == origin_rows);
		assert(input_cols == origin_cols);
		this->matrix_weight_array[i] = value[i];

	}
}

void NeuralNetwork::all_weight_reset_random() 
{
	//�̸� make_neural_network�� ���� ũ�Ⱑ ������ �ڿ� ����ؾ� �Ѵ�.
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	for (int i = 0; i < weight_matrix_count; i++) {
		this->matrix_weight_array[i].setRandom();
	}
}

size_t NeuralNetwork::query(double input_arr[], const int size) 
{
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	assert(input_node_count == size);
	MatrixXd Input_Matrix;
	Input_Matrix.resize(size, 1);//���� size, ���� 1���̴�.
	for (int i = 0; i < size; i++) {
		Input_Matrix(i, 0) = input_arr[i];

	}
	//���� ����
	//Logic
	//X = W*I
	MatrixXd Temp_Input_Matrix = Input_Matrix;



	MatrixXd Output_Matrix;
	//ó������ �Է°��� ����ġ�� ���ϰ� �� �ڿ� �ٽ� ������ ���� ���ϰ� �̷������� ���� �ȴ�.
	for (int i = 0; i < weight_matrix_count; i++) {
		//���� ��İ��� �ؾ� �ϹǷ� �ݵ�� ���� ���� ���ƾ� �Ѵ�.
		assert(matrix_weight_array[i].cols() == Temp_Input_Matrix.rows());
		//�߰� ��� ���
		MatrixXd Mid_Put_Matrix = matrix_weight_array[i] * Temp_Input_Matrix;

		Output_Matrix = activation_function(Mid_Put_Matrix, "sigmoid");
		//�˾Ƽ� ũ�Ⱑ �ٲ��.
		Temp_Input_Matrix = Output_Matrix;
	}
	//���������� Output_Matrix�� ��� ����� �ȴ�.


	assert(Output_Matrix.cols() == 1);
	assert(Output_Matrix.rows() == output_node_count);

	return max_node_index(Output_Matrix);
}

MatrixXd NeuralNetwork::activation_function(MatrixXd input_array, string function_name) const
{
	int r = input_array.rows();
	int c = input_array.cols();
	MatrixXd return_matrix;
	return_matrix.resize(r, c);
	for (int i = 0; i < r; i++) {//��
		for (int j = 0; j < c; j++) {//��
			if (function_name == "sigmoid") {
				return_matrix(i, j) = sigmoid(input_array(i, j));
			}
			else if (function_name == "relu") {
				return_matrix(i, j) = relu(input_array(i, j));
			}

		}
	}
	return return_matrix;
}

double NeuralNetwork::sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double NeuralNetwork::relu(double x)
{
	return max(0, x);
}
size_t NeuralNetwork::max_node_index(MatrixXd arr)
{
	//���� 1������ �Ѵ�. �׸��� ���� ������
	assert(arr.cols() == 1);
	size_t row_size = arr.rows();
	double max_value = -1;
	int max_value_index = -1;
	for (int i = 0; i < row_size; i++)
	{
		//i�� 1��
		if (arr(i, 0) > max_value) {
			max_value = arr(i, 0);
			max_value_index = i;
		}
	}
	return (size_t)max_value_index;
}

void NeuralNetwork::PrintMatrix(MatrixXd matrix) const
{
	for (int j = 0; j < matrix.rows(); j++)
	{
		for (int k = 0; k < matrix.cols(); k++)
		{
			printf("%lf ", matrix(j, k));
		}
		printf("\n");
	}
	printf("\n");
}
