#include "Query.h"
#include <algorithm>
#include <iterator>

inline Query operator~(const Query& q)
{
	return shared_ptr<Query_base>(new NotQuery(q));
}

inline Query operator&(const Query& lhs, const Query& rhs)
{
	return shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

inline Query operator|(const Query& lhs, const Query& rhs)
{
	return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

std::ostream& operator<<(std::ostream& os, const Query& query)
{
	return os << query.rep();
}

QueryResult WordQuery::eval(const TextQuery& t) const
{ 
	return t.query(query_word); 
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
	auto result = query.eval(text);
	auto ret_lines = make_shared<set<line_no>>();
	auto beg = result.begin(), end = result.end();
	auto sz = result.get_file()->size();
	for (size_t i = 0; i < sz; ++i)
	{
		if (beg == end || *beg != i)
			ret_lines->insert(i);
		else if (beg != end)
			++beg;
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
	auto left = lhs.eval(text), right = rhs.eval(text);
	auto ret_lines = make_shared<set<line_no>>();
	set_intersection(left.begin(), left.end(),
		right.begin(), right.end(),
		inserter(*ret_lines, ret_lines->begin()));
	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult OrQuery::eval(const TextQuery& text) const
{
	auto left = lhs.eval(text), right = rhs.eval(text);
	auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
	ret_lines->insert(right.begin(), right.end());
	return QueryResult(rep(), ret_lines, left.get_file());
}