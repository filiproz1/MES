#pragma once
#include<vector>
#include"Node.h"
#include"myFunctions.h"
class Edge
{
public:
	int edge_id; // identyfikator krawedzi - gora/dol/lewo/prawo
	std::vector<std::vector<double>> PC; // wsp�rz�dne punkt�w ca�kowania
	std::vector<std::vector<double>> N_values; // warto�ci funkcji kszta�tu w PC
	Node N1, N2;
	double detJ; // wyznacznik jakobianu przeksztalcenia
	double** Hbc; // macierz H z warunkiem brzegowym
	double* P;
	double alpha, tot;

	Edge(Node, Node, int, double, double);

	void setPC();

	void setNvalues();

	void setDetJ();

	void setHbc();

	void setP();
};

