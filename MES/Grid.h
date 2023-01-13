#pragma once
#include"Node.h"
#include"Element.h"
#include<vector>
#include<iostream>
class Grid
{
public:
	size_t nE, nN;			//liczba elementów i wêz³ów
	std::vector<Node> ND;		//tablica wêz³ów
	std::vector<Element> Elem;	//tablica elementów

	Grid(size_t n_elems, size_t n_nodes);

	void setNodesCoords(std::vector<double> X, std::vector<double> Y);

	void setNodesFlags(std::vector<int> flags);

	void printNodesInfo();

	void printElementsIDs();
};