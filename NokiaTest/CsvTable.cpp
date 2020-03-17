#include<sstream>
#include"CsvTable.h"
#include"CsvExceptions.h"

std::vector<std::string> split(const std::string str, char delim) {
	std::istringstream iss(str);
	std::string token;
	std::vector<std::string> res;

	while (std::getline(iss, token, delim))
		res.emplace_back(token);

	return res;
}

std::string to_string(const double& d) {
	std::ostringstream strm;
	strm << d;
	return strm.str();
}


CsvTable::CsvTable(std::istream& instream, const char d) : delim(d) {
	initializeTable(instream);
}

CsvTable::CsvTable(std::string fileName, const char d) : delim(d) {
	std::ifstream fin(fileName);
	initializeTable(fin);
}

void CsvTable::initializeTable(std::istream& instream) {
	if (!instream)
		throw new std::invalid_argument("Bad stream");

	std::string str;
	std::getline(instream, str);
	auto tokens = split(str, ',');
	if (!tokens[0].empty())
		throw new FileFormatException("Data is incorrect");

	// parse columns names and their indexes
	{
		std::vector<std::string> temp;
		for (int i = 0; i < tokens.size(); ++i) {
			temp.emplace_back(tokens[i]);
			if (columns.find(tokens[i]) != columns.end())
				throw RepeatingNamesException("Data has repeating column names");
			columns[tokens[i]] = i;
		}
		data.emplace_back(temp);
	}

	// parse rows names and their indexes; read other data
	for (int i = 1; std::getline(instream, str); ++i) {
		std::vector<std::string> temp;
		tokens = split(str, ',');

		if (rows.find(tokens[0]) != rows.end())
			throw new RepeatingNamesException("Data has repeating row names");

		rows[tokens[0]] = i;

		for (auto item : tokens)
			temp.emplace_back(item);

		data.emplace_back(temp);

		if (data[i].size() != data[0].size())
			throw FileFormatException("Data is incorrrect");
	}

	visited.resize(data.size() - 1);
	for (int i = 0; i < data.size() - 1; ++i) {
		visited[i].resize(data[i + 1].size() - 1, false);
	}

}


void CsvTable::evaluate() {
	for (int i = 1; i < data.size(); ++i) {
		for (int j = 1; j < data[i].size(); ++j) {
			if (data[i][j][0] == '=') {
				evaluateCell(std::make_pair(i, j));
			}
		}
	}
}

double CsvTable::evaluateCell(std::pair<uint32_t, uint32_t> cell) {
	uint32_t i = cell.first, j = cell.second;

	if (data[i][j][0] != '=') {
		return std::stod(data[i][j]);
	}

	if (visited[i - 1][j - 1])
		throw CycleException("Invalid data, cycle exists");
	visited[i - 1][j - 1] = true;

	std::pair<uint32_t, uint32_t> cell1, cell2;
	char op;

	parseArgs(data[i][j], cell1, cell2, op);

	switch (op){
		case '+':
		{
			double res = evaluateCell(cell1) + evaluateCell(cell2);
			data[i][j] = to_string(res);
			return res;
		}
		case '-':
		{
			double res = evaluateCell(cell1) - evaluateCell(cell2);
			data[i][j] = to_string(res);
			return res;
		}
		case '*':
		{
			double res = evaluateCell(cell1) * evaluateCell(cell2);
			data[i][j] = to_string(res);
			return res;
		}
		case '/':
		{
			double res = evaluateCell(cell1) / evaluateCell(cell2);
			data[i][j] = to_string(res);
			return res;
		}

	}

}


void CsvTable::write(std::ostream& outstream) const {
	for (int i = 0; i < data.size(); ++i) {
		for (int j = 0; j < data[i].size(); ++j) {
			outstream << data[i][j];
			if (j != data[i].size() - 1)
				outstream << delim;
		}
		outstream << std::endl;
	}
}

void CsvTable::parseArgs(std::string& str, std::pair<uint32_t, uint32_t>& cell1, std::pair<uint32_t, uint32_t>& cell2, char& op) {
	std::string col1, row1, col2, row2;
	int i;

	// parse string to get columns and rows names
	for (i = 1; str[i] < '0' || str[i] > '9'; ++i)
		col1 += str[i];
	for (i; str[i] >= '0' && str[i] <= '9'; ++i)
		row1 += str[i];

	op = str[i++];

	for (i; str[i] < '0' || str[i] > '9'; ++i)
		col2 += str[i];
	for (i; i < str.size() && str[i] >= '0' && str[i] <= '9'; ++i)
		row2 += str[i];

	// check that this cell exists
	if (rows.find(row1) == rows.end() || rows.find(row2) == rows.end()
		|| columns.find(col1) == columns.end() || columns.find(col2) == columns.end())
		throw NonexistentCellException("Bad cell reference");

	// find indexes, columns and rows
	cell1 = std::make_pair(rows[row1], columns[col1]);
	cell2 = std::make_pair(rows[row2], columns[col2]);

}


std::vector<std::string>& CsvTable::operator[](int index) {
	return data[index];
}


