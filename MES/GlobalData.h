#pragma once
#include<vector>
#include<iostream>
class GlobalData
{
public:
	double t, dt, K, spec_heat, density;	//dane globalne - czas, krok czasowy, wsp. przewodzenia, ciep³o w³aœciwe, gêstoœæ
	double alpha, t_init, tot;			//wsp konwekcji i temp otoczenia - tymczasowo tutaj
	GlobalData(std::vector<double> data);
	GlobalData();
	void printData();
};