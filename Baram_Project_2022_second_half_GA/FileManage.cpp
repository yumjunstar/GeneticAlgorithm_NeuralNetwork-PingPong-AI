#include "FileManage.h"
FileManage::FileManage() 
{
	Created_Time = time(NULL);
	
	Generation = 0;

}
void FileManage::Write_OneDNNWeights_IntoFile(size_t Generation, size_t id, OneDNNWeights Weights, size_t WeightsCount, size_t Score)
{
	string ModifiedFileName = DNNWeightsSaveFileName + "_" + to_string(Generation);
	ofstream fout;
	if (id > 0)
	{
		fout.open(ModifiedFileName, std::ios_base::out | std::ios_base::app);
	}
	else // ¸Ç Ã³À½¿¡´Â µ¤¾î ¾º¿ì±â
	{
		fout.open(ModifiedFileName);
	}


	if (fout)
	{
		fout << "<" << id << ">"<<":" <<Score<<endl;
		for (int i = 0; i < WeightsCount; i++)
		{
			fout << Weights[i] << endl; 
			fout << "#" << endl;
		}
		fout << "!" << endl;
		fout.close();
	}
}

OneDNNWeights FileManage::Read_OneDNNWeights_FromFile(size_t Generation)
{
	return OneDNNWeights();
}

OneDNNWeights FileManage::Read_OneDNNWeights_FromFile()
{
	return Read_OneDNNWeights_FromFile(GetNewestGeneration());
}

void FileManage::WriteCurrentGeneration(size_t Generation)
{
	ofstream fout(CurrentGenerationSaveFileNameAndExtension);
	this->Generation = Generation;
	if (fout)
	{
		fout << Generation;
		fout.close();
	}
}

size_t FileManage::GetNewestGeneration()
{
	if (this->Generation < 0)
	{
		ifstream fin(CurrentGenerationSaveFileNameAndExtension);
		if (fin)
		{
			int gen;
			fin >> gen;
			return gen;
			fin.close();
		}
		else
		{
			return 0;
		}

	}
	else
	{
		return (size_t)Generation;
	}
}

void FileManage::Write_Statistics(size_t Generation, size_t Respawn, size_t Score)
{
	string ModifiedFileName = StatisticsFileName + "_" + to_string(Created_Time) + StatisticsFileExtension;
	assert(_CrtCheckMemory());
	ofstream fout(ModifiedFileName, std::ios_base::out | std::ios_base::app);
	assert(fout);
	size_t Elapsed_Time = time(NULL) - Created_Time;
	fout << Generation <<","<<Respawn<<","<< Score << ","<<Elapsed_Time<<endl;
	fout.close();
	assert(_CrtCheckMemory());
}