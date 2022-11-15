#include "FileManage.h"
FileManage::FileManage() 
{
	Created_Time = time(NULL);
	
	Generation = 0;

}
void FileManage::Write_OneDNNWeights_IntoTopFile(size_t Generation, vector<size_t> NeuralShape, const OneDNNWeights Weights, size_t WeightsCount, size_t score)
{
	string ModifiedFileName = TopDNNWeightsSaveFileName + "_" + to_string(Generation) + TopDNNWeightsSaveExtension;
	ofstream fout(ModifiedFileName);
	if (fout)
	{
		fout << WeightsCount << endl;
		size_t Layer_Count = WeightsCount + 1;
		// 각 레이어의 노드 개수 쓰기
		for (size_t i = 0; i < Layer_Count; ++i)
		{
			fout << NeuralShape[i]<<" ";
		}
		fout << endl;

		// 각 가중치 행렬의 행과 열 쓰기
		for (size_t i = 0; i < WeightsCount; ++i)
		{
			fout << Weights[i].rows() << " " << Weights[i].cols() << endl;
		}


		// 가중치 행렬 출력
		for (size_t i = 0; i < WeightsCount; ++i)
		{
			fout << Weights[i] << endl;
		}
		// 점수 출력
		fout << score << endl;
		fout.close();
	}
	else
	{
		cout << "Write_OneDNNWeights_IntoTopFile 에서 파일을 쓸 수 없습니다." << endl;
	}
}
void FileManage::Write_OneDNNWeights_IntoSpecificFile(size_t Generation, size_t id, const OneDNNWeights Weights, size_t WeightsCount, size_t Score, double distance)
{
	string ModifiedFileName = AllDNNWeightsSaveFileName + "_" + to_string(Generation) + AllDNNWeightsSaveExtension;
	ofstream fout;
	if (id > 0)
	{
		fout.open(ModifiedFileName, std::ios_base::out | std::ios_base::app);
	}
	else // 맨 처음에는 덮어 씌우기
	{
		fout.open(ModifiedFileName);
	}


	if (fout)
	{
		fout << "<" << id << ">" << ":" << "Score:" << Score << endl;
		fout<<"Distance:" <<distance << endl;
		for (size_t i = 0; i < WeightsCount; ++i)
		{
			fout << Weights[i] << endl; 
			fout << "#" << endl;
		}
		fout << "!" << endl;
		fout.close();
	}
	else
	{
		cout << "Write_OneDNNWeights_IntoSpecificFile 에서 파일을 쓸 수 없습니다." << endl;
	}
}

OneDNNWeights_Include_Info FileManage::Read_OneDNNWeights_FromFile(size_t Generation)
{
	string ObjectFileName = TopDNNWeightsSaveFileName + "_" + to_string(Generation) + TopDNNWeightsSaveExtension;
	// 벡터 크기
	// 파일에 첫번째는
	// 가중치 행렬의 개수
	// 가중치 행렬의 행, 열
	// 가중치 행렬의 행, 열
	//.
	//.
	//.
	// 가중치 행렬 이어짐
	size_t weight_matrix_count = 0;
	MatrixXd* OneDNNWeights_ReadFromFile;
	size_t* each_matrix_rows;
	size_t* each_matrix_cols;



	ifstream fin(ObjectFileName);
	double micro_value;

	vector <size_t> Read_NeuralShape;
	if (fin)
	{
		// 처음에 가중치 행렬의 개수 구하기
		fin >> weight_matrix_count;
		OneDNNWeights_ReadFromFile = new MatrixXd[weight_matrix_count];
		each_matrix_cols = new size_t[weight_matrix_count];
		each_matrix_rows = new size_t[weight_matrix_count];


		// 각 레이어의 노드 개수 쓰기
		size_t Layer_Count = weight_matrix_count + 1;
		for (size_t i = 0; i < Layer_Count; ++i)
		{
			size_t each_layer_count;
			fin >> each_layer_count;
			Read_NeuralShape.push_back(each_layer_count);
		}

		// 각 가중치 행렬의 행과 열 구하기
		for (size_t i = 0; i < weight_matrix_count; ++i)
		{
			fin >> each_matrix_rows[i] >> each_matrix_cols[i];
		}


		// 가중치 행렬의 값 저장하기
		for (size_t i = 0; i < weight_matrix_count; ++i)
		{
			OneDNNWeights_ReadFromFile[i].resize(each_matrix_rows[i], each_matrix_cols[i]);
			OneDNNWeights_ReadFromFile[i].setZero();
			for (size_t r = 0; r < each_matrix_rows[i]; ++r)
			{
				for (size_t c = 0; c < each_matrix_cols[i]; ++c)
				{
					fin >> micro_value;
					OneDNNWeights_ReadFromFile[i](r, c) = micro_value;
				}
			}
		}
		delete[] each_matrix_cols;
		delete[] each_matrix_rows;
		fin.close();

		// 정보 저장
		OneDNNWeights_Include_Info Return_Value;
		Return_Value.weights = OneDNNWeights_ReadFromFile;
		Return_Value.weights_count = weight_matrix_count;
		Return_Value.NeuralShape = Read_NeuralShape;
		return Return_Value;
	}
	else
	{
		cout << "해당 세대의 파일이 없습니다." << endl;
		return OneDNNWeights_Include_Info();
	}
}

OneDNNWeights_Include_Info FileManage::Read_OneDNNWeights_FromFile()
{
	return Read_OneDNNWeights_FromFile(ReadCurrentGeneration());
}

void FileManage::WriteCurrentGeneration(size_t Generation)
{
	ofstream fout(CurrentGenerationSaveFileName+CurrentGenerationSaveExtension);
	this->Generation = Generation;
	if (fout)
	{
		fout << Generation;
		fout.close();
	}
	else
	{
		cout << "현재 세대 파일 쓰기 실패" << endl;
	}
}

size_t FileManage::ReadCurrentGeneration()
{
	ifstream fin(CurrentGenerationSaveFileName + CurrentGenerationSaveExtension);
	if (fin)
	{
		int gen;
		fin >> gen;
		this->Generation = gen;
		fin.close();
		return gen;

	}
	else
	{
		cout << "현재 세대 파일 읽기 실패" << endl;
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