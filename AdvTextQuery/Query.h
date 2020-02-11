#pragma once
#include "TextQuery.h"
#include <string>
#include <memory>

class Query_base
{
	friend class Query;
protected:
	using line_no = TextQuery::line_no;
	virtual ~Query_base() = default;
private:
	virtual QueryResult eval(const TextQuery&) const = 0;
	virtual std::string rep() const = 0;
};

class WordQuery : public Query_base
{
	friend class Query;
private:
	WordQuery(const std::string& s) : query_word(s) {}
	QueryResult eval(const TextQuery& t) const;
	std::string rep() const { return query_word; }
	std::string query_word;
};

class Query
{
	extern friend Query operator~(const Query&);
	extern friend Query operator|(const Query&, const Query&);
	extern friend Query operator&(const Query&, const Query&);
public:
	Query(const std::string& s) : q(new WordQuery(s)) {}
	QueryResult eval(const TextQuery& t) const { return q->eval(t); }
	std::string rep() const { return q->rep(); }
private:
	Query(std::shared_ptr<Query_base> query) : q(query) {}
	std::shared_ptr<Query_base> q;
};

class NotQuery : public Query_base
{
	extern friend Query operator~(const Query&);
private:
	NotQuery(const Query& q) : query(q) {}
	QueryResult eval(const TextQuery&) const;
	std::string rep() const { return "~(" + query.rep() + ")"; }
	Query query;
};

class BinaryQuery : public Query_base
{
protected:
	BinaryQuery(const Query& l, const Query& r, std::string s) :
		lhs(l), rhs(r), sym(s) {}
	std::string rep() const { return "(" + lhs.rep() + " " + sym + " " + rhs.rep() + ")"; }
	Query lhs, rhs;
	std::string sym;
};

class AndQuery : public BinaryQuery
{
	extern friend Query operator&(const Query&, const Query&);
private:
	AndQuery(const Query& left, const Query& right) :
		BinaryQuery(left, right, "&") {}
	QueryResult eval(const TextQuery&) const;
};

class OrQuery : public BinaryQuery
{
	extern friend Query operator|(const Query&, const Query&);
private:
	OrQuery(const Query& left, const Query& right) :
		BinaryQuery(left, right, "|") {}
	QueryResult eval(const TextQuery&) const;
};