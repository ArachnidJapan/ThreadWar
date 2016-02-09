#ifndef _CSV_READER_H_
#define _CSV_READER_H_

#include <string>
#include <vector>
#include <map>
#include "FileID.h"

enum CONFIG_DATA{
	MUSIC_VOL,
	SE_VOL,
};

class CSVReader
{
public :
	CSVReader();
	~CSVReader();

	static CSVReader &GetInstance(){
		static CSVReader c;
		return c;
	}

	void FileSet(FILE_ID id, const std::string& filename);
	/**********
	*	@brief 読み込み
	*	@param[in] filename [std::string]
	*	@note -
	*
	*	@return	読み込みに成功したら真
	**********/
	bool load(FILE_ID id);
	/**********
	*	@brief 現在の読み込まれている値の保存
	*	@param[in] filename [std::string]
	*	@note -
	*
	*	@return	保存に成功したら真
	**********/
	bool save(FILE_ID id);
	/**********
	*	@brief 行数取得
	*	@note 表で言う縦
	*
	*	@return	行数を返す
	**********/
	int rows(FILE_ID id) const;
	/**********
	*	@brief 列数取得
	*	@param[in] row [unsigned int] default = 0
	*	@note 表で言う横, 行がなかったら０が返る
	*
	*	@return	列数を返す
	**********/
	int colums(FILE_ID id, unsigned int row = 0) const;
	/**********
	*	@brief 文字列取得
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	読み込まれている文字列を返す
	**********/
	const std::string& get(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得int
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をint型に変換したものを返す
	**********/
	int geti(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得float
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をfloat型に変換したものを返す
	**********/
	float getf(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得double
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をdouble型に変換されたものを返す
	**********/
	double getd(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得long double
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をlong double型に変換されたものを返す
	**********/
	long double getld(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得long
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をlong型に変換されたものを返す
	**********/
	long getl(FILE_ID id, unsigned int row, unsigned int col) const;
	/**********
	*	@brief 数値取得long long
	*	@param[in]	row [unsigned int]
	*	@param[in]	col [unsigned int]
	*	@note -
	*
	*	@return	文字列をlong long型に変換されたものを返す
	**********/
	long long getll(FILE_ID id, unsigned int row, unsigned int col) const;

	void WriteData(FILE_ID id, std::map<int, std::vector<std::string>> str);

private :	/* Copy Guard */
	CSVReader(const CSVReader& other);
	CSVReader& operator = (const CSVReader& other);

private :
	typedef std::vector<std::string> Row;
	typedef std::vector<Row> Rows;
	std::map<FILE_ID, Rows> datas;
	std::map<FILE_ID, std::string> files;
};

#endif