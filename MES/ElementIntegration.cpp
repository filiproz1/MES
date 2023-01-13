#include "ElementIntegration.h"

ElementIntegration::ElementIntegration(std::vector<Node> nodes, double k, double sh, double d, double alpha, double tot)
{
	if (nodes.size() != 4)
		throw std::exception("ElementIntegration - bad size of vector");
	for (auto i = 0; i < 4; i++)
	{
		bool flag = false;
		x.push_back(nodes[i].x);
		y.push_back(nodes[i].y);
		if (nodes[i].BC == 1 && nodes[(i + 1) % nodes.size()].BC == 1)
			edges.push_back(Edge(nodes[i], nodes[(i + 1) % nodes.size()], i, alpha, tot)); // 2 - ilosc punktow calkowania dla Hbc i P
	}
	K = k;
	spec_heat = sh;
	density = d;
	mat_H = new double* [4];
	mat_C = new double* [4];
	P.resize(4);
}

void ElementIntegration::fillTables()
{
	for (int i = 0; i < MACIERZ_PC_2; i++)
	{
		std::vector<double> row_xi_table, row_eta_table, row_shape_f_values;
		for (int j = 0; j < 4; j++)
		{
			row_xi_table.push_back(dN_dxi(punkty_calkowania[MACIERZ_PC - 2][i / MACIERZ_PC],
				j + 1));
			row_eta_table.push_back(dN_deta(punkty_calkowania[MACIERZ_PC - 2][i % MACIERZ_PC],
				j + 1));
			row_shape_f_values.push_back(
				shape_function(punkty_calkowania[MACIERZ_PC - 2][i / MACIERZ_PC],
					punkty_calkowania[MACIERZ_PC - 2][i % MACIERZ_PC], j + 1));
		}
		xi_table.push_back(row_xi_table);
		eta_table.push_back(row_eta_table);
		shape_function_values.push_back(row_shape_f_values);
	}
}

void ElementIntegration::setJakobian()
{
	detJ.resize(MACIERZ_PC_2);
	for (int i = 0; i < MACIERZ_PC_2; i++)
	{
		double dx_dxi = 0, dy_dxi = 0, dx_deta = 0, dy_deta = 0;
		double** jakobian = new double* [2];
		for (int j = 0; j < 2; j++)
		{
			jakobian[j] = new double[2];
		}
		for (int j = 0; j < 4; j++)
		{
			dx_dxi += x[j] * xi_table[i][j];
			dy_dxi += y[j] * xi_table[i][j];
			dx_deta += x[j] * eta_table[i][j];
			dy_deta += y[j] * eta_table[i][j];
		}
		jakobian[0][0] = dx_dxi;
		jakobian[0][1] = dy_dxi;
		jakobian[1][0] = dx_deta;
		jakobian[1][1] = dy_deta;

		jakobiany.push_back(jakobian);
		detJ[i] = jakobian[0][0] * jakobian[1][1] - jakobian[0][1] * jakobian[1][0];
	}
}

void ElementIntegration::setdNdxy()
{
	dN_dx.resize(MACIERZ_PC_2);
	dN_dy.resize(MACIERZ_PC_2);
	for (int i = 0; i < MACIERZ_PC_2; i++) //petla po punktach calkowania
	{
		double** jakobian_inv = mat2_inv(jakobiany[i]); //jakobian odwrotny bez wyznacznika
		jakobian_inv = mat_mult(jakobian_inv, 1 / detJ[i], 2, 2); //mnozenie przez odwrotnosc wyznacznika
		for (int j = 0; j < 4; j++) //petla po wezlach - funkcje ksztaltu
		{
			double dx_value = jakobian_inv[0][0] * xi_table[i][j] +
				jakobian_inv[0][1] * eta_table[i][j];

			double dy_value = jakobian_inv[1][0] * xi_table[i][j] +
				jakobian_inv[1][1] * eta_table[i][j];

			dN_dx[i].push_back(dx_value);
			dN_dy[i].push_back(dy_value);
		}
	}
}

void ElementIntegration::setMatricesPC()
{
	mat_H_PC.resize(MACIERZ_PC_2);
	mat_C_PC.resize(MACIERZ_PC_2);
	for (int i = 0; i < MACIERZ_PC_2; i++)
	{
		double* x = vec_to_double(dN_dx[i]);
		double* y = vec_to_double(dN_dy[i]);

		double** x_mat = vector_mult(x, x, 4);
		double** y_mat = vector_mult(y, y, 4);
		double** final_matrix = mat_sum(x_mat, y_mat, 4, 4);
		mat_H_PC[i] = mat_mult(
			mat_mult(
				final_matrix, K, 4, 4)
			, detJ[i], 4, 4);

		double* N = vec_to_double(shape_function_values[i]);
		double** N_mat = vector_mult(N, N, 4);
		mat_C_PC[i] = mat_mult(N_mat, spec_heat * density, 4, 4);
	}
}

void ElementIntegration::setMatrices()
{
	for (int i = 0; i < 4; i++)
	{
		mat_H[i] = new double[4];
		mat_C[i] = new double[4];
		P[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			mat_H[i][j] = 0;
			mat_C[i][j] = 0;
		}
	}
	for (int i = 0; i < MACIERZ_PC_2; i++)
	{
		double** H_temp = mat_H_PC[i];
		double** C_temp = mat_C_PC[i];
		H_temp = mat_mult(H_temp, 
		wagi[MACIERZ_PC - 2][i % MACIERZ_PC] * wagi[MACIERZ_PC - 2][i / MACIERZ_PC], 4, 4); // temp *= wagi[i]*wagi[j]
		C_temp = mat_mult(C_temp, wagi[MACIERZ_PC - 2][i % MACIERZ_PC] * wagi[MACIERZ_PC - 2][i / MACIERZ_PC] * detJ[i], 4, 4);

		mat_C = mat_sum(mat_C, C_temp, 4, 4); // mat_C += temp
		mat_H = mat_sum(mat_H, H_temp, 4, 4); // mat_H += temp
	}
	for (auto& e : edges)
	{
		mat_H = mat_sum(mat_H, e.Hbc, 4, 4);
		for (int i = 0; i < 4; i++)
		{
			P[i] += e.P[i];
		}
	}
}

void ElementIntegration::setEdges()
{
	for (auto& e : edges)
	{
		e.setPC();
		e.setNvalues();
		e.setDetJ();
		e.setHbc();
		e.setP();
	}
}

void ElementIntegration::setUpElement()
{
	fillTables();
	setJakobian();
	setdNdxy();
	setMatricesPC();
	setMatrices();
}