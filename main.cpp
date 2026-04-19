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
