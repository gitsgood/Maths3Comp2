#include "Matrix.h"

#define stringify(x) #x

int main()
{
	Matrixes<int> test({ {1, 2, 3}, 
						{4, 5, 6}, 
						{7, 8, 9} });

	Matrixes<double> test2({ {1, 2, 3},
							 {4, 5, 6},
							 {7, 8, 9} });

	std::cout << test.GetColumnSize() << "\n" << test.GetRowSize() << "\n";
	if (test.IsMatrixSquare())
		std::cout << stringify(test) << " is square\n";
	else
		std::cout << stringify(test) << " is NOT square\n";
	std::cout << test.GetNumberAt(3, 3) << std::endl;

	test.PrintMatrix();

	test.GetTranspose().PrintMatrix();

	(test * test.GetTranspose()).PrintMatrix();

	(test * (int)1.5).PrintMatrix();

	(test + test2).PrintMatrix();

}