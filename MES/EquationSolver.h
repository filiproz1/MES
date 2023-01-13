#pragma once
#include<exception>
#include<vector>
class EquationSolver
{
public: 
	double** matrix; // macierz wsp�czynnik�w
	std::vector<double> last_col; // wektor wyraz�w wolnych
	int size;
	std::vector<double> result;

	EquationSolver();
	EquationSolver(double**, int);
	EquationSolver(double** m, std::vector<double> v);

	std::vector<double> solve();
	std::vector<double> getResult();
};

