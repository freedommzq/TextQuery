#include "Query.h"
#include <iostream>

using namespace std;

void runQueries(ifstream &infile)
{
	TextQuery tq(infile);
	//Query q = ~Query("Alice");
	//Query q = Query("hair") | Query("Alice");
	Query q = Query("fiery") & Query("bird") | Query("wind");
	cout << "Executing query for: " << q.rep() << endl;
	print(cout, q.eval(tq)) << endl;
}

int main()
{
	ifstream f("qqq.txt");
	runQueries(f);
	system("pause");
	return 0;
}