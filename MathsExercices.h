#ifndef MATHSEXERCICES
#define MATHSEXERCICES

#include "Matrix.h"

class MathsExercices
{
public:
	void Exercice4_6_4();

	void Exercice4_6_7();

	void Exercice4_6_14();

	void Exercice4_6_15();

	MathsExercices()
	{
		std::cout << "Running maths exercices...\n";
		std::cout << "-------------------------\n";
		std::cout << "\nExercice 4.6.4:\n";
		Exercice4_6_4();
		std::cout << "\n-------------------------\n";
		std::cout << "\nExercice 4.6.7:\n";
		Exercice4_6_7();
		std::cout << "\n-------------------------\n";
		std::cout << "\nExercice 4.6.14:\n";
		Exercice4_6_14();
		std::cout << "\n-------------------------\n";
		std::cout << "\nExercice 4.6.15:\n";
		Exercice4_6_15();
	}
};

#endif // !MATHSEXERCICES