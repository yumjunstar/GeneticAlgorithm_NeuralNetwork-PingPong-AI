#pragma once
#include "Common_Define.h"
class NeuralNetwork
{
private:
	//전체 가중치 묶음
	MatrixXd* matrix_weight_array;
	//전체 가중치 묶음의 개수
	size_t weight_matrix_count;

	//입력 계층 노드 개수
	size_t input_node_count;

	//은닉 계층 개수
	size_t hidden_layer_count;
	//은닉 계층 노드 각 개수
	size_t* each_hidden_node_count;

	//출력 계층 노드 개수
	size_t output_node_count;

	size_t* each_matrix_rows;
	size_t* each_matrix_cols;
	string ActivationFunction;
public:
	NeuralNetwork();
	NeuralNetwork(const vector<size_t> each_layer_node_count);
	~NeuralNetwork();

	//신경망을 만드는 함수
	void make_neural_network(const vector<size_t> each_layer_node_count);

	//신경망에 있는 가중치 값을 설정해주는 함수 단, 현재 신경망의 노드의 개수 크기가 같아야 한다.
	//저장된 값을 불러와서 설정할때 그리고 가중치 값을 업데이트 할때 사용한다.
	void set_weight(MatrixXd* value, size_t size);

	void all_weight_reset_normal(double Mean, double Sigma);
	//모든 가중치 값을 -1과 1사이의 랜덤으로 초기화 할때 사용
	void all_weight_reset_random();

	static double sigmoid(double x);
	static double relu(double x);
	static void OneHotEncoding(double input_arr[], size_t start_index, Ball_Direction dir);
	static size_t max_node_index(MatrixXd arr);

	//precondition: input_arr이 입력값이다, size는 input_node_count와 같아야 한다.
	//postcondition: 가장 활성도가 높은 노드의 위치를 나타낸다. output_node_count 3이면 0, 1, 2중 값 반환
	//활성화 함수
	size_t query(double input_arr[], const size_t size);
	MatrixXd activation_function(MatrixXd input_array, string function_name) const;
	MatrixXd* ReturnAllWeightMatrix();
	size_t GetWeightMatrixCount();
	void SetActivationFunction(string function_name);
	void PrintMatrix(MatrixXd matrix) const;
};

