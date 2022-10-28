#include "FileManage.h"
FileManage::FileManage() 
{
	ofstream fout(StatisticsFileName);
	fout.close();
}
void FileManage::Write_OneDNNWeights_IntoFile(int Generation, OneDNNWeights Weights)
{

}

OneDNNWeights FileManage::Read_OneDNNWeights_FromFile(int Generation)
{
	return OneDNNWeights();
}

OneDNNWeights FileManage::Read_OneDNNWeights_FromFile()
{
	return OneDNNWeights();
}

void FileManage::Write_Statistics(size_t Generation, size_t Score)
{

	ofstream fout(StatisticsFileName, std::ios_base::out | std::ios_base::app);
	assert(fout);

	fout << Generation <<" "<< Score << endl;
	fout.close();
}