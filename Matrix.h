#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <sstream>

struct Matrixes
{
	size_t RowCount{ 0 };
	size_t ColumnCount{ 0 };
	std::vector<std::vector<double>> Matrix;

	Matrixes(std::vector<std::vector<double>> inMatrix)
	{
		RowCount = inMatrix.size();
		ColumnCount = inMatrix.at(0).size();

		// Find the maximum row length
		size_t maxCols = 0;
		for (const std::vector<double>& Row : inMatrix)
			if (Row.size() > maxCols)
				maxCols = Row.size();

		// Pad any shorter rows with 0.0
		ColumnCount = maxCols;
		for (std::vector<double>& Row : inMatrix)
			if (Row.size() < maxCols)
				Row.resize(maxCols, 0.0);

		Matrix = inMatrix;
	}
	~Matrixes() 
	{

	}

	inline size_t GetRowSize() const { return ColumnCount; }
	inline size_t GetColumnSize() const { return RowCount; }

	inline size_t GetRowCount() const{ return RowCount; }
	inline size_t GetColumnCount() const { return ColumnCount; }

	inline bool IsMatrixSquare() const { return RowCount == ColumnCount ? true : false; }

	/**
	* @param RowIndice The row indice of the number we want. Like in maths, it assumes it starts from 1.
	* @param ColumnIndice Column indice, also assuming it starts from 1.
	* 
	* @return The element's value at given matrix coordinates.
	*/
	double GetNumberAt(const size_t RowIndice, const size_t ColumnIndice) const;

	/**
	* @brief Prints the matrix in an ascii square.
	*/
	void PrintMatrix() const;

	void PrintMatrixSimple() const;

	Matrixes operator*(const Matrixes& rhs) const;

	inline bool operator==(const Matrixes& rhs) const { return Matrix == rhs.Matrix ? true : false; }

	inline bool AreDimensionsCompatibleForAddition(const Matrixes& OtherMatrix) const { return (RowCount == OtherMatrix.RowCount && ColumnCount == OtherMatrix.ColumnCount) ? true : false; }

	Matrixes operator+(const Matrixes& rhs) const;

	Matrixes GetTranspose() const;
};

#endif // !MATRIX_H