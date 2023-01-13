#include "GridFile.h"

GridFile::GridFile(std::string path)
{
	file.open(path);
	if (!file.is_open()) throw std::string("Opening file failed");
}

GlobalData GridFile::readGlobalData()
{
	std::vector<double> ret;
	std::string line;
	while (line.find("SpecificHeat"))
	{
		std::getline(file, line);
		if (line.empty()) throw std::string("Reading file failed");
		size_t space_pos = line.find_last_of(" ");			//znajdz spacje w linijce
		std::string value = line.substr(space_pos);			//wytnij to co jest przed spacja
		ret.push_back(std::stod(value));					//przekonwertuj na double
	}
	return GlobalData(ret);
}

Grid GridFile::readGridData()
{
	int i = 0;
	std::string line;
	int nodes_number, elements_number;
	while (i < 2)
	{
		std::getline(file, line);
		if (line.empty()) throw std::string("Reading file failed");
		auto space_pos = line.find_last_of(" ");
		auto value = line.substr(space_pos);
		(i == 0) ? nodes_number = std::stoi(value) : elements_number = std::stoi(value);
		i++;
	}
	Grid ret = Grid(elements_number, nodes_number);
	std::vector<double> X, Y;
	std::vector<int> elemIDs;
	for(int i = 0; i <= ret.nN; i++)
	{
		std::getline(file, line);
		if (line.empty()) throw std::string("Reading file failed");
		line = strip(line);
		if (std::isdigit(line[0]))
		{
			auto delimiter_pos = line.find_first_of(',')+1;		//znajdz pierwszy przecinek
			line = line.substr(delimiter_pos);					//wytnij to co jest przed pierwszym przecinkiem razem z nim
			delimiter_pos = line.find_first_of(",")+1;			//znajdz drugi przecinek
			std::string y = line.substr(delimiter_pos);
			Y.push_back(std::stod(y));							//wpisz do wektora Y to co po przecinku
			line.resize(delimiter_pos-1);						//skróæ string do pozycji pierwszego przecinka
			X.push_back(std::stod(line));						//wpisz do wektora X to co zosta³o
		}
	}
	ret.setNodesCoords(X, Y);
	std::getline(file, line);									//skip jednej linijki z napisem Element
	for (int i = 0; i <= ret.nE; i++)
	{
		std::getline(file, line);
		if (line.empty()) throw std::string("Reading file failed");
		line = strip(line);
		if (std::isdigit(line[0]))
		{
			auto delimiter_pos = line.find_first_of(',') + 1;		//znajdz pierwszy przecinek
			line = line.substr(delimiter_pos);
			delimiter_pos = line.find_last_of(',');					//wytnij to co jest przed pierwszym przecinkiem razem z nim
			while (delimiter_pos != std::string::npos)
			{
				auto value = line.substr(delimiter_pos + 1);
				elemIDs.push_back(std::stoi(value));
				line.resize(delimiter_pos);
				delimiter_pos = line.find_last_of(',');
			}
			elemIDs.push_back(std::stoi(line));
			std::reverse(elemIDs.begin(), elemIDs.end());
			ret.Elem[i] = Element(elemIDs);
			elemIDs.resize(0);
		}
	}
	std::getline(file, line);
	std::vector<int> flags(ret.nN);
	auto delimiter_pos = line.find_last_of(',');
	while (delimiter_pos != std::string::npos)
	{
		auto value = line.substr(delimiter_pos + 1);
		flags[std::stoi(value)-1] = 1;
		line.resize(delimiter_pos);
		delimiter_pos = line.find_last_of(',');
	}
	flags[std::stoi(line) - 1] = 1;
	ret.setNodesFlags(flags);
	return ret;
}

void GridFile::printFile()
{
	std::string line;
	//file.seekg(0, file.beg);
	while (std::getline(file, line))
	{
		std::cout << line << "\n";
	}
}

GridFile::~GridFile()
{
	file.close();
}

