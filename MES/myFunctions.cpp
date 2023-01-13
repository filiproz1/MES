#include"myFunctions.h"

std::string strip(std::string s)
{
	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
	return s;
}

void print_matrix(double** mat, int n, int m)

{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}
void print_vector(std::vector<double> v)
{
	for (double value : v) std::cout << value << " ";
	std::cout << "\n";
}

std::vector<double> getMinMax(std::vector<double> v) // 0 - min, 1 - max
{
	std::vector<double> res(2);
	double min, max;
	min = max = v[0];
	for (double value : v)
	{
		if (value > max) max = value;
		else if (value < min) min = value;
	}
	res[0] = min; res[1] = max;
	return res;
}

double shape_function(double xi, double eta, int n)
{
	switch (n)
	{
	case 1:
		return 0.25 * (1 + xi) * (1 + eta);
	case 2:
		return 0.25 * (1 - xi) * (1 + eta);
	case 3:
		return 0.25 * (1 - xi) * (1 - eta);
	case 4:
		return 0.25 * (1 + xi) * (1 - eta);
	default:
		throw std::string("shape_function(): bad function index");
	}
}

double dN_dxi(double eta, int n)
{
	switch (n)
	{
	case 1:
		return 0.25 * (1 + eta);
	case 2:
		return -0.25 * (1 + eta);
	case 3:
		return -0.25 * (1 - eta);
	case 4:
		return +0.25 * (1 - eta);
	default:
		throw std::string("dN_dxi(): bad function index");
	}
}

double dN_deta(double xi, int n)
{
	switch (n)
	{
	case 1:
		return 0.25 * (1 + xi);
	case 2:
		return 0.25 * (1 - xi);
	case 3:
		return -0.25 * (1 - xi);
	case 4:
		return -0.25 * (1 + xi);
	default:
		throw std::string("dN_deta(): bad function index");
	}
}

double** mat2_inv(double** mat) // oblicza macierz odwrotna, bez mnozenia przez wyznacznik
{
	double** ret = new double* [2];
	for (int i = 0; i < 2; i++)
	{
		ret[i] = new double[2];
	}
	ret[0][0] = mat[1][1];
	ret[1][1] = mat[0][0];
	ret[0][1] = -mat[0][1];
	ret[1][0] = -mat[1][0];

	return ret;
}
double** mat_mult(double** mat, double c, int n, int m) // mnozy macierz przez stala
{
	double** ret = new double* [n];
	for (int i = 0; i < n; i++)
	{
		ret[i] = new double[m];
		for (int j = 0; j < m; j++)
		{
			ret[i][j] = mat[i][j] * c;
		}
	}
	return ret;
}
double** mat_sum(double** mat1, double** mat2, int n, int m) // dodaje dwie macierze
{
	double** ret = new double* [n];
	for (int i = 0; i < n; i++)
	{
		ret[i] = new double[m];
		for (int j = 0; j < m; j++)
		{
			ret[i][j] = mat1[i][j] + mat2[i][j];
		}
	}
	return ret;
}
double** mat_transpose(double** mat, int n, int m) // transponowanie macierzy
{
	double** ret = new double*[m];
	for (int i = 0; i < n; i++)
	{
		ret[i] = new double[n];
		for (int j = 0; j < m; j++)
		{
			ret[i][j] = mat[j][i];
		}
	}
	return ret;
}
double** mat_vec_to_double(std::vector<std::vector<double>> mat) // konwersja z vector<vector> do double**
{
	double** ret = new double* [mat.size()];
	for (int i = 0; i < mat.size(); i++)
	{
		ret[i] = new double[mat[i].size()];
		for (int j = 0; j < mat[i].size(); j++)
		{
			ret[i][j] = mat[i][j];
		}
	}
	return ret;
}
double** vector_mult(double* v1, double* v2, int n) // transponuje jeden z wektorow, nastepnie mnozy
{
	double** ret = new double* [n];
	for (int i = 0; i < n; i++)
	{
		ret[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			ret[i][j] = v1[i] * v2[j];
		}
	}
	return ret;
}
double* vec_to_double(std::vector<double> v) // konwersja z vector do double*
{
	double* ret = new double[v.size()];
	for (int i = 0; i < v.size(); i++)
	{
		ret[i] = v[i];
	}
	return ret;
}

std::vector<double> mult_mat_vec(double** mat, std::vector<double> vec)
{
	int n = vec.size();
	std::vector<double> ret(n);

	for (int i = 0; i < n; i++)
	{
		ret[i] = 0;
		for (int j = 0; j < n; j++)
		{
			ret[i] += mat[i][j] * vec[j];
		}
	}
	return ret;
}

double norm(std::vector<double> p1, std::vector<double> p2) // odleglosc pomiedzy punktami
{
	if (p1.size() != p2.size() || p1.size() != 2) throw new std::string("norm(): bad size");
	return sqrt(  
		  pow(p1[0] - p2[0], 2)
		+ pow(p1[1] - p2[1], 2)
	);
}

void runSimulation(double** H_global, double** C_global, std::vector<double> P_global, std::vector<double> t0, int t, int dt)
{
	std::vector<double> current_temp = t0;
	std::vector<double> C_t(t0.size()), PC_t(t0.size());
	std::vector<double> minmax(2);
	EquationSolver eqsolv(H_global, t0.size());


	for (int i = dt; i <= t; i += dt)
	{
		//{C_t} = {t0} * [C] / dT
		C_t = mult_mat_vec(C_global, current_temp);
		//{P_t} = {P} + {C_t}
		for (int i = 0; i < C_t.size(); i++)
		{
			PC_t[i] = P_global[i] + C_t[i];
		}


		eqsolv.last_col = PC_t;
		current_temp = eqsolv.solve();
		minmax = getMinMax(current_temp);
		std::cout.precision(8);
		std::cout << "time = " << i << "s\tmin = " << minmax[0] << "\tmax = " << minmax[1] << "\n";
	}
}
