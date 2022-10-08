#pragma once
#include "Common_Define.h"
class NeuralNetwork
{
private:
	//��ü ����ġ ����
	MatrixXd* matrix_weight_array;
	//��ü ����ġ ������ ����
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

	//�Ű���� ����� �Լ�
	void make_neural_network();
	void make_neural_network(const int each_layer_node_count[], int all_layer_count);

	//�Ű���� �ִ� ����ġ ���� �������ִ� �Լ� ��, ���� �Ű���� ����� ���� ũ�Ⱑ ���ƾ� �Ѵ�.
	//����� ���� �ҷ��ͼ� �����Ҷ� �׸��� ����ġ ���� ������Ʈ �Ҷ� ����Ѵ�.
	void set_weight(MatrixXd value[], int size);

	//��� ����ġ ���� -1�� 1������ �������� �ʱ�ȭ �Ҷ� ���
	void all_weight_reset_random();

	//precondition: input_arr�� �Է°��̴�, size�� input_node_count�� ���ƾ� �Ѵ�.
	//postcondition: ���� Ȱ������ ���� ����� ��ġ�� ��Ÿ����. output_node_count 3�̸� 0, 1, 2�� �� ��ȯ
	//Ȱ��ȭ �Լ�
	size_t query(double input_arr[], const int size);


	MatrixXd activation_function(MatrixXd input_array, string function_name) const;
	static double sigmoid(double x);
	static double relu(double x);
	static size_t max_node_index(MatrixXd arr);
	void PrintMatrix(MatrixXd matrix) const;
};

