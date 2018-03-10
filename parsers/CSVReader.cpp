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
#include "CSVReader.h"
#include <fstream>
#include <algorithm>
using std::string;
using std::cerr;
using std::endl;
using std::getline;
using std::ifstream;
using std::vector;

// trim \r from end (in place)
static inline void rtrim(std::string &s) {
    int l=s.length();
	if(s[l-1] == '\r') s=s.substr(0,l-1);
}

CSVReader::CSVReader(const std::string& file) : CSVReader(file,',','"') {
	
}
CSVReader::CSVReader(const std::string& file, char separator) : CSVReader(file,separator,'"') {
	
}
CSVReader::CSVReader(const std::string& file, char separator, char quoteChar) : separator(separator), quoteChar(quoteChar) {
	in = new ifstream(file);
	
}
CSVReader::~CSVReader()
{
	delete in;
}

bool CSVReader::readNext(std::vector<std::string>& record) {
	if(!in->is_open())
	{
		cerr << "The file is closed" << endl;
		return false;
	}
	record.clear();
	string line;
	if(!getline(*in, line))
	{
		return false;
	}
	rtrim(line);
	while(line.length() > 0) {
		bool beginToken=true;
		bool inQuote=false;
		string currentRecord="";
		int openQuote=-1;
		for(int i=0;i<line.length();i++)
		{
			if(line[i] == quoteChar)
			{
				if(beginToken)
				{
					inQuote=true;
				}
				else if(i < line.length()-1)
				{
					char next=line[i+1];
					if(next == quoteChar)
					{
						currentRecord+=quoteChar;
						i++;
					}
					else if(next == separator && inQuote)
					{
						record.push_back(currentRecord);
						line=line.substr(i+2);
						break;
					}
					else
					{
						cerr << "Parse error" << endl;
						return false;
					}
				}
				else // last char of line
				if(inQuote)
				{
					inQuote=false;
					record.push_back(currentRecord);
					line="";
					break;
				}
				else
				{
					currentRecord+=quoteChar;
					record.push_back(currentRecord);
					line="";
					break;
				}
			}
			else if(line[i] == separator && !inQuote)
			{
				record.push_back(currentRecord);
				line=line.substr(i+1);
				break;
			}
			else
			{
				currentRecord+=line[i];
				if(i == line.length()-1)
				{
					if(inQuote)
					{
						currentRecord+="\n";
						string next;
						if(getline(*in,next))
						{
							rtrim(next);
							line+=next;
						}
						else
						{
							cerr << "Parse warning: reached EOF before quote closing" << endl;
							record.push_back(currentRecord);
							return true;
						}
					}
					else
					{
						record.push_back(currentRecord);
						line="";
						break;
					}
				}
			}
			beginToken=false;
		}
	}
	return true;
}

using std::cout;
int main()
{
	CSVReader reader("./test.csv", ';');
	vector<string> record;
	int nr=0;
	while(reader.readNext(record))
	{
		for(int i=0;i<record.size();i++)
		{
			cout << "record[" << nr << "," << i << "]: " << record[i] << endl;
		}
		nr++;
	}
	return 0;
}
