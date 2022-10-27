#pragma once
#include "Common_Define.h"

typedef MatrixXd* OneDNNWeights;
class FileManage
{
public:
	void Write_OneDNNWeights_IntoFile(int Generation, OneDNNWeights Weights);
	OneDNNWeights Read_OneDNNWeights_FromFile(int Generation);
	OneDNNWeights Read_OneDNNWeights_FromFile();//√÷Ω≈ Generation
	void ReadConfig();
private:
	int GetNewestGeneration();

	int Generation;
	vector<int> NeuralShape;
};

