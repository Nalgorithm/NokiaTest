#include<iostream>
#include<fstream>
#include"CsvTable.h"


int main(int argc, char* argv[])
{

	CsvTable t(argv[1]);
	t.evaluate();
	t.write(std::cout);
	return 0;
}
