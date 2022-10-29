#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

typedef MatrixXd* OneDNNWeights;
class FileManage
{
public:
	const string StatisticsFileName = "Staticstics";
	const string StatisticsFileExtension = ".csv";
	const string CurrentGenerationSaveFileNameAndExtension = "CurrentGeneration.txt";
	const string DNNWeightsSaveFileName = "WeightSaveFile";
public:
	FileManage();
	void Write_OneDNNWeights_IntoFile(size_t Generation, size_t id, OneDNNWeights Weights, size_t WeightsCount, size_t Score);
	OneDNNWeights Read_OneDNNWeights_FromFile(size_t Generation);
	OneDNNWeights Read_OneDNNWeights_FromFile();//√÷Ω≈ Generation
	void WriteCurrentGeneration(size_t Generation);
	size_t GetNewestGeneration();
	void ReadConfig();
	void Write_Statistics(size_t Generation, size_t Respawn, size_t Score);

private:
	int Generation;
	vector<size_t> NeuralShape;
	size_t Created_Time;
};

