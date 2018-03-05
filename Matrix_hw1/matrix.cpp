////////////////////////////////////////////////////////
// ECE 2574, Homework 1, Steven Diamante
//
// File name: matrix.cpp
// Description: Implementation File file for the ADT Matrix
// Date: 09/12/2014
//

#include <iostream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include "matrix.h"
using namespace std;

// Constructor: Make a matrix 
Matrix::Matrix(int row_size, int column_size)
{
	rows = row_size; // set inputs to private data members
	columns = column_size;
	data = new MatrixItem[rows*columns]; // allocating dynamic memory
		for(int i=0; i < rows; i++)
			for(int j=0; j < columns; j++)
				setElement(i, j, i-j); // Call set element to construct basic matrix with values i - j

	// Checking if matrix is valid (row and column size must be greater than 0)
	try
	{
	if(rows < 1)
		throw(rows);
	if(columns < 1)
		throw(columns);
	}
	catch(int error)
	{
		// Output error message if exception is thrown
		cout << "Dimensions must be greater than zero." << endl;
		cout << error << " is not greater than zero." << endl;
	}
}

// Copy constructor: create a deep copy of matrix orig
Matrix::Matrix(const Matrix& orig)
{
	rows = orig.rows; // Set private data members to original matrix dimensions
	columns = orig.columns;
	int size = rows*columns;
	data = new MatrixItem[size];
	for(int i=0; i < rows; i++)
	{
    for(int j=0; j < columns; j++)
		data[i*columns+j] = orig.getElement(i,j); // copy original matrix into data memory
	}
}

// Desructor function
Matrix::~Matrix()
{
	delete[] data;
}

// Set element in matrix 
void Matrix::setElement(int row, int column, MatrixItem value)
{
	data[row*columns+column] = value; // store value in specific index (i,j)
}

// Get Element from Matrix
MatrixItem Matrix::getElement(int row, int column) const
{
	return(*(data+(row*columns+column))); // return the value that is located at (i,j)
}

// Overloaded operator=
Matrix& Matrix::operator= (const Matrix& orig)
{
	 // Check for self-assignment
    if (this == &orig)      // Same object?
      return *this;        // Yes, so skip assignment, and just return *this.
	rows = orig.rows;
	columns = orig.columns;
	data = new MatrixItem[rows*columns];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
			setElement(i, j, orig.getElement(i,j)); // copy original matrix's values
	}
	return (*this); // return reference object
}

// Overload operator <<
ostream& operator<< (ostream& out, Matrix& m)
{
	for(int i = 0; i < m.rows; i++)
	{
	for(int j = 0; j < m.columns; j++)
	{
		if(i == 0 && j == 0) // Will only perform once
			out << endl;	// return carriage 
		out << setw(5) << *(m.data+(i*m.columns+j)); // output value in matrix index (i,j)
	}
	out << endl;
	}
	return out;
}

// Matrix Addition
Matrix Matrix::operator+ (const Matrix& orig) const
{
	Matrix sum(orig.rows, orig.columns); // create new object to store the sum in
	for(int i = 0; i < rows; i++)
	{
	for(int j = 0; j < columns; j++)
		sum.setElement(i, j, (data[i*columns+j] + orig.data[i*columns+j]));
	}
	// Exception handling: Two matrices that do not have matching dimensions cannot be added together
	try
	{
		if(orig.rows != rows)
			throw(orig.rows); // if rows don't match, throw row
		if(orig.rows != rows)
			throw(orig.columns); // if columns don't match, throw column
	}
	catch(int error)
	{
		cout << "Cannot add these matrices." << endl;
		cout << error << " does not equal the other matrix's dimensions." << endl; 
	}
		return sum;
}

// Matrix Addition with reference
Matrix& Matrix::operator+= (const Matrix& orig)
{
	try
	{
		if(orig.rows != rows)
			throw(orig.rows);
		if(orig.rows != rows)
			throw(orig.columns);
	}
	catch(int error)
	{
		cout << "Cannot add these matrices." << endl;
		cout << error << " does not equal the other matrix's dimensions." << endl; 
	}

	for(int i = 0; i < rows; i++)
	{
	for(int j = 0; j < columns; j++)
		setElement(i, j, (data[i*columns+j] + orig.data[i*columns+j]));
	}
		return (*this);		// return reference object (bb += aa) so return bb with bb+aa 
}

// Matrix Multiplication
Matrix Matrix::operator* (const Matrix& orig) const
{
	// Exception handling: If the inner dimensions of the two matrices do not equal then throw exception  
	try
	{
		if(columns != orig.rows)
			throw(columns);
	}
	catch(int error)
	{
		cout << "Cannot multiply these matrices." << endl;
		cout << error << " columns does not equal " << orig.rows << " rows and therefore cannot be multiplied."<< endl; 
	}
	int orig_j = 0;
	int i = 0;
	MatrixItem dot = 0;
	Matrix product(rows, orig.columns);
	for(int count = 0; count < (rows*orig.columns); count++) // Step through entire matrix to set up answer output
	{
		for(int j = 0; j < columns; j++)	// controls columns of current and rows of orig
		{
			dot += data[i*columns + j]*orig.data[j*orig.columns+orig_j]; // Dot product and concatenate
		}
		if(orig_j == (orig.columns-1)) // Once the orig columns end move to the next current row
		{
			product.data[count] = dot; // store dot in product at index[count]
			dot = 0; // reset dot product
			orig_j = 0; // reset orig_j (iterate through columns again for the next row)
			i++; // next row
		}
		else
		{
			product.data[count] = dot;
			dot = 0;
			orig_j++; // move to next column on the original matrix
		}
	}
	return product;
}

// Matrix Multiplication with reference 
Matrix& Matrix::operator*= (const Matrix& orig)
{
	try
	{
		if(columns != orig.rows)
			throw(columns);
	}
	catch(int error)
	{
		cout << "Cannot multiply these matrices." << endl;
		cout << "Column size " << error << " does not equal row size " << orig.rows << " and therefore cannot be multiplied."<< endl; 
	}
	int orig_j = 0;
	int i = 0;
	MatrixItem dot = 0;
	Matrix product(rows, orig.columns);
	for(int count = 0; count < (rows*orig.columns); count++)
	{
		for(int j = 0; j < columns; j++)	// controls columns
		{
			dot += data[i*columns + j]*orig.data[j*orig.columns+orig_j];
		}
		if(orig_j == (orig.columns-1))
		{
			product.data[count] = dot;
			dot = 0;
			orig_j = 0;
			i++;
		}
		else
		{
			product.data[count] = dot;
			dot = 0;
			orig_j++;
		}
	}
	(*this) = product;
	return product;
}

// Test for Equality
 bool Matrix::operator==(const Matrix &orig) const
 {
	bool a = true; // a will remain true until proven false
	// Checking to see if the dimensions are the same
	 if(this->rows != orig.rows || this->columns != orig.columns)
	 {
		 a = false;
	 }
	 if(a == true) // Will not run if first case above is executed
	 {
	for(int i = 0; i < orig.rows; i++)
	{
		for(int j = 0; j < orig.columns; j++)
	{
			// If any of the corresponding elements do not match 
			// then a will be returned as false
			if(data[i*columns + j] != orig.data[i*columns+j])
				a = false;
	}
	}
  }
	 return a;
 }

 // Test for Inequality
 bool Matrix::operator!=(const Matrix &orig) const
 {
	bool a = true;
	 if(this->rows != orig.rows || this->columns != orig.columns)
	 {
		 a = false;
	 }
	 if(a == true)
	 {
	for(int i = 0; i < orig.rows; i++)
	{
		for(int j = 0; j < orig.columns; j++)
	{
			if(data[i*columns + j] != orig.data[i*columns+j])
				a = false;
	}
	}
  }
	 return !a; // If true return false
 }


