#include"GlobalData.h"

GlobalData::GlobalData(std::vector<double> data)
{
	t = data[0];
	dt = data[1];
	K = data[2];
	alpha = data[3];
	tot = data[4];
	t_init = data[5];
	density = data[6];
	spec_heat = data[7];
}

GlobalData::GlobalData()
{
	t = 0;
	dt = 0;
	K = 0;
	spec_heat = 0;
	density = 0;
	alpha = 0;
	t_init = 0;
	tot = 0;
}

void GlobalData::printData()
{
	std::cout
		<< "Time: " << t << "\n"
		<< "StepTime: " << dt << "\n"
		<< "Conductivity: " << K << "\n"
		<< "Alfa: " << alpha << "\n"
		<< "Tot: " << tot << "\n"
		<< "InitialTemp: " << t_init << "\n"
		<< "Density: " << density << "\n"
		<< "SpecificHeat: " << spec_heat
		<< "\n";
}
