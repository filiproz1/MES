#include"Grid.h"

Grid::Grid(size_t n_elems, size_t n_nodes)
{
	nE = n_elems;
	nN = n_nodes;
	Elem.resize(nE);
}

void Grid::setNodesCoords(std::vector<double> X, std::vector<double> Y)
{
	for (int i = 0; i < X.size() && i < Y.size(); i++)
	{
		ND.push_back(Node(X[i], Y[i]));
	}
}

void Grid::setNodesFlags(std::vector<int> flags)
{
	for (int i = 0; i < nN; i++)
	{
		ND[i].setBC(flags[i]);
	}
}

void Grid::printNodesInfo()
{
	for (const auto& each : ND)
	{
		std::cout.precision(7);
		std::cout << std::fixed << each.x << "\t" << each.y << "\t" << each.BC << "\n";
	}
}

void Grid::printElementsIDs()
{
	for (const auto& each : Elem)
	{
		for (int i = 0; i < 4; i++)
		{
			std::cout << each.ID[i] << " ";
		}
		std::cout << "\n";
	}
}

