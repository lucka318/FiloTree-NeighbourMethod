//============================================================================
// Name        : Hello.cpp
// Author      : Lucija Megla
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Neighbor-Joining method in C++, Ansi-style, C++11 std
//============================================================================
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ctime>
#include "classes.h"
using namespace std;

void write_it_out(Node n, ofstream& myFile) {

	vector<Node> leafs = n.get_leafs();
	int size = leafs.size();
	if (size == 0) {
		myFile << n.get_name();
		myFile << ':';
		double c = n.get_cost();
		if(c < 0) {
			c = 0;
		}
		myFile << c;
		return;
	}
	myFile << "(";
	for (int i = 0; i < size; i++) {
		write_it_out(leafs.at(i), myFile);
		if (i == size - 1) {
			myFile << ")";
		} else {
			myFile << ",";
		}
	}

	if (n.get_name().compare("X") != 0) {
		myFile << ":";
		double c = n.get_cost();
		if(c < 0) {
			c = 0;
		}
		myFile << c;
	} else {
		myFile << ";";
	}

}

int main(int argc, char **argv) {

	int flag = 0;
	string ff;

	if (argc < 2) {
		cerr << "Missing parameters. Please provide one FASTA file." << endl;
		exit(1);
	}

	if (argc == 3) {
		string flag = argv[1];
		if (flag.compare("-align") != 0) {
			cerr
					<< "Wrong flag provided. If you want to align your sequences provide flag -align. If your sequences are already aligned, just provide a FASTA file."
					<< endl;
			exit(1);

		} else {
			flag = 1;
			ff = argv[2];
		}

	} else if (argc == 2) {
		ff = argv[1];
	} else {
		cerr
				<< "Wrong parameters or missing parameters. If you want to reconstruct phylogenetic tree provide one FASTA file. If your sequences are not aligned call the program with flag -align and provide one FASTA file."
				<< endl;
		exit(1);
	}

	ParseFasta fasta(ff);
	vector<string> keys = fasta.get_keys();
	vector<string> values = fasta.get_values();
	int size = fasta.get_size();

	//odredivanje distanci

	Distance dist(values, flag);
	vector<double> distance = dist.get_distance_matrix();

	clock_t begin = clock();
	NeighborJoining nj(distance, keys);
	Node n = nj.get_root();
	clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << elapsed_secs << endl;
	ofstream myFile("phyloTreeNJM.nwk");
	write_it_out(n, myFile);
	
	return 0;
}
