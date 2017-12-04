#pragma once

class matrix
{
public:
	matrix(); //Default Constructor
	matrix(int sizeR, int sizeC, double* input_data);
	matrix getBlock(int start_row, int end_row, int start_column, int end_column);
	const double get(int i); //persistent values
	const double get(int i, int j); //persistent values
	double set(int i, int j, double val); 
	double* getData();
	const double sum();

	matrix(const matrix& other); //copy constructor
	matrix operator=(const matrix& other); //operator overloaded
	matrix operator-(const matrix& other);
	matrix operator*(const matrix& other);

	~matrix(); //Destructor

private:
	int _M, _N;
	double *_data;
};
