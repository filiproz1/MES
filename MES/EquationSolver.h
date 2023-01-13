#pragma once
#include<exception>
#include<vector>
class EquationSolver
{
public: 
	double** matrix; // macierz wspó³czynników
	std::vector<double> last_col; // wektor wyrazów wolnych
	int size;
	std::vector<double> result;

	EquationSolver();
	EquationSolver(double**, int);
	EquationSolver(double** m, std::vector<double> v);

	std::vector<double> solve();
	std::vector<double> getResult();
};

