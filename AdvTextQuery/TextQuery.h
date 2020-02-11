#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

using namespace std;

class QueryResult;

class TextQuery
{
public:
	using line_no = vector<string>::size_type;
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
	QueryResult(string s, shared_ptr<set<TextQuery::line_no>> p, shared_ptr<vector<string>> f)
		: sought(s), lines(p), file(f) {}
	set<TextQuery::line_no>::iterator begin() { return lines->begin(); }
	set<TextQuery::line_no>::iterator end() { return lines->end(); }
	shared_ptr<vector<string>> get_file() { return file; }
private:
	string sought;
	shared_ptr<set<TextQuery::line_no>> lines;
	shared_ptr<vector<string>> file;
};