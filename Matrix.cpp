#include "Matrix.h"

double Matrixes::GetNumberAt(const size_t RowIndice, const size_t ColumnIndice) const
{
	try
	{
		return Matrix.at(RowIndice - 1).at(ColumnIndice - 1);
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << ex.what() << std::endl;
		std::cout << "(The row indice or column indice are out of this matrix's range...)" << std::endl;
	}
}

void Matrixes::PrintMatrix() const
{
	// Calculate the maximum width of each column for proper alignment
	size_t MaxColumnWidth{ 0 };
	for (const std::vector<double>& Row : Matrix)
		for (double Element : Row)
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
	for (const std::vector<double>& Row : Matrix)
	{
		RowStrings[RowIter] = HorizontalBorder;
		RowIter++;

		std::ostringstream RowStream;
		for (double Element : Row)
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

Matrixes Matrixes::operator*(const Matrixes& rhs) const
{
	if (ColumnCount != rhs.RowCount)
		throw std::invalid_argument("Incompatible matrix dimensions for multiplication.");

	std::vector<std::vector<double>> ResultingMatrix;
	ResultingMatrix.resize(GetRowCount());

	for (size_t RowIter{ 0 }; RowIter < GetRowCount(); RowIter++)
	{
		ResultingMatrix[RowIter].resize(rhs.GetColumnCount(), 0.0);
		for (size_t ColumnIter{ 0 }; ColumnIter < rhs.GetColumnCount(); ColumnIter++)
		{
			double Element{ 0.f };
			for (size_t k = 0; k < ColumnCount; k++)
			{
				Element += Matrix[RowIter][k] * rhs.Matrix[k][ColumnIter];
			}
			ResultingMatrix[RowIter][ColumnIter] = Element;
		}
	}
	return ResultingMatrix;
}

Matrixes Matrixes::operator+(const Matrixes& rhs) const
{
	if (this->AreDimensionsCompatibleForAddition(rhs) == false)
		throw std::invalid_argument("Incompatible matrix dimensions for addition.");

	std::vector<std::vector<double>> ResultingMatrix;
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

Matrixes Matrixes::GetTranspose() const
{
	std::vector<std::vector<double>> Transpose;

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
