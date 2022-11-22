#pragma once
#include "Common_Define.h"
class NeuralNetwork
{
private:
	//��ü ����ġ ����
	MatrixXd* matrix_weight_array;
	//��ü ����ġ ������ ����
	size_t weight_matrix_count;

	//�Է� ���� ��� ����
	size_t input_node_count;

	//���� ���� ����
	size_t hidden_layer_count;
	//���� ���� ��� �� ����
	size_t* each_hidden_node_count;

	//��� ���� ��� ����
	size_t output_node_count;

	size_t* each_matrix_rows;
	size_t* each_matrix_cols;
	string ActivationFunction;
public:
	NeuralNetwork();
	NeuralNetwork(const vector<size_t> each_layer_node_count);
	~NeuralNetwork();

	//�Ű���� ����� �Լ�
	void make_neural_network(const vector<size_t> each_layer_node_count);

	//�Ű���� �ִ� ����ġ ���� �������ִ� �Լ� ��, ���� �Ű���� ����� ���� ũ�Ⱑ ���ƾ� �Ѵ�.
	//����� ���� �ҷ��ͼ� �����Ҷ� �׸��� ����ġ ���� ������Ʈ �Ҷ� ����Ѵ�.
	void set_weight(MatrixXd* value, size_t size);

	void all_weight_reset_normal(double Mean, double Sigma);
	//��� ����ġ ���� -1�� 1������ �������� �ʱ�ȭ �Ҷ� ���
	void all_weight_reset_random();

	static double sigmoid(double x);
	static double relu(double x);
	static void OneHotEncoding(double input_arr[], size_t start_index, Ball_Direction dir);
	static size_t max_node_index(MatrixXd arr);

	//precondition: input_arr�� �Է°��̴�, size�� input_node_count�� ���ƾ� �Ѵ�.
	//postcondition: ���� Ȱ������ ���� ����� ��ġ�� ��Ÿ����. output_node_count 3�̸� 0, 1, 2�� �� ��ȯ
	//Ȱ��ȭ �Լ�
	size_t query(double input_arr[], const size_t size);
	MatrixXd activation_function(MatrixXd input_array, string function_name) const;
	MatrixXd* ReturnAllWeightMatrix();
	size_t GetWeightMatrixCount();
	void SetActivationFunction(string function_name);
	void PrintMatrix(MatrixXd matrix) const;
};

