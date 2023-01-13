#include "EquationSolver.h"

EquationSolver::EquationSolver()
{
	matrix = new double*;
	size = 0;
}

EquationSolver::EquationSolver(double** m, int n)
{
	matrix = m;
	size = n;
	result = std::vector<double>(n);
	last_col = std::vector<double>(n);
}

EquationSolver::EquationSolver(double** m, std::vector<double> v)
{
	matrix = m;
	size = v.size();
	last_col = v;
	result = std::vector<double>(size);
}

std::vector<double> EquationSolver::solve()
{
	double** m_temp = new double* [size];
	for (int i = 0; i < size; i++)
	{
		m_temp[i] = new double[size + 1];
		m_temp[i][size] = last_col[i];
		for (int j = 0; j < size; j++)
			m_temp[i][j] = matrix[i][j];
	}


	for (int i = 0; i < size - 1; i++)
	{
		if (m_temp[i][i] == 0) throw std::exception("wyj¹tek: zero na przek¹tnej");
		else
		{
			for (int j = i + 1; j < size; j++)
			{
				double mnoznik = m_temp[j][i] / m_temp[i][i];
				for (int k = i; k < size + 1; k++)
				{
					m_temp[j][k] = m_temp[j][k] - (m_temp[i][k] * mnoznik);
				}
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		last_col[i] = m_temp[i][size];
	}
	result[size - 1] = last_col[size - 1] / m_temp[size - 1][size - 1];
	for (int i = size - 2; i >= 0; i--)
	{
		double suma = 0;
		for (int k = i + 1; k < size; k++)
		{
			suma += m_temp[i][k] * result[k];
		}
		result[i] = (last_col[i] - suma) / m_temp[i][i];
	}
	return result;
}


std::vector<double> EquationSolver::getResult()
{
	return result;
}

