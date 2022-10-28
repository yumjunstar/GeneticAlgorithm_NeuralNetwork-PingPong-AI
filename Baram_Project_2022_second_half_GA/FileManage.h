#pragma once
#include "Common_Define.h"
#include <iostream>
#include <fstream>
#include <string>
typedef MatrixXd* OneDNNWeights;
class FileManage
{
	const string StatisticsFileName = "Staticstics.txt";
public:
	FileManage();
	void Write_OneDNNWeights_IntoFile(int Generation, OneDNNWeights Weights);
	OneDNNWeights Read_OneDNNWeights_FromFile(int Generation);
	OneDNNWeights Read_OneDNNWeights_FromFile();//√÷Ω≈ Generation
	void ReadConfig();
	void Write_Statistics(size_t Generation, size_t Score);
private:
	vector<size_t> NeuralShape;
};

