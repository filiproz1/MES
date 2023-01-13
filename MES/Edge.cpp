#include "Edge.h"

Edge::Edge(Node n1, Node n2, int id, double a, double t)
{
	N1 = n1;
	N2 = n2;
	edge_id = id;
	detJ = 0;
	Hbc = new double* [4];
	P = new double[4];
	alpha = a;
	tot = t;
}

void Edge::setPC()
{
	//hard coded - indeks petli przy sprawdzaniu determinuje pozycje krawedzi
	// 0 - top - eta = 1;
	// 1 - left - xi = -1;
	// 2 - bottom - eta = -1;
	// 3 - right - xi = 1;
	std::vector<double> coords = {};
	switch (edge_id)
	{
		case 0:
			for (auto i = 0; i < KRAWEDZ_PC; i++)
			{
				coords = {};
				coords.push_back(punkty_calkowania[KRAWEDZ_PC - 2][i]); // xi
				coords.push_back(1); // eta
				PC.push_back(coords);
			}
			break;
		case 1:
			for (auto i = 0; i < KRAWEDZ_PC; i++)
			{
				coords = {};
				coords.push_back(-1); // x1
				coords.push_back(punkty_calkowania[KRAWEDZ_PC - 2][i]); // eta
				PC.push_back(coords);
			}
			break;
		case 2:
			for (auto i = 0; i < KRAWEDZ_PC; i++)
			{
				coords = {};
				coords.push_back(punkty_calkowania[KRAWEDZ_PC - 2][i]); // xi
				coords.push_back(-1); // eta
				PC.push_back(coords);
			}
			break;
		case 3:
			for (auto i = 0; i < KRAWEDZ_PC; i++)
			{
				coords = {};
				coords.push_back(1); // xi
				coords.push_back(punkty_calkowania[KRAWEDZ_PC - 2][i]); // eta
				PC.push_back(coords);
			}
			break;
		default:
			throw std::exception("Edge::setPC() - wrong edge id");
	}
}

void Edge::setNvalues()
{
	for (auto& punkt : PC)
	{
		std::vector<double> punkt_N_value;
		for (int i = 0; i < 4; i++)
		{
			double xi = punkt[0];
			double eta = punkt[1];
			punkt_N_value.push_back(shape_function(xi, eta, i+1));
		}
		N_values.push_back(punkt_N_value);
	}
}

void Edge::setDetJ()
{
	std::vector<double> p1 = { N1.x, N1.y };
	std::vector<double> p2 = { N2.x, N2.y };	
	double L = norm(p1, p2);
	detJ = L / 2;
}

void Edge::setHbc()
{
	for (int i = 0; i < 4; i++) // inicjalizacja macierzy elementu Hbc
	{
		Hbc[i] = new double[4];
		for (int j = 0; j < 4; j++)
		{
			Hbc[i][j] = 0;
		}
	}
	for (int i = 0; i < KRAWEDZ_PC; i++) // obliczanie Hbc dla pojedynczych punktow calkowania
	{
		double* N_vec = vec_to_double(N_values[i]);
		double** Hbc_temp = vector_mult(N_vec, N_vec, 4);
		Hbc_temp = mat_mult(Hbc_temp, wagi[KRAWEDZ_PC - 2][i],4,4);
		Hbc = mat_sum(Hbc, Hbc_temp, 4, 4);
	}
	Hbc = mat_mult(Hbc, detJ*alpha, 4, 4);
}

void Edge::setP()
{
	for (int i = 0; i < 4; i++)
	{
		P[i] = 0; // inicjalizacja wektora P
	}

	for (int i = 0; i < KRAWEDZ_PC; i++)
	{
		std::vector<double> N_vec = N_values[i];
		for (int j = 0; j < 4; j++)
		{
			N_vec[j] *= tot * wagi[KRAWEDZ_PC - 2][i];
			P[j] += N_vec[j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		P[i] *= alpha * detJ;
	}
}