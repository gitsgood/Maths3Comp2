#include "MathsExercices.h"

void MathsExercices::Exercice4_6_4()
{
	Matrixes<double> VandermondeAugmented({
		{ 1,   0,    0,     0,      1.0      },
		{ 1,   0.5,  0.25,  0.125,  9.0 / 16  },
		{ 1,   1.5,  2.25,  3.375, -5.0 / 16  },
		{ 1,   3,    9,     27,     4.0      }
		});

	auto Solution = VandermondeAugmented.GetRowEchelonForm(true); // RREF gives solution directly
	Solution.PrintMatrix();
}

void MathsExercices::Exercice4_6_7()
{
	const double Pi = 3.14159265358979;

	Matrixes<double> HermiteAugmented({
		{ 1,  1,       1,           1,            0  },   // p(1)  = 0
		{ 1,  Pi,      Pi * Pi,       Pi * Pi * Pi,     0  },   // p(π)  = 0
		{ 0,  1,       2,           3,            1  },   // p'(1) = 1
		{ 0,  1,       2 * Pi,        3 * Pi * Pi,     -1  }    // p'(π) = -1
		});

	auto Solution = HermiteAugmented.GetRowEchelonForm(true);
	Solution.PrintMatrix();
}

void MathsExercices::Exercice4_6_14()
{
	Matrixes<double> A({
		{ 1, 4 },
		{ 1, 0 },
		{ 1, 2 },
		{ 1, 3 }
		});

	Matrixes<double> b({
		{ 3 },
		{ 1 },
		{ 0 },
		{ 4 }
		});

	// Build AᵀA and Aᵀb
	Matrixes<double> AT = A.GetTranspose();
	Matrixes<double> ATA = AT * A;          // 2x2
	Matrixes<double> ATb = AT * b;          // 2x1

	// Augment AᵀA with Aᵀb to form [AᵀA | Aᵀb]
	Matrixes<double> NormalEquations({
		{ ATA.GetNumberAt(1,1), ATA.GetNumberAt(1,2), ATb.GetNumberAt(1,1) },
		{ ATA.GetNumberAt(2,1), ATA.GetNumberAt(2,2), ATb.GetNumberAt(2,1) }
		});

	auto Solution = NormalEquations.GetRowEchelonForm(true);
	Solution.PrintMatrix();

	// Last column gives a (intercept) and b (slope)
	// y = a + bx
}

void MathsExercices::Exercice4_6_15()
{
	Matrixes<double> A({
		{ 1,  0.5,  0.25 },
		{ 1,  0.5,  0.25 },
		{ 1,  2.5,  6.25 },
		{ 1,  3.0,  9.0  },
		{ 1,  4.0,  16.0 }
		});

	Matrixes<double> b({
		{  0.5 },
		{  1.5 },
		{ -1.0 },
		{  0.5 },
		{  3.0 }
		});

	Matrixes<double> AT = A.GetTranspose();
	Matrixes<double> ATA = AT * A;   // 3x3
	Matrixes<double> ATb = AT * b;   // 3x1

	// Print A, Aᵀ and Aᵀb as the problem asks for
	std::cout << "A:\n";   A.PrintMatrix();
	std::cout << "AT:\n";  AT.PrintMatrix();
	std::cout << "ATb:\n"; ATb.PrintMatrix();

	// Augment to [AᵀA | Aᵀb]
	Matrixes<double> NormalEquations({
		{ ATA.GetNumberAt(1,1), ATA.GetNumberAt(1,2), ATA.GetNumberAt(1,3), ATb.GetNumberAt(1,1) },
		{ ATA.GetNumberAt(2,1), ATA.GetNumberAt(2,2), ATA.GetNumberAt(2,3), ATb.GetNumberAt(2,1) },
		{ ATA.GetNumberAt(3,1), ATA.GetNumberAt(3,2), ATA.GetNumberAt(3,3), ATb.GetNumberAt(3,1) }
		});

	auto Solution = NormalEquations.GetRowEchelonForm(true);
	Solution.PrintMatrix();

	// Last column gives a, b, c
	// p(x) = a + bx + cx²
}
