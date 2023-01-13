#pragma once
#include"myFunctions.h"
#include"Grid.h"
#include"GlobalData.h"

class GridFile
{
public:
	std::ifstream file;

	GridFile(std::string path);

	void printFile();
	GlobalData readGlobalData();

	Grid readGridData();

	~ GridFile();
};

