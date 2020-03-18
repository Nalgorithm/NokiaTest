#pragma once

#include<vector>
#include<string>
#include<istream>
#include<fstream>
#include<unordered_map>

std::vector<std::string> split(const std::string);

std::string to_string(const double& d);

class CsvTable {
public:
	const char delim;

	CsvTable(std::istream& instream, const char delim = ',');

	CsvTable(std::string fileName, const char delim = ',');

	void evaluate();

	void write(std::ostream& outstream) const;

	std::vector<std::string>& operator[] (int);	

private:
	std::vector<std::vector<std::string>> data;

	// indexes of columns and rows in table
	std::unordered_map<std::string, uint32_t> columns;
	std::unordered_map<std::string, uint32_t> rows;

	// auxiliary array to determine cycle
	std::vector<std::vector<bool>> visited;

	/* 
	 * parse str to find two cells with operator to evaluate expression
	 * 
	 * Example:
	 * str = =A1+B2
	 * In this case, after parsing, cell1 will be index of A1, cell2 will be index of B2 and op will be equal to +
	 */
	void parseArgs(std::string& str, std::pair<uint32_t, uint32_t>& cell1, std::pair<uint32_t, uint32_t>& cell2, char& op);

	void initializeTable(std::istream&);

	// recursively evaluate particular cell
	double evaluateCell(std::pair<uint32_t, uint32_t> cell);
};

