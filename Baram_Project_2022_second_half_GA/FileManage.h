#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
class FileManage
{
public:
	const string StatisticsFileName = "Staticstics";
	const string StatisticsFileExtension = ".csv";

	const string CurrentGenerationSaveFileName = "CurrentGeneration";
	const string CurrentGenerationSaveExtension = ".txt";


	const string TopDNNWeightsSaveFileName = "WeightsSaveFile";
	const string TopDNNWeightsSaveExtension = ".wsvf";

	const string AllDNNWeightsSaveFileName = "WeightsSaveFile_Specific";
	const string AllDNNWeightsSaveExtension = ".txt";
public:
	FileManage();
	void Write_OneDNNWeights_IntoSpecificFile(size_t Generation, size_t id, const OneDNNWeights Weights, size_t WeightsCount, size_t Score);
	void Write_OneDNNWeights_IntoTopFile(size_t Generation, vector<size_t> NeuralShape, const OneDNNWeights Weights, size_t WeightsCount, size_t score);
	OneDNNWeights_Include_Info Read_OneDNNWeights_FromFile(size_t Generation);
	OneDNNWeights_Include_Info Read_OneDNNWeights_FromFile();//√÷Ω≈ Generation
	void WriteCurrentGeneration(size_t Generation);
	size_t ReadCurrentGeneration();
	void ReadConfig();
	void Write_Statistics(size_t Generation, size_t Respawn, size_t Score);

private:
	int Generation;
	//vector<size_t> NeuralShape;
	size_t Created_Time;
};

