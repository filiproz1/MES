#pragma once
#include<string>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<cmath>
#include"EquationSolver.h"


#define PRINT false;

const int MACIERZ_PC = 2; // ilosc punktow calkowania dla macierzy H i C
const int KRAWEDZ_PC = 2; // ilosc punktow calkowania dla macierzy Hbc i wektora P

const int MACIERZ_PC_2 = MACIERZ_PC * MACIERZ_PC; // ilosc pc dla macierzy H i C do kwadratu

const std::vector<std::vector<double>> punkty_calkowania = {
	{-1 / sqrt(3),	1 / sqrt(3)},										//N = 2
	{-0.77459666924,		0,			0.77459666924},					//N = 3
	{-0.861136,		-0.339981,	0.339981,	0.861136},					//N = 4
	{-0.906180,		-0.538469,	0,			0.538469,		0.906180}	//N = 5
};
const std::vector<std::vector<double>> wagi = {
	{1,1},												//N = 2
	{0.555555555, 0.888888888, 0.555555555},			//N = 3
	{0.347855, 0.652145, 0.652145, 0.347855},			//N = 4
	{0.236927, 0.478629, 0.568889, 0.478629, 0.236927}	//N = 5
};

std::string strip(std::string s);	//strips string from whitespaces

void print_matrix(double** mat, int n, int m);
void print_vector(std::vector<double> v);
std::vector<double> getMinMax(std::vector<double> v);

double shape_function(double xi, double eta, int n); // funkcja ksztaltu, n - ID wierzcholka
double dN_dxi(double, int);
double dN_deta(double, int);

// operacje na macierzach
double** mat2_inv(double** mat); //macierz odwrotna - tylko 2x2, bez dzielenia przez wyznacznik
double** mat_mult(double** mat, double c, int n, int m); // mnozenie macierzy przez stala
double** mat_sum(double** mat1, double** mat2, int n, int m); // suma macierzy
double** mat_transpose(double** mat, int n, int m); // transpozycja macierzy
double** mat_vec_to_double(std::vector<std::vector<double>> mat); // konwersja z macierzy z vector na double**
double** vector_mult(double* v1, double* v2, int n); // mnozenie wektorow, drugi jest transponowany
double* vec_to_double(std::vector<double> v); // konwersja vector -> double*
std::vector<double> mult_mat_vec(double** mat, std::vector<double> vec);

double norm(std::vector<double> p1, std::vector<double> p2); // oblicza odleglosc pomiedzy punktami

void runSimulation(double** H_global, double** C_global, std::vector<double> P_global, std::vector<double> t0, int t, int dt);

