#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define N_DATA_POINTS 400
#define N_CHANNELS 14


#define C_BIN_SIZE 10 //Samples
#define C_STD_SIZE 20 //Samples

double stdev(double vals[],int n);

int Classifier(double data[N_DATA_POINTS*(N_CHANNELS+1)]) 
{
	double binedData[N_DATA_POINTS / C_BIN_SIZE][N_CHANNELS]; //Step1
	double diffData[N_DATA_POINTS / C_BIN_SIZE - 1][N_CHANNELS]; //Step2
	double chanelAverage[N_DATA_POINTS / C_BIN_SIZE - 1]; //Step3
	int i,j,k;

	//Step1 Bin
	//Loop over all chanels
	for (i=0;i<N_CHANNELS;i++)
	{
		//For each bin
		for (j=0;j<N_DATA_POINTS / C_BIN_SIZE;j++)
		{
			binedData[j][i] = 0;
			//Loop on each data point in the bin
			for (k=0;k<C_BIN_SIZE;k++)
				binedData[j][i] += data[(j*C_BIN_SIZE+k)*(N_CHANNELS+1)+i+1] / C_BIN_SIZE;
		}
	}

	//Step2 Diff
	for (i=0;i<N_CHANNELS;i++)
	{
		//For each bin
		for (j=1;j<N_DATA_POINTS / C_BIN_SIZE;j++)
		{
			diffData[j-1][i] = binedData[j][i] - binedData[j-1][i];
		}
	}

	//Average on all chanels
	//For each bin
//	for (j=1;j<N_DATA_POINTS / C_BIN_SIZE;j++)
//	{
//		chanelAverage[j] = 0;
//		for (i=0;i<N_CHANNELS;i++)
//		{
//			chanelAverage[j] += diffData[j][i] / N_CHANNELS;
//		}
//	}
	double s = stdev(chanelAverage,20); 
	std::cout << "std: " << s;
//	if (chanelAverage[(N_DATA_POINTS / C_BIN_SIZE - 1)-1] < (-3.0)*s)
//		return 1;
//	else

		return 0;
}

double stdev(double vals[],int n) 
{
	double m1=0., m2=0.;
	for (int i=0; i<n; i++) m1 += vals[i];
	m1 /= n;
	
	for (int i=0; i<n; i++) m2 += (vals[i]-m1)*(vals[i]-m1);
	m2 /= n; // to account for a bias in the mean, we could divide by n-1

	double sd = sqrt(m2);
	return sd;
}
/*
int main() {
	ifstream myfile;
	char *line;
	double binedData[N_DATA_POINTS / C_BIN_SIZE][N_CHANNELS];
	int channel = 1;
	int sample = 1;

	myfile.open("Text.txt");
	
	if (myfile.is_open()) {
		for (int i = 1; i<N_DATA_POINTS; i++)
			for (int j=1; i<N_CHANNELS) {
				binedData[i][j] = fscanf(myfile, "%f");
		}
	}

	myfile.close();
	return 0;
}*/