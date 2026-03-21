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
		std::cout << "Matrix destructed\n";
		PrintMatrixSimple();
	}
	// It seems that writing our own destructor tells the compiler to drop default copy/move behaviours.
	// So we need to explicitely tell it to behave in the default way for such operations.
	Matrixes(const Matrixes&) = default;
	Matrixes& operator=(const Matrixes&) = default;
	Matrixes(Matrixes&&) = default;
	Matrixes& operator=(Matrixes&&) = default;

	inline size_t GetRowSize() const noexcept{ return ColumnCount; }
	inline size_t GetColumnSize() const noexcept{ return RowCount; }

	inline size_t GetRowCount() const noexcept{ return RowCount; }
	inline size_t GetColumnCount() const noexcept{ return ColumnCount; }

	inline bool IsMatrixSquare() const noexcept{ return RowCount == ColumnCount; }

	inline bool operator==(const Matrixes& rhs) const noexcept { return Matrix == rhs.Matrix; }

	inline bool AreDimensionsCompatibleForAddition(const Matrixes& OtherMatrix) const noexcept { return (RowCount == OtherMatrix.RowCount && ColumnCount == OtherMatrix.ColumnCount) ? true : false; }

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

	Matrixes operator+(const Matrixes& rhs) const;

	Matrixes GetTranspose() const;

private:
	// I decided to organise the code such that the errors would be thrown by private functions, and the publicly available ones would automatically handle that.
	// Hopefully meaning we won't need to worry about writing try and catch when using this library.

	Matrixes PrivateMultiply(const Matrixes& rhs) const;

	Matrixes PrivateAdd(const Matrixes& rhs) const;
};

#endif // !MATRIX_H