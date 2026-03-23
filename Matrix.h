#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <sstream>

template <typename T>
struct Matrixes
{
	size_t RowCount{ 0 };
	size_t ColumnCount{ 0 };
	std::vector<std::vector<T>> Matrix;

	Matrixes(std::vector<std::vector<T>> inMatrix)
	{
		RowCount = inMatrix.size();
		ColumnCount = inMatrix.at(0).size();

		// Find the maximum row length
		size_t maxCols = 0;
		for (const std::vector<T>& Row : inMatrix)
			if (Row.size() > maxCols)
				maxCols = Row.size();

		// Pad any shorter rows with 0.0
		ColumnCount = maxCols;
		for (std::vector<T>& Row : inMatrix)
			if (Row.size() < maxCols)
				Row.resize(maxCols, (T)0);

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

	template <typename N>
	inline bool AreDimensionsCompatibleForAddition(const Matrixes<N>& OtherMatrix) const noexcept { return (RowCount == OtherMatrix.RowCount && ColumnCount == OtherMatrix.ColumnCount) ? true : false; }

	/**
	* @param RowIndice The row indice of the number we want. Like in maths, it assumes it starts from 1.
	* @param ColumnIndice Column indice, also assuming it starts from 1.
	* 
	* @return The element's value at given matrix coordinates.
	*/
	T GetNumberAt(const size_t RowIndice, const size_t ColumnIndice) const
	{
		try
		{
			return Matrix.at(RowIndice - 1).at(ColumnIndice - 1);
		}
		catch (const std::out_of_range& ex)
		{
			std::cout << ex.what() << "\n";
			std::cout << "(The row indice or column indice are out of this matrix's range...)\n";
		}
	}

	/**
	* @brief Prints the matrix in an ascii square.
	*/
	void PrintMatrix() const
	{
		// Calculate the maximum width of each column for proper alignment
		size_t MaxColumnWidth{ 0 };
		for (const std::vector<T>& Row : Matrix)
			for (T Element : Row)
			{
				std::ostringstream ss;
				ss << Element;
				size_t ElementWidth = ss.str().length();
				if (ElementWidth > MaxColumnWidth)
					MaxColumnWidth = ElementWidth;
			}

		// +3 per column: 1 separator + 1 space each side, +1 for the last separator
		size_t TotalMatrixWidth = (MaxColumnWidth + 3) * GetColumnCount() + 1;
		std::string HorizontalBorder(TotalMatrixWidth, '-');

		std::vector<std::string> RowStrings;
		RowStrings.resize(GetRowCount() * 2 + 1);

		size_t RowIter{ 0 };
		for (const std::vector<T>& Row : Matrix)
		{
			RowStrings[RowIter] = HorizontalBorder;
			RowIter++;

			std::ostringstream RowStream;
			for (T Element : Row)
			{
				std::ostringstream ss;
				ss << Element;
				std::string ElementStr = ss.str();

				// Pad element to MaxColumnWidth
				size_t Padding = MaxColumnWidth - ElementStr.length();
				RowStream << "| " << ElementStr << std::string(Padding, ' ') << " ";
			}
			RowStream << "|";
			RowStrings[RowIter] = RowStream.str();
			RowIter++;
		}

		// Bottom border
		RowStrings[RowIter] = HorizontalBorder;

		for (const std::string& RowString : RowStrings)
			std::cout << RowString << "\n";
	}

	void PrintMatrixSimple() const
	{
		for (const std::vector<T>& Row : Matrix)
		{
			for (T Element : Row)
			{
				std::cout << Element << "\t";
			}
			std::cout << "\n";
		}
	}

	template <typename N>
	Matrixes operator*(const Matrixes<N>& rhs) const
	{
		try
		{
			return PrivateMultiplyByMatrix(rhs);
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << e.what();
		}
	}

	/**
	* @return Scalar matrix multiplication. Type of the resulting matrix is the same as the type of the scalar.
	*/
	template <typename N>
	Matrixes<N> operator*(const N& Scalar) const
	{
		std::vector<std::vector<N>> ResultingMatrix;
		ResultingMatrix.resize(GetRowCount());
		for (size_t RowIter{ 0 }; RowIter < GetRowCount(); RowIter++)
		{
			ResultingMatrix[RowIter].resize(GetColumnCount(), 0.0);
			for (size_t ColumnIter{ 0 }; ColumnIter < GetColumnCount(); ColumnIter++)
			{
				ResultingMatrix[RowIter][ColumnIter] = Matrix[RowIter][ColumnIter] * Scalar;
			}
		}
		return ResultingMatrix;
	}

	template <typename N>
	Matrixes operator+(const Matrixes<N>& rhs) const
	{
		try
		{
			return PrivateAdd(rhs);
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << e.what();
		}
	}

	Matrixes GetTranspose() const
	{
		std::vector<std::vector<T>> Transpose;

		Transpose.resize(ColumnCount);
		for (size_t RowIter{ 0 }; RowIter < ColumnCount; RowIter++)
		{
			Transpose[RowIter].resize(RowCount);
			for (size_t ColumnIter{ 0 }; ColumnIter < RowCount; ColumnIter++)
			{
				Transpose[RowIter][ColumnIter] = Matrix[ColumnIter][RowIter];
			}
		}

		//size_t RowIter{ 0 };
		//for (std::vector<double>& Row : Transpose)
		//{
		//	Row.resize(RowCount);
		//	size_t ColumnIter{ 0 };
		//	for (double& Element : Row)
		//	{
		//		Element = Matrix.at(ColumnIter).at(RowIter);
		//		ColumnIter++;
		//	}
		//	RowIter++;
		//}

		return Transpose;
	}

private:
	// I decided to organise the code such that the errors would be thrown by private functions, and the publicly available ones would automatically handle that.
	// Hopefully meaning we won't need to worry about writing try and catch when using this library.

	template <typename N>
	Matrixes PrivateMultiplyByMatrix(const Matrixes<N>& rhs) const
	{
		if (ColumnCount != rhs.RowCount)
			throw std::invalid_argument("Incompatible matrix dimensions for multiplication.\n");

		std::vector<std::vector<T>> ResultingMatrix;
		ResultingMatrix.resize(GetRowCount());

		for (size_t RowIter{ 0 }; RowIter < GetRowCount(); RowIter++)
		{
			ResultingMatrix[RowIter].resize(rhs.GetColumnCount(), 0.0);
			for (size_t ColumnIter{ 0 }; ColumnIter < rhs.GetColumnCount(); ColumnIter++)
			{
				T Element{ (T)0 };
				for (size_t k = 0; k < ColumnCount; k++)
				{
					Element += Matrix[RowIter][k] * rhs.Matrix[k][ColumnIter];
				}
				ResultingMatrix[RowIter][ColumnIter] = Element;
			}
		}
		return ResultingMatrix;
	}

	template <typename N>
	Matrixes PrivateAdd(const Matrixes<N>& rhs) const
	{
		if (this->AreDimensionsCompatibleForAddition(rhs) == false)
			throw std::invalid_argument("Incompatible matrix dimensions for addition.\n");

		std::vector<std::vector<T>> ResultingMatrix;
		ResultingMatrix.resize(GetRowCount());
		for (size_t RowIter{ 0 }; RowIter < GetRowCount(); RowIter++)
		{
			ResultingMatrix[RowIter].resize(GetColumnCount(), 0.0);
			for (size_t ColumnIter{ 0 }; ColumnIter < GetColumnCount(); ColumnIter++)
			{
				ResultingMatrix[RowIter][ColumnIter] = Matrix[RowIter][ColumnIter] + rhs.Matrix[RowIter][ColumnIter];
			}
		}
		return ResultingMatrix;
	}
};

#endif // !MATRIX_H