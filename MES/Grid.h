#pragma once
#include"Node.h"
#include"Element.h"
#include<vector>
#include<iostream>
class Grid
{
public:
	size_t nE, nN;			//liczba element�w i w�z��w
	std::vector<Node> ND;		//tablica w�z��w
	std::vector<Element> Elem;	//tablica element�w

	Grid(size_t n_elems, size_t n_nodes);

	void setNodesCoords(std::vector<double> X, std::vector<double> Y);

	void setNodesFlags(std::vector<int> flags);

	void printNodesInfo();

	void printElementsIDs();
};