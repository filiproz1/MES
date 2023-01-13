#pragma once
#include"myFunctions.h"
#include"Node.h"
#include"Edge.h"
#include<vector>
class ElementIntegration
{
public:
	std::vector<std::vector<double>> xi_table, eta_table; // wartosci pochodnych funkcji ksztaltu w punktach calkowania
	std::vector<std::vector<double>> shape_function_values; // wartosci funkcji ksztaltu w punktach calkowania
	std::vector<double> x, y; // wspolrzedne wezlow tworzacych element
	std::vector<Edge> edges; // krawedzie z zadanym warunkiem brzegowym
	std::vector<double**> jakobiany; // jakobiany dla kazdego PC
	std::vector<double> detJ; // wyznaczniki jakobianu dla kazdego PC
	std::vector<std::vector<double>>dN_dx, dN_dy; // macierz pomocnicza do obliczen - mnozona przez jakobian odwrotny
	std::vector<double**> mat_H_PC, mat_C_PC; // macierze H i C dla punktow calkowania
	double** mat_H, ** mat_C; // zsumowane macierze H i C
	std::vector<double> P;
	double K, spec_heat, density;

	ElementIntegration(std::vector<Node>, double, double, double, double, double);

	void fillTables(); // wype³nia tablice z wartoœciami funkcji kszta³tu i pochodnymi funkcji w PC

	void setJakobian();

	void setdNdxy();  // oblicza wartoœci pochodnych funkcji kszta³tu po x i y

	void setMatricesPC(); // oblicza H i C w punktach ca³kowania

	void setMatrices(); // dodaje H i C z punktów ca³kowania, dodaje Hbc do H i ustawia P

	void setEdges(); // oblicza Hbc i P na krawêdziach z zadanym warunkiem

	void setUpElement();
};