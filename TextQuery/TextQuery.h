#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

using namespace std;

using line_no = vector<string>::size_type;

class QueryResult;

class TextQuery
{
public:
	TextQuery(ifstream&);
	QueryResult query(const string&) const;
private:
	shared_ptr<vector<string>> file;
	map<string, shared_ptr<set<line_no>>> wm;
};

class QueryResult
{
	friend ostream& print(ostream& os, const QueryResult& qr)
	{
		os << qr.sought << " occurs " << qr.lines->size() << " times" << endl;
		for (auto num : *qr.lines)
		{
			os << "\t(line " << num + 1 << ") " << *(qr.file->begin() + num) << endl;
		}
		return os;
	}

public:
	QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f)
		: sought(s), lines(p), file(f) {}
private:
	string sought;
	shared_ptr<set<line_no>> lines;
	shared_ptr<vector<string>> file;
};