#pragma once
/*
 *  CSVReader
 *  Copyright (c) 2018 Julien Soler
 *                All Rights Reserved
 *
 *  This program is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by Sam Hocevar. See
 *  http://www.wtfpl.net/ for more details.
 */
#include <iostream>
#include <vector>

class CSVReader {
private:
	char separator;
	char quoteChar;
	
	std::ifstream* in;	

public:
	char getSeparator() const;
	void setSeparator(char separator);

	char getQuoteChar() const;
	void setQuoteChar(char quoteChar);

	
	CSVReader(const std::string& file);
	CSVReader(const std::string& file, char separator);
	CSVReader(const std::string& file, char separator, char quoteChar);
	virtual ~CSVReader();
	
	bool readNext(std::vector<std::string>& lineOut);
};
inline char CSVReader::getSeparator() const { return separator; }
inline void CSVReader::setSeparator(char separator) { this->separator = separator; }
inline char CSVReader::getQuoteChar() const { return quoteChar; }
inline void CSVReader::setQuoteChar(char quoteChar) { this->quoteChar = quoteChar; }
