#include "FileManage.h"
FileManage::FileManage() 
{
	Created_Time = time(NULL);
	ModifiedFileName = StatisticsFileName + "_" + to_string(Created_Time) + StatisticsFileExtension;
	ofstream fout(ModifiedFileName);
	if (fout)
	{
		fout.close();
	}

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

void FileManage::Write_Statistics(size_t Generation, size_t Respawn, size_t Score)
{
	assert(_CrtCheckMemory());
	ofstream fout(ModifiedFileName, std::ios_base::out | std::ios_base::app);
	assert(fout);

	fout << Generation <<","<<Respawn<<","<< Score << endl;
	fout.close();
	assert(_CrtCheckMemory());
}