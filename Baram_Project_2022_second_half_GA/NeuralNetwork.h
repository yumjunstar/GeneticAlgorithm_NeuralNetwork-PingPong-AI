#pragma once
#include "Common_Define.h"
class NeuralNetwork
{
private:
	//전체 가중치 묶음
	MatrixXd* matrix_weight_array;
	//전체 가중치 묶음의 개수
	int weight_matrix_count;

	int input_node_count;
	int hidden_layer_count;
	int* each_hidden_node_count;
	int output_node_count;

	static constexpr int DefaultLayerNodeCount[] = {5, 3, 3};
	static constexpr int DefaultLayerCount = 5;
public:
	NeuralNetwork();
	NeuralNetwork(const int each_layer_node_count[], int all_layer_count);
	~NeuralNetwork();

	//신경망을 만드는 함수
	void make_neural_network();
	void make_neural_network(const int each_layer_node_count[], int all_layer_count);

	//신경망에 있는 가중치 값을 설정해주는 함수 단, 현재 신경망의 노드의 개수 크기가 같아야 한다.
	//저장된 값을 불러와서 설정할때 그리고 가중치 값을 업데이트 할때 사용한다.
	void set_weight(MatrixXd value[], int size);

	//모든 가중치 값을 -1과 1사이의 랜덤으로 초기화 할때 사용
	void all_weight_reset_random();

	//precondition: input_arr이 입력값이다, size는 input_node_count와 같아야 한다.
	//postcondition: 가장 활성도가 높은 노드의 위치를 나타낸다. output_node_count 3이면 0, 1, 2중 값 반환
	//활성화 함수
	size_t query(double input_arr[], const int size);


	MatrixXd activation_function(MatrixXd input_array, string function_name) const;
	static double sigmoid(double x);
	static double relu(double x);
	static size_t max_node_index(MatrixXd arr);
	void PrintMatrix(MatrixXd matrix) const;
};

