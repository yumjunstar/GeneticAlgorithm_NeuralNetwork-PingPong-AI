#include "NeuralNetwork.h"

//����ġ ���� �ʱ�ȭ �Լ��� �̿��� �ʱ�ȭ ���� �ϰ�
//all weight reset random �Լ��� �� �� �ְ� �Ѵ�.



NeuralNetwork::NeuralNetwork() {
	matrix_weight_array = nullptr;
	each_matrix_cols = nullptr;
	each_matrix_rows = nullptr;
	weight_matrix_count = 0;
	input_node_count = 0;
	hidden_layer_count = 0;
	each_hidden_node_count = nullptr;
	output_node_count = 0;
}

NeuralNetwork::NeuralNetwork(const vector<size_t> each_layer_node_count) : NeuralNetwork() {
	make_neural_network(each_layer_node_count);
}

//����ġ ���� �Լ�

void NeuralNetwork::make_neural_network(const vector<size_t> each_layer_node_count) {
	//������ �Ҵ� �ڿ� �ٽ� ���� �� �� �����Ƿ�
//if (matrix_weight_array != nullptr) delete[] matrix_weight_array;
//if (each_hidden_node_count != nullptr) delete[] each_hidden_node_count;
//if (each_matrix_rows != nullptr) delete[] each_matrix_rows;
//if (each_matrix_cols != nullptr) delete[] each_matrix_cols;

		//�⺻���� Ȱ��ȭ �Լ��� Relu��
	this->ActivationFunction = "relu";

	//each_layer_node_count���� ù��° ���̾�� ������ ���̾�� �Է� ������ ��� ������ �����̴�.
	size_t all_layer_count = each_layer_node_count.size();
	assert(all_layer_count >= (size_t)2);//�ּ��� �Է� ������ ��� ������ �����Ͽ� 2�� �̻��� �Ǿ�� �Ѵ�.

	//����ġ ����� ������ �׻� ��� ���̾��� �������� �Ѱ��� �� ���̴�.
	weight_matrix_count = all_layer_count - 1;
	//�Ҵ� Ȯ��
	// 
	//�Է� ��� * ����ġ ��� = ��� ���
	size_t input_layer_pos = 0;
	size_t output_layer_pos = all_layer_count - 1;
	hidden_layer_count = all_layer_count - 2;

	matrix_weight_array = new MatrixXd[weight_matrix_count];//����ġ ����� ������ŭ �Ҵ�
	assert(matrix_weight_array);

	each_matrix_rows = new size_t[weight_matrix_count];
	each_matrix_cols = new size_t[weight_matrix_count];
	assert(each_matrix_cols);
	assert(each_matrix_cols);


	input_node_count = each_layer_node_count[input_layer_pos];
	output_node_count = each_layer_node_count[output_layer_pos];

	//�������� ������ ������ ���� ������ �� ��
	this->each_hidden_node_count = new size_t[hidden_layer_count];
	assert(this->each_hidden_node_count);
	for (int i = 0; i < hidden_layer_count; i++) {
		assert((i + 1) < each_layer_node_count.size());
		this->each_hidden_node_count[i] = each_layer_node_count[i + 1];
	}



	//�� ����ġ�� ��� ���� ũ��� ���� ��Ģ�� ������. ���� �� ������ �ִ� ����� ����, ���� �� ������ �ִ� ����� ����
	for (int i = 0; i < weight_matrix_count; i++) {
		matrix_weight_array[i].resize(each_layer_node_count[i + 1], each_layer_node_count[i]);//�Էµ� ũ�⿡ �°� ����� ũ�⸦ �ٲ۴�.
		matrix_weight_array[i].setZero();//0���� �ʱ�ȭ
		assert((i + 1) < each_layer_node_count.size());
		each_matrix_rows[i] = each_layer_node_count[i + 1];//matrix_weight_array[i].rows();
		assert(each_matrix_rows[i] >= 0);
		assert((i) < each_layer_node_count.size());
		each_matrix_cols[i] = each_layer_node_count[i];//matrix_weight_array[i].cols();
		assert(each_matrix_cols[i] >= 0);
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
	/*if (matrix_weight_array != nullptr)*/ delete[] matrix_weight_array;
	/*if (each_hidden_node_count != nullptr)*/ delete[] each_hidden_node_count;
	/*if (each_matrix_rows != nullptr)*/ delete[] each_matrix_rows;
	/*if (each_matrix_cols != nullptr)*/ delete[] each_matrix_cols;
	assert(_CrtCheckMemory());
}

void NeuralNetwork::set_weight(MatrixXd* value, size_t size) {
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);//����� ������ ���¿����� ���� �Ǿ�� �Ѵ�.
																				//�̹� �ʱ�ȭ�� ������ �Ϸ��ϰ� weight ���鸸 �ٲٴ� ���̹Ƿ� ���� ������ �޾ƿ� �ʿ䰡 ����.
	assert(weight_matrix_count == size);
	for (int i = 0; i < weight_matrix_count; i++) {
		int input_rows = value[i].rows();
		int input_cols = value[i].cols();
		assert(input_rows == each_matrix_rows[i]);
		assert(input_cols == each_matrix_cols[i]);
		//���ԵǴϱ� �Ȱ��� ����.
		this->matrix_weight_array[i] = value[i];


	}
	assert(_CrtCheckMemory());
}
void NeuralNetwork::all_weight_reset_normal(double Mean, double Sigma)
{
	//�̸� make_neural_network�� ���� ũ�Ⱑ ������ �ڿ� ����ؾ� �Ѵ�.
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	random_device rd;
	mt19937_64 mt(rd());
	normal_distribution<double> d{ Mean,  Sigma };
	for (size_t i = 0; i < weight_matrix_count; i++) {
		for (size_t j = 0; j < matrix_weight_array[i].rows(); ++j)
		{
			for (size_t k = 0; k < matrix_weight_array[i].cols(); ++k)
			{
				this->matrix_weight_array[i](j, k) = d(mt);
			}
		}

	}
	assert(_CrtCheckMemory());
}
void NeuralNetwork::all_weight_reset_random() 
{
	//�̸� make_neural_network�� ���� ũ�Ⱑ ������ �ڿ� ����ؾ� �Ѵ�.
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	for (size_t i = 0; i < weight_matrix_count; i++) {
		this->matrix_weight_array[i].setRandom();
	}
	assert(_CrtCheckMemory());
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
	for (size_t i = 0; i < weight_matrix_count; i++) {
		//���� ��İ��� �ؾ� �ϹǷ� �ݵ�� ���� ���� ���ƾ� �Ѵ�.
		assert(matrix_weight_array[i].cols() == Temp_Input_Matrix.rows());
		//�߰� ��� ���
		MatrixXd Mid_Put_Matrix = matrix_weight_array[i] * Temp_Input_Matrix;

		Output_Matrix = activation_function(Mid_Put_Matrix, this->ActivationFunction);
		//�˾Ƽ� ũ�Ⱑ �ٲ��.
		Temp_Input_Matrix = Output_Matrix;
	}
	//���������� Output_Matrix�� ��� ����� �ȴ�.


	assert(Output_Matrix.cols() == 1);
	assert(Output_Matrix.rows() == output_node_count);
	assert(_CrtCheckMemory());

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
	assert(_CrtCheckMemory());
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
	assert(arr.rows() > 0);
	size_t row_size = arr.rows();
	double max_value = arr(0, 0);
	size_t max_value_index = 0;
	for (size_t i = 0; i < row_size; i++)
	{
		//i�� 1��
		if (arr(i, 0) > max_value) {
			max_value = arr(i, 0);
			max_value_index = i;
		}
	}
	assert(_CrtCheckMemory());
	return max_value_index;
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

MatrixXd* NeuralNetwork::ReturnAllWeightMatrix()
{
	return matrix_weight_array;
}

size_t NeuralNetwork::GetWeightMatrixCount()
{
	return weight_matrix_count;
}

void NeuralNetwork::SetActivationFunction(string function_name)
{
	this->ActivationFunction = function_name;
}
