#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

CSVReader::CSVReader()
{
	datas.clear();
}
CSVReader::~CSVReader()
{
	for (int i = FILE_ID::BEGIN_FILE; i <= FILE_ID::END_FILE; i++){
		std::for_each(datas[(FILE_ID)i].begin(), datas[(FILE_ID)i].end(),
			[&](Row& row){row.clear(); Row().swap(row); });
	}
	datas.clear();
	Rows().swap(datas[(FILE_ID)0]);
}

void CSVReader::FileSet(FILE_ID id, const std::string& filename){
	files[id] = filename;
}

bool CSVReader::load(FILE_ID id)
{
	datas[id].clear();
	std::ifstream file(files[id]);
	/* ファイルが読み込めなかったら */
	if (file.bad())
		return false;
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream stream(std::move(line));
		Row row;
		std::string val;
		/* カンマ区切りで読み込む */
		while (std::getline(stream, val, ','))
			row.push_back(val);
		row.shrink_to_fit();
		datas[id].push_back(std::move(row));
	}
	datas[id].shrink_to_fit();
	return true;
}
bool CSVReader::save(FILE_ID id)
{
	std::ofstream file(files[id], std::ios_base::trunc);
	/* ファイルが書き込めなかったら */
	if (file.bad())
		return false;
	for (int row = 0; row < rows(id); ++row)
	{
		int col = 0;
		for (; col < colums(id, row) - 1; ++col)
		{
			file << get(id, row, col) << ",";
		}
		file << get(id, row, col) << "\n";
	}
	return true;
}
int CSVReader::rows(FILE_ID id) const
{
	return datas.at(id).size();
}
int CSVReader::colums(FILE_ID id, unsigned int row) const
{
	if (rows(id) == 0)
		return 0;
	return datas.at(id).at(row).size();
}
const std::string& CSVReader::get(FILE_ID id, unsigned int row, unsigned int col) const
{
	return datas.at(id).at(row).at(col);
}
int CSVReader::geti(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stoi(get(id, row, col));
}
float CSVReader::getf(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stof(get(id, row, col));
}
double CSVReader::getd(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stod(get(id, row, col));
}
long double CSVReader::getld(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stold(get(id, row, col));
}
long CSVReader::getl(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stol(get(id, row, col));
}
long long CSVReader::getll(FILE_ID id, unsigned int row, unsigned int col) const
{
	return std::stoll(get(id, row, col));
}

//Config用。vector<string>の0番目にキーの名前、1番目にその数値を入れる。
void CSVReader::WriteData(FILE_ID id, std::map<int, std::vector<std::string>> str){
	datas.clear();
	for (int i = 0; i < str.size(); i++)
	{
		Row row;
		row.push_back(str[i].at(0));
		row.push_back(str[i].at(1));
		datas[id].push_back(row);
	}
}