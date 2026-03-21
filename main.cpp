#include "Matrix.h"

#define stringify(x) #x

int main()
{
	Matrixes test({ {1, 2, 3}, 
					{4, 5, 6}, 
					{7, 8, 9} });

	Matrixes test2({ {1, 2, 3},
					 {4, 5, 6},
					 {7, 8, 9} });

	std::cout << test.GetColumnSize() << "\n" << test.GetRowSize() << "\n";
	if (test.IsMatrixSquare())
		std::cout << stringify(test) << " is square" << std::endl;
	else
		std::cout << stringify(test) << " is NOT square" << std::endl;
	std::cout << test.GetNumberAt(3, 3) << std::endl;

	test.PrintMatrix();
	std::cout << std::endl;
	test.GetTranspose().PrintMatrix();

	(test * test.GetTranspose()).PrintMatrix();

	(test + test2).PrintMatrix();

}