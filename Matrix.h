#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <sstream>

class MatrixConfig
{
public:
	inline static bool bDestructorVerbosity = true;
};

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
		if (!MatrixConfig::bDestructorVerbosity) return;
		std::cout << "Matrix destructed\n";
		PrintMatrixSimple();
	}

	// It seems that writing our own destructor tells the compiler to drop default copy/move behaviours.
	// So we need to explicitely tell it to behave in the default way for such operations.

	Matrixes(const Matrixes&) = default;
	Matrixes& operator=(const Matrixes&) = default;
	Matrixes(Matrixes&&) = default;
	Matrixes& operator=(Matrixes&&) = default;

	inline size_t GetRowSize() const noexcept { return ColumnCount; }
	inline size_t GetColumnSize() const noexcept { return RowCount; }

	inline size_t GetRowCount() const noexcept { return RowCount; }
	inline size_t GetColumnCount() const noexcept { return ColumnCount; }

	inline bool IsMatrixSquare() const noexcept { return RowCount == ColumnCount; }

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

	Matrixes GetRowEchelonForm(bool bReduceToFullRREF = false) const
	{
		if (bReduceToFullRREF && std::is_integral<T>::value)
		{
			std::cerr << "Achtung: RREF on integer matrix will produce incorrect results. \n"
					  << "Falling back to plain REF.\n";
			return GetRowEchelonForm(false);
		}

		auto [U, LFactors, Swaps] = PrivateGaussianElimination();

		if (!bReduceToFullRREF)
			return Matrixes(U);

		for (size_t CurrentRow = 0; CurrentRow < RowCount; CurrentRow++)
		{
			size_t PivotColumn = ColumnCount;
			for (size_t CurrentColumn = 0; CurrentColumn < ColumnCount; CurrentColumn++)
			{
				if (std::abs(U[CurrentRow][CurrentColumn]) > (T)1e-9)
				{
					PivotColumn = CurrentColumn;
					break;
				}
			}

			if (PivotColumn == ColumnCount)
				continue;

			T PivotValue = U[CurrentRow][PivotColumn];
			PrivateMultiplyRowByScalar(U, CurrentRow, (T)1 / PivotValue);
			U[CurrentRow][PivotColumn] = (T)1;

			for (size_t RowToEliminate = 0; RowToEliminate < CurrentRow; RowToEliminate++)
			{
				T ValueToEliminate = U[RowToEliminate][PivotColumn];
				if (std::abs(ValueToEliminate) < (T)1e-9)
					continue;

				PrivateAddMultipleOfRowToAnother(U, CurrentRow, RowToEliminate, -ValueToEliminate);
				U[RowToEliminate][PivotColumn] = (T)0;
			}
		}

		return Matrixes(U);
	}

	template <typename T>
	struct LUResult
	{
		Matrixes<T> L; // Lower triangular matrix
		Matrixes<T> U; // Upper triangular matrix
		int NumberOfRowSwaps; // Useful for computing the determinant later
	};

	LUResult<T> GetLUDecomposition() const
	{
		if (!IsMatrixSquare())
			throw std::invalid_argument("LU decomposition requires a square matrix.\n");

		if (std::is_integral<T>::value)
		{
			std::cerr << "Achtung: LU decomposition on an integer matrix may produce incorrect results \n"
					  << "due to integer division. Consider using Matrixes<double> instead.\n";
		}

		auto [U, LFactors, NumberOfRowSwaps] = PrivateGaussianElimination();

		return LUResult<T>{ Matrixes<T>(LFactors), Matrixes<T>(U), NumberOfRowSwaps };
	}

private:
	// Private helper functions.

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

	void PrivateSwapRows(std::vector<std::vector<T>>& Mat, size_t Row1, size_t Row2) const
	{
		std::swap(Mat[Row1], Mat[Row2]);
	}

	void PrivateMultiplyRowByScalar(std::vector<std::vector<T>>& Mat, size_t Row, T Scalar) const
	{
		for (T& Element : Mat[Row])
			Element *= Scalar;
	}

	void PrivateAddMultipleOfRowToAnother(std::vector<std::vector<T>>& Mat, size_t SourceRow, size_t TargetRow, T Scalar) const
	{
		for (size_t ColIter = 0; ColIter < Mat[SourceRow].size(); ColIter++)
			Mat[TargetRow][ColIter] += Scalar * Mat[SourceRow][ColIter];
	}

	struct GaussianEliminationResult
	{
		std::vector<std::vector<T>> U;
		std::vector<std::vector<T>> LFactors;
		int NumberOfRowSwaps;
	};

	GaussianEliminationResult PrivateGaussianElimination() const
	{
		const size_t N = RowCount;
		std::vector<std::vector<T>> U = Matrix;
		std::vector<std::vector<T>> LFactors(N, std::vector<T>(N, (T)0));
		for (size_t i = 0; i < N; i++)
			LFactors[i][i] = (T)1;

		int NumberOfRowSwaps = 0;

		for (size_t CurrentColumn = 0; CurrentColumn < N; CurrentColumn++)
		{
			size_t RowWithLargestValue = CurrentColumn;
			T LargestValue = std::abs(U[CurrentColumn][CurrentColumn]);
			for (size_t SearchRow = CurrentColumn + 1; SearchRow < N; SearchRow++)
			{
				T Candidate = std::abs(U[SearchRow][CurrentColumn]);
				if (Candidate > LargestValue)
				{
					LargestValue = Candidate;
					RowWithLargestValue = SearchRow;
				}
			}

			if (std::abs(U[RowWithLargestValue][CurrentColumn]) < (T)1e-9)
				continue;

			if (RowWithLargestValue != CurrentColumn)
			{
				PrivateSwapRows(U, CurrentColumn, RowWithLargestValue);
				for (size_t SwapColumn = 0; SwapColumn < CurrentColumn; SwapColumn++)
					std::swap(LFactors[CurrentColumn][SwapColumn],
						LFactors[RowWithLargestValue][SwapColumn]);
				NumberOfRowSwaps++;
			}

			for (size_t RowToEliminate = CurrentColumn + 1; RowToEliminate < N; RowToEliminate++)
			{
				if (std::abs(U[CurrentColumn][CurrentColumn]) < (T)1e-9)
					continue;

				T EliminationScaleFactor =
					U[RowToEliminate][CurrentColumn] / U[CurrentColumn][CurrentColumn];

				LFactors[RowToEliminate][CurrentColumn] = EliminationScaleFactor;

				PrivateAddMultipleOfRowToAnother(U, CurrentColumn, RowToEliminate, -EliminationScaleFactor);
				U[RowToEliminate][CurrentColumn] = (T)0;
			}
		}

		return { U, LFactors, NumberOfRowSwaps };
	}
};

#endif // !MATRIX_H