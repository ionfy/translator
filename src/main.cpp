#include "tree.h"
#include "iter.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
	std::string input;
	
	if (argc < 2) {
		std::cout << "Insert code filename" << std::endl;
		return -1;
	}
	
	input = argv[1];

	std::ifstream file(input);

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
}
