#pragma once
#include<vector>
class Element
{
public:
	int ID[4];		//tablica z numerami wêz³ów
	Element(int id1, int id2, int id3, int id4);
	Element();
	Element(std::vector<int> vec);
};