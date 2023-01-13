#include"Element.h"

Element::Element(int id1, int id2, int id3, int id4)
{
	ID[0] = id1;
	ID[1] = id2;
	ID[2] = id3;
	ID[3] = id4;
}

Element::Element()
{
	ID[0] = 0;
	ID[1] = 0;
	ID[2] = 0;
	ID[3] = 0;
}

Element::Element(std::vector<int> vec)
{
	for (int i = 0; i < 4; i++)
	{
		ID[i] = vec[i];
	}
}
