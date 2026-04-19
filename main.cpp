#include "table.h"
#include "tree.h"
#include "iter.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>

int main() {
	//Expr* top = getTree("a=23+45*32*(23+4);a2=a+45-32;b=4;c=a+b;a=a+a;c=a");
	//PrintVisitor v;
	//top->accept(&v);
	//std::cout << "\n";
	//CalcVisitor cv;
	//top->accept(&cv);
	//IterRun r;
	//r.run(top);
	//
	TTable<int, int> tbl;
	tbl.insert(1, 10);
	tbl.insert(2, 10);
	tbl.insert(3, 10);
	tbl.insert(4, 10);
	tbl.insert(5, 10);

	TTable<int, int> tbl2;
	tbl2.insert(1, 1);
	tbl2.insert(232, 1);
	tbl2.insert(123, 1);
	tbl2.insert(124, 1);
	tbl2.insert(123, 1);
	tbl2.insert(0, 1);
	tbl2.insert(2, 1);
	tbl2.insert(3, 1);
	tbl2.insert(4, 1);

	std::cout << tbl.get(1) << std::endl;
	std::cout << tbl.get(2) << std::endl;
	std::cout << tbl.get(3) << std::endl;
	std::cout << tbl.get(4) << std::endl;
	std::cout << tbl.get(5) << std::endl;

	tbl.refresh(tbl2);

	std::cout << tbl.get(1) << std::endl;
	std::cout << tbl.get(2) << std::endl;
	std::cout << tbl.get(3) << std::endl;
	std::cout << tbl.get(4) << std::endl;
	std::cout << tbl.get(5) << std::endl;



	std::ifstream file("../code.txt");

	std::string line;
	std::string fileContent;
	while (std::getline(file, line)) {
		fileContent += line + '\n';
	}
	file.close();

	Parser p;
	Expr* e = p.getTreeFromString(fileContent);
	IterRun r;
	r.run(e);
	//Avt av;
	//av.fromString("a={}2+23+45*32*(23+4);a2+=a+-45-32;b=4;c=a+b;a=a+a;c=a\n");
}
