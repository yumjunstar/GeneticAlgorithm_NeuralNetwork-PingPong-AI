#include <iostream>
#include <fstream>
using namespace std;
int main(void)
{
	ofstream fout("Statics.txt");
	if (fout)
	{
		fout << 100 << 10 << endl;
		fout.close();
	}
	else
	{
		cout << "error" << endl;
	}

}