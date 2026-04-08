#include "MathsExercices.h"

#define stringify(x) #x

static_assert(sizeof(Matrixes<int>) == sizeof(Matrixes<double>), "Matrixes struct should have the same size regardless of the type of its elements.");
static_assert(sizeof(Matrixes<int>) == sizeof(std::vector<std::vector<int>>) + 2 * sizeof(size_t), "Matrixes struct should only contain a vector of vectors and two size_t variables.");
static_assert(std::is_same_v<decltype(std::declval<Matrixes<int>>() * 1.0), Matrixes<double>>, "Multiplying Matrixes<int> by double should produce Matrixes<double>");

int main()
{
	{
		//Matrixes<int> test({ {1, 2, 3}, 
		//					{4, 5, 6}, 
		//					{7, 8, 9} });

		//Matrixes<double> test2({ {1, 1, -1, -2},
		//						 {2, -1, 1, 5},
		//						 {-1, 2, 2, 1} });

		//std::cout << test.GetColumnSize() << "\n" << test.GetRowSize() << "\n";
		//if (test.IsMatrixSquare())
		//	std::cout << stringify(test) << " is square\n";
		//else
		//	std::cout << stringify(test) << " is NOT square\n";
		//std::cout << test.GetNumberAt(3, 3) << std::endl;

		//test.PrintMatrix();

		//test.GetTranspose().PrintMatrix();

		//(test * test.GetTranspose()).PrintMatrix();

		//(test * (int)1.5).PrintMatrix();

		//(test + test2).PrintMatrix();

		//test2.GetRowEchelonForm(true).PrintMatrix();

		//test2.GetRowEchelonForm(false).PrintMatrix();
	}

	MatrixConfig::bDestructorVerbosity = false; // Disable verbose output for destructors, to avoid cluttering the console with messages about matrix destruction during exercises.

	MathsExercices Exercices;

	return 0;
}