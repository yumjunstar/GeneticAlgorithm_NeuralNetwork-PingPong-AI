#include "NeuralNetwork.h"

//가중치 갯수 초기화 함수를 이용해 초기화 먼저 하고
//all weight reset random 함수를 쓸 수 있게 한다.



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

//가중치 적용 함수

void NeuralNetwork::make_neural_network(const vector<size_t> each_layer_node_count) {
	//실행후 할당 뒤에 다시 실행 할 수 있으므로
//if (matrix_weight_array != nullptr) delete[] matrix_weight_array;
//if (each_hidden_node_count != nullptr) delete[] each_hidden_node_count;
//if (each_matrix_rows != nullptr) delete[] each_matrix_rows;
//if (each_matrix_cols != nullptr) delete[] each_matrix_cols;

		//기본적인 활성화 함수는 Relu로
	this->ActivationFunction = "relu";

	//each_layer_node_count에서 첫번째 레이어와 마지막 레이어는 입력 계층과 출력 계층의 개수이다.
	size_t all_layer_count = each_layer_node_count.size();
	assert(all_layer_count >= (size_t)2);//최소한 입력 계층과 출력 계층을 포함하여 2개 이상은 되어야 한다.

	//가중치 행렬의 개수는 항상 모든 레이어의 개수에서 한개를 뺀 값이다.
	weight_matrix_count = all_layer_count - 1;
	//할당 확인
	// 
	//입력 행렬 * 가중치 행렬 = 출력 행렬
	size_t input_layer_pos = 0;
	size_t output_layer_pos = all_layer_count - 1;
	hidden_layer_count = all_layer_count - 2;

	matrix_weight_array = new MatrixXd[weight_matrix_count];//가중치 행렬의 개수만큼 할당
	assert(matrix_weight_array);

	each_matrix_rows = new size_t[weight_matrix_count];
	each_matrix_cols = new size_t[weight_matrix_count];
	assert(each_matrix_cols);
	assert(each_matrix_cols);


	input_node_count = each_layer_node_count[input_layer_pos];
	output_node_count = each_layer_node_count[output_layer_pos];

	//은닉층의 노드들의 개수만 따로 정리해 둔 것
	this->each_hidden_node_count = new size_t[hidden_layer_count];
	assert(this->each_hidden_node_count);
	for (size_t i = 0; i < hidden_layer_count; ++i) {
		assert((i + 1) < each_layer_node_count.size());
		this->each_hidden_node_count[i] = each_layer_node_count[i + 1];
	}



	//각 가중치의 행과 열의 크기는 다음 규칙을 따른다. 행은 뒤 계층에 있는 노드의 개수, 열은 앞 계층에 있는 노드의 개수
	for (size_t i = 0; i < weight_matrix_count; ++i) {
		matrix_weight_array[i].resize(each_layer_node_count[i + 1], each_layer_node_count[i]);//입력된 크기에 맞게 행렬의 크기를 바꾼다.
		matrix_weight_array[i].setZero();//0으로 초기화
		assert((i + 1) < each_layer_node_count.size());
		each_matrix_rows[i] = each_layer_node_count[i + 1];//matrix_weight_array[i].rows();
		assert(each_matrix_rows[i] >= 0);
		assert((i) < each_layer_node_count.size());
		each_matrix_cols[i] = each_layer_node_count[i];//matrix_weight_array[i].cols();
		assert(each_matrix_cols[i] >= 0);
	}
	// 처음에 입력 행렬은 행이 5개 열이 1개이다. 5x1
	// W2x5 * I5x1 = O2x1
	// 행은 다음 것의 노드의 개수를 참고하고 열은 입력의 노드의 개수 참고
	// 만약 첫번째 hidden 노드가 3개면
	// W3x5 가 되어야 한다. 그럼 O3x1 그 뒤에
	// 
	// 출력 행렬도 행 2개 열 1개
}

NeuralNetwork::~NeuralNetwork() {
	/*if (matrix_weight_array != nullptr)*/ delete[] matrix_weight_array;
	/*if (each_hidden_node_count != nullptr)*/ delete[] each_hidden_node_count;
	/*if (each_matrix_rows != nullptr)*/ delete[] each_matrix_rows;
	/*if (each_matrix_cols != nullptr)*/ delete[] each_matrix_cols;
	assert(_CrtCheckMemory());
}

void NeuralNetwork::set_weight(MatrixXd* value, size_t size) {
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);//행렬이 설정된 상태에서만 진행 되어야 한다.
																				//이미 초기화때 설정을 완료하고 weight 값들만 바꾸는 것이므로 따로 개수를 받아올 필요가 없다.
	assert(weight_matrix_count == size);
	for (size_t i = 0; i < weight_matrix_count; ++i) {
		int input_rows = value[i].rows();
		int input_cols = value[i].cols();
		assert(input_rows == each_matrix_rows[i]);
		assert(input_cols == each_matrix_cols[i]);
		//대입되니깐 똑같아 진다.
		this->matrix_weight_array[i] = value[i];


	}
	assert(_CrtCheckMemory());
}
void NeuralNetwork::all_weight_reset_normal(double Mean, double Sigma)
{
	//미리 make_neural_network를 통해 크기가 정해진 뒤에 사용해야 한다.
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	random_device rd;
	mt19937_64 mt(rd());
	normal_distribution<double> d{ Mean,  Sigma };
	for (size_t i = 0; i < weight_matrix_count; ++i) {
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
	//미리 make_neural_network를 통해 크기가 정해진 뒤에 사용해야 한다.
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	for (size_t i = 0; i < weight_matrix_count; ++i) {
		this->matrix_weight_array[i].setRandom();
	}
	assert(_CrtCheckMemory());
}

size_t NeuralNetwork::query(double input_arr[], const size_t size) 
{
	assert(matrix_weight_array != nullptr && each_hidden_node_count != nullptr);
	assert(input_node_count == size);
	MatrixXd Input_Matrix;
	Input_Matrix.resize(size, 1);//행이 size, 열이 1개이다.
	for (size_t i = 0; i < size; ++i) {

		Input_Matrix(i, 0) = input_arr[i];

	}
	//복사 과정
	//Logic
	//X = W*I
	MatrixXd Temp_Input_Matrix = Input_Matrix;



	MatrixXd Output_Matrix;
	//처음부터 입력값과 가중치를 곱하고 그 뒤에 다시 노드들의 값을 곱하고 이런식으로 진행 된다.
	for (size_t i = 0; i < weight_matrix_count; ++i) {
		//서로 행렬곱을 해야 하므로 반드시 열과 행이 같아야 한다.
		assert(matrix_weight_array[i].cols() == Temp_Input_Matrix.rows());
		//중간 출력 행렬
		MatrixXd Mid_Put_Matrix = matrix_weight_array[i] * Temp_Input_Matrix;

		Output_Matrix = activation_function(Mid_Put_Matrix, this->ActivationFunction);
		//알아서 크기가 바뀐다.
		Temp_Input_Matrix = Output_Matrix;
	}
	//최종적으로 Output_Matrix가 출력 행렬이 된다.


	assert(Output_Matrix.cols() == 1);
	assert(Output_Matrix.rows() == output_node_count);
	assert(_CrtCheckMemory());

	return max_node_index(Output_Matrix);
}

MatrixXd NeuralNetwork::activation_function(MatrixXd input_array, string function_name) const
{
	size_t r = input_array.rows();
	size_t c = input_array.cols();
	MatrixXd return_matrix;
	return_matrix.resize(r, c);
	for (size_t i = 0; i < r; ++i) {//행
		for (size_t j = 0; j < c; ++j) {//열
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
	//열이 1개여야 한다. 그리고 행이 여러게
	assert(arr.cols() == 1);
	assert(arr.rows() > 0);
	size_t row_size = arr.rows();
	double max_value = arr(0, 0);
	size_t max_value_index = 0;
	for (size_t i = 0; i < row_size; ++i)
	{
		//i행 1열
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
	for (size_t j = 0; j < matrix.rows(); ++j)
	{
		for (size_t k = 0; k < matrix.cols(); ++k)
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

void NeuralNetwork::OneHotEncoding(double input_arr[], size_t start_index, Ball_Direction dir)
{
	size_t end_index = start_index + DIRECTION_COUNT;
	for (size_t i = start_index; i < end_index; ++i)
	{
		input_arr[i] = 0;
	}
	switch (dir)
	{
	case Ball_Direction::STOP:
		break;
	case LEFT:
		input_arr[start_index + LEFT - 1] = 9.99;//SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;
	case UPLEFT:
		input_arr[start_index + UPLEFT - 1] = 9.99;//SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;
	case DOWNLEFT:
		input_arr[start_index + DOWNLEFT - 1] = 9.99;//SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case RIGHT:
		input_arr[start_index + RIGHT - 1] = 9.99;//SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case UPRIGHT:
		input_arr[start_index + UPRIGHT - 1] = 9.99;//SIZE_OF_COL_SCREEN; // 1 * MultipleNumberForNNInput;
		break;
	case DOWNRIGHT:
		input_arr[start_index + DOWNRIGHT - 1] = 9.99;//SIZE_OF_COL_SCREEN; //1 * MultipleNumberForNNInput;
		break;

	}
}