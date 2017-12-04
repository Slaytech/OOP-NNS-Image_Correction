#include <iostream>
#include "Matrix.h"

matrix::matrix() //Default constructor
{
	_M = 1;
	_N = 1;
	_data = new double[_M*+_N];

	for (int i = 0; i < _M*_N; i++)
	{
		_data[i] = 0;
	}
}


matrix::matrix(int sizeR, int sizeC, double* input_data)
{
	_M = sizeR;
	_N = sizeC;

	_data = new double[_M*_N];

	for (int i = 0; i < _M*_N; i++)
	{
		_data[i] = input_data[i];
	}
}

matrix matrix::getBlock(int start_row, int end_row, int start_column, int end_column)
{
	int rows = (end_row - start_row); //rows and columns for subimage are set
	int cols = (end_column - start_column);

	double *tempData = new double[rows*cols];

	int count = 0;
	for (int i = start_row; i < end_row; i++)
		for (int j = start_column; j < end_column; j++) //The For loops interate through image data
		{
			tempData[count] = get(i, j);
			count++;
		}
	matrix temp = matrix(rows, cols, tempData);

	delete[] tempData; //memory management
	return temp;
}

const double matrix::get(int i, int j) 
{
	return _data[i * _N + j];
}

const double matrix::get(int i)
{
	return _data[i];
}

double matrix::set(int i, int j, double val) //Used to set new block position data
{
	_data[i * _N + j] = val;
	return _data[i * _N + j];
}

double* matrix::getData() 
{
	return _data;
}

const double matrix::sum() //Used to calculate SSD value from Squared difference
{
	double _sum = 0;
	for (int x = 0; x < (_M*_N); x++)
	{
		_sum += _data[x];
	}

	return _sum;
}

matrix::matrix(const matrix& other) //Copy constructor
{
	_M = other._M;
	_N = other._N;

	_data = new double[_M*_N];

	for (int i = 0; i < _M*_N; i++)
	{
		_data[i] = other._data[i];
	}
}

matrix matrix::operator=(const matrix& other) //Operator overloaded
{
	delete[] _data;
	_M = other._M;
	_N = other._N;

	_data = new double[_M*_N];

	for (int x = 0; x < (_M*_N); x++)
	{
		this->_data[x] = other._data[x];
	}

	return *this;
}

matrix matrix::operator-(const matrix& other) //Operator overloaded
{
	matrix temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] - other._data[x];
	}

	return temp;
}

matrix matrix::operator*(const matrix& other) //Operator overloaded
{
	matrix temp;
	temp._M = other._M;
	temp._N = other._N;

	temp._data = new double[temp._M*temp._N];

	for (int x = 0; x < (temp._M*temp._N); x++)
	{
		temp._data[x] = this->_data[x] * other._data[x];
	}

	return temp;
}

matrix::~matrix() // Used to prevent exception throw
{
	delete[] _data;
}
