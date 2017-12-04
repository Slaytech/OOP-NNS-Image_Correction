#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <windows.h>
#include "Matrix.h"

using namespace std;

double* readTXT(char *fileName, int sizeR, int sizeC);
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q);

int main()
{
	int M = 512; int N = 512; //Data used for row and column values
	double* input_data = 0;
	double* comparison_data = 0;
	double SSDtotal = 0;
	string input;

	cout << endl;
	cout << " Data being read from text file -------------------------------------------" << endl;

	char* inputFileName = "shuffled_logo.txt"; 
	input_data = readTXT(inputFileName, M, N);

	inputFileName = "unshuffled_logo_noisy.txt";
	comparison_data = readTXT(inputFileName, M, N);

	double* placehold_data = new double[32 * 32];


	for (int i = 0; i < 262144; i++) // Used to filter noise out of the Noisy unshuffled image
	{
		if (comparison_data[i] >= 250)
		{
			comparison_data[i] = 150;
		}
	}

	matrix input_shuffled(512, 512, input_data);
	matrix input_noisy_unshuffled(512, 512, comparison_data);
	matrix matrix_fixed(512, 512, input_data);
	matrix tempSubMatrix(32, 32, placehold_data);
	matrix comparison, shuffled, SSD_Dif, SSD_Squ = tempSubMatrix; //Sets default values for matrixes used in the SSD algorithm
	
	delete[] placehold_data;  //Memory management

	for (int x = 0; x < 512; x += 32)
	{
		for (int y = 0; y < 512; y += 32) //Two nested For loop used to target every part of 2d array
		{
			double replacement = 512 * 512 * 32;
			comparison = input_noisy_unshuffled.getBlock(x, (x + 32), y, (y + 32)); //Gets block position data for noisy image
			
			for (int i = 0; i < 512; i += 32) //Two nested For loop used to target block position
			{
				for (int j = 0; j < 512; j += 32)
				{
					shuffled = input_shuffled.getBlock(i, (i + 32), j, (j + 32)); //Gets block position data for unsorted image

					SSD_Dif = comparison - shuffled;
					SSD_Squ = SSD_Dif * SSD_Dif;
					SSDtotal = SSD_Squ.sum(); //Sum of Squared difference calculation

					while (SSDtotal < replacement) //Active only when the Sum of Squared difference is lower than default, and subsequent SSD values
					{
						int Blockx = x;

						for (int a = 0; a < 32; a++)
						{
							int Blocky = y;

							for (int b = 0; b < 32; b++)
							{
								double val = shuffled.get(a, b);
								matrix_fixed.set(Blockx, Blocky, val); //Used to set the new corrected block position values
								Blocky++;
								replacement = SSDtotal;			//Sets replacement equal to the new lowest SSD value					
							}
							Blockx++;
						}
					}
				}
			}
		}
	}

	cout << endl;
	cout << " Data being written to .pgm format ----------------------------------------" << endl;
	Sleep(1000); //Pauses process to allow user to read status display

	int Q = 255;
	char* outputFileName = "Logo_Unshuffled.pgm";
	WritePGM(outputFileName, matrix_fixed.getData(), M, N, Q); //Writes corrected 2d array into .pgm format
	
	outputFileName = "Logo_Shuffled.pgm";
	WritePGM(outputFileName, input_data, M, N, Q);

	outputFileName = "Noisy_Logo_improved.pgm";
	WritePGM(outputFileName, comparison_data, M, N, Q);

	delete[] input_data; //memory management
	delete[] comparison_data;	

	cout << endl;
	cout << " Operation complete, please press enter to exit ---------------------------" << endl;
	getline(cin, input); //Waits for user input before program terminates
	return 0;
}

double* readTXT(char *fileName, int sizeR, int sizeC) //Method function used to read the text files
{
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{

		while (myfile.good())
		{
			if (i>sizeR*sizeC - 1) break;
			myfile >> *(data + i);
			i++;
		}
		myfile.close();
	}

	else cout << "Unable to open file";

	return data;
}

void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q) //Method function used to write the .pgm files
{

	int i;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << sizeC << " " << sizeR << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;

}