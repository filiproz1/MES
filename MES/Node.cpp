#include"Node.h"


Node::Node(double X, double Y)
{
	x = X;
	y = Y;
	t = 0;
	BC = 0;
}

Node::Node()
{
	x = 0;
	y = 0;
	t = 0;
	BC = 0;
}

void Node::setBC(int bc)
{
	BC = bc;
}

void Node::setTemp(double temp)
{
	t = temp;
}
