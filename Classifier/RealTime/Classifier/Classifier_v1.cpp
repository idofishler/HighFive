#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
using namespace std;

#define N_DATA_POINTS 400
#define N_CHANNELS 14

#define C_BIN_SIZE 5 //Samples
#define C_STD_SIZE 20 //Samples
#define C_STD_LIM 3.0 // STD threshold

double stdev(double vals[],int n);
int ClassifierV3(double data[]);

int ClassifierV1(double data[N_DATA_POINTS*(N_CHANNELS+1)]) 
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
				binedData[j][i] += data[(j*C_BIN_SIZE+k)*(N_CHANNELS+1)+i+1] / ((double)C_BIN_SIZE);
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
	for (j=0;j<(N_DATA_POINTS / C_BIN_SIZE-1);j++)
	{
		chanelAverage[j] = 0;
		for (i=0;i<N_CHANNELS;i++)
		{
			chanelAverage[j] += diffData[j][i] / ((double)N_CHANNELS);
		}
	}
	
	double s = stdev(chanelAverage,C_STD_SIZE); 

	int flag = 0;
	for (i=0;i<(N_DATA_POINTS / C_BIN_SIZE-1);i++)
		flag |= chanelAverage[(N_DATA_POINTS / C_BIN_SIZE - 1)-1] < - C_STD_LIM*s;

	return flag;
}

double stdev(double vals[],int n) 
{
	double m1=0., m2=0.;
	for (int i=0; i<n; i++) m1 += vals[i];
	m1 /= n;
	
	for (int i=0; i<n; i++) m2 += (vals[i]-m1)*(vals[i]-m1);
	m2 /= (n-1); // to account for a bias in the mean, we could divide by n-1

	double sd = sqrt(m2);
	return sd;
}

/*int main() {
	ifstream myfile;
	
	//int array[20];
	int i=0;
	char cNum[256] ;
    //infile.open ("test.txt", ifstream::in);
	myfile.open("C:\\Users\\AviB\\Dropbox\\Projects\\HighFiveProject\\Classifier\\RealTime\\Classifier\\trainSet1_true.txt", ifstream::in);
	double data[N_DATA_POINTS][N_CHANNELS+1];
	int channel = 1;
	int sample = 1;
    if (myfile.is_open()) {
		while (myfile.good()) {
			myfile.getline(cNum, 256, ',');
            data[i/(N_CHANNELS+1)][i%(N_CHANNELS+1)]= atof(cNum) ;
            i++ ;
			if (i==(N_CHANNELS+1)*N_DATA_POINTS)
				break;
        }
		cout << "Classifier returned " << ClassifierV3(data) << endl;
        myfile.close();
	}
    else {
		cout << "Error opening file";
	}

	int result = ClassifierV3((double [])data);
	cout << result;
	
	return 0;
}*/