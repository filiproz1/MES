#include <iostream>
#include"Grid.h"
#include"GlobalData.h"
#include"GridFile.h"
#include"ElementIntegration.h"
#include"Edge.h"
#include"EquationSolver.h"

using std::vector;


int main()
{
	try {
		// wczytywanie danych z pliku do obiektów
		GridFile grid_file("txt/Test1_4_4.txt");
		//GridFile grid_file("txt/Test2_4_4_MixGrid.txt");
		//GridFile grid_file("txt/Test3_31_31_kwadrat.txt");

		GlobalData global_data = grid_file.readGlobalData();
		Grid grid = grid_file.readGridData();

		// wypisanie danych z obiektów
		//global_data.printData();
		//grid.printNodesInfo();
		//grid.printElementsIDs();

		double** H_global = new double* [grid.nN]; // zagregowana macierz H
		double** C_global = new double* [grid.nN]; // zagregowana macierz C
		vector<double> P_global = vector<double>(grid.nN);	//	zagregowany wektor P
		vector<double> t0 = vector<double>(grid.nN);	//temperatura poczatkowa
		for(double &v : t0) v = global_data.t_init;

		//inicjalizacja macierzy H i C globalnych, ustawienie temperatury poczatkowej w wezlach
		for (int i = 0; i < grid.nN; i++)
		{
			H_global[i] = new double[grid.nN];
			C_global[i] = new double[grid.nN];
			P_global[i] = 0;
			for (int j = 0; j < grid.nN; j++)
			{
				H_global[i][j] = 0;
				C_global[i][j] = 0;
			}
		}

		for (auto i = 0; i < grid.nE; i++) // petla po elementach, tworzenie elementow do obliczania macierzy H,C,P
		{
			Element elem = grid.Elem[i];
			std::vector<Node> elem_nodes;
			for (int j = 0; j < 4; j++) 
				elem_nodes.push_back(grid.ND[elem.ID[j] - 1]);

			ElementIntegration element(elem_nodes, global_data.K, global_data.spec_heat, global_data.density, global_data.alpha, global_data.tot);
			element.setEdges();
			element.setUpElement();

			//agregacja macierzy H C P z elementow do macierzy globalnej
			for (int j = 0; j < 4; j++)
			{
				int id1 = elem.ID[j];
				P_global[id1 - 1] += element.P[j];
				for (int k = 0; k < 4; k++)
				{
					int id2 = elem.ID[k];
					H_global[id1-1][id2-1] += element.mat_H[j][k];
					C_global[id1-1][id2-1] += element.mat_C[j][k];
				}
			}
		}
		double dt_inv = 1 / global_data.dt;
		//[C] = [C] / dT
		C_global = mat_mult(C_global, dt_inv, grid.nN, grid.nN);
		//[H] = [H] + [C]
		H_global = mat_sum(H_global, C_global, grid.nN, grid.nN);


		runSimulation(H_global, C_global, P_global, t0, global_data.t, global_data.dt);

	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what();
	}
}