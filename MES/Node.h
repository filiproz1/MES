#pragma once
class Node
{
public:
	double x, y;	//wsp�rz�dne
	double t;		//temperatura
	int BC;			//warunek brzegowy 0 lub 1

	Node(double X, double Y);
	Node();
	void setBC(int bc);
	void setTemp(double temp);
};