/*
 * Classes.cpp
 *
 *  Created on: 19. 5. 2015.
 *  Author: Lucija Megla
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <math.h>
#include "classes.h"
using namespace std;

ParseFasta::ParseFasta(string file) {
	size = 0;
	parse_fasta(file);
}

void ParseFasta::parse_fasta(string fasta) {
	ifstream input;
	input.open(fasta, ifstream::in);

	if (!input) {
		cerr << "Error opening '" << fasta << "'. Terminating the program."
				<< endl;
		exit(1);
	}

	string line, name, content;

	while (getline(input, line)) {
		if (line.empty()) {
			continue;
		}
		if (line[0] == '>') {
			if (!name.empty() && !content.empty()) {
				keys.push_back(name);
				values.push_back(content);
				size++;
			}
			name = line.substr(1);
		} else if (!name.empty()) {
			if (line.find(' ') != string::npos) { // invalid sequence
				name.clear();
				content.clear();
			} else {
				content.append(line);
			}
		}
	}
	if (!name.empty()) {
		keys.push_back(name);
		values.push_back(content);
		size++;
	}

}

vector<string> ParseFasta::get_keys() {
	return keys;
}

vector<string> ParseFasta::get_values() {
	return values;
}

int ParseFasta::get_size() {
	return size;
}

/*---------------------------------------------------------------------------------------------------------*/

Distance::Distance(vector<string> values, int flag_align) {
	this->flag_align = flag_align;
	int size = values.size();
	distance_matrix.resize(size * size);
	if (flag_align == 0) {
		calculate_distance_levenshtein_1(values); // calculating levenshtein distance without aligning
	} else {
		calculate_distance_levenshtein_2(values); // calculating levenshtein distance with aligning
	}
}

void Distance::calculate_distance_levenshtein_1(vector<string> values) {

	int size = values.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			distance_matrix[i * size + j] = levenshtein(values.at(i),
					values.at(j));
		}
	}
}

void Distance::calculate_distance_levenshtein_2(vector<string> values) {

	SmithWaterman s;
	int size = values.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			s.align(values.at(i), values.at(j));
			string one = s.get_align_one();
			string two = s.get_align_two();
			distance_matrix[i * size + j] = levenshtein(one, two);
		}
	}
}

double Distance::levenshtein(string s1, string s2) {
	double distance = 0;
	int lenS1 = s1.size();
	int lenS2 = s2.size();
	if (lenS1 == 0)
		return lenS2;
	if (lenS2 == 0)
		return lenS1;

	int minLength = 0;
	int maxLength = 0;
	if (lenS1 > lenS2) {
		minLength = lenS2;
		maxLength = lenS1;
	} else {
		maxLength = lenS2;
		minLength = lenS1;
	}

	for (int i = 0; i < minLength; i++) {
		if (s1.at(i) != s2.at(i)) {
			distance += 1;
		}
	}
	distance += maxLength - minLength;
	return distance;
}

vector<double> Distance::get_distance_matrix() {
	return distance_matrix;
}

/***********************************************************************************************************/

SmithWaterman::SmithWaterman() {
}
/*!
 Pseudocode for SWA taken from:
 http://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
 */
void SmithWaterman::align(string s1, string s2) {
	string one = "-" + s1;
	string two = "-" + s2;
	vector<int> matrix; // score matrix
	int gap = 0; // total gap punishment gap = o + (l - 1) * e;
	int match = 2; // reward for alignment
	int o = -2; // punishment for opening a gap
	int l = 0; // counts gaps
	int e = -1; // punishment for extending the gap
	string align_one = "";
	string align_two = "";

	int lenOne = one.length();
	int lenTwo = two.length();

	matrix.reserve(lenOne + lenTwo);
	for (int i = 0; i < lenOne; i++) {
		for (int j = 0; j < lenTwo; j++) {
			matrix.push_back(0);
		}
	}

	for (int i = 0; i < lenOne; i++) {
		for (int j = 0; j < lenTwo; j++) {
			gap = o + (l - 1) * e;
			if (i != 0 && j != 0) {
				if (one.at(i) == two.at(j)) { // alignment
					l = 0;
					matrix[i * lenTwo + j] = max_value(0,
							max_value(
									matrix.at((i - 1) * lenTwo + j - 1) + match,
									max_value(
											matrix.at((i - 1) * lenTwo + j)
													+ gap,
											matrix[i * lenTwo + j - 1] + gap)));
				} else { // gap
					l++;
					matrix[i * lenTwo + j] = max_value(0,
							max_value(matrix.at((i - 1) * lenTwo + j - 1) + gap,
									max_value(
											matrix.at((i - 1) * lenTwo + j)
													+ gap,
											matrix[i * lenTwo + j - 1] + gap)));
				}
			}
		}
	}

	// find the highest value
	int longest = 0;
	int iL = 0, jL = 0;
	for (int i = 0; i < lenOne; i++) {
		for (int j = 0; j < lenTwo; j++) {
			if (matrix[i * lenTwo + j] > longest) {
				longest = matrix[i * lenTwo + j];
				iL = i;
				jL = j;
			}
		}
	}

	// reconstructing the path with two stack
	int i = iL;
	int j = jL;
	stack<string> actions;

	while (i != 0 && j != 0) { // remembering all actions
		// diag case
		int max = max_value(matrix.at((i - 1) * lenTwo + j - 1),
				max_value(matrix.at((i - 1) * lenTwo + j),
						matrix[i * lenTwo + j - 1]));
		if (max == matrix[(i - 1) * lenTwo + j - 1]) {
			actions.push("align");
			i = i - 1;
			j = j - 1;
			// left case
		} else if (max == matrix[i * lenTwo + j - 1]) {
			actions.push("insert");
			j = j - 1;
			// up case
		} else {
			actions.push("delete");
			i = i - 1;
		}
	}

	stack<string> backActions(actions);
	// reconstructing first string
	for (int z = 0; z < one.length(); z++) {
		align_one = align_one + one.at(z);
		if (!actions.empty()) {
			string curAction = actions.top();
			actions.pop();
			if (curAction.compare("insert") == 0) {
				align_one = align_one + "-";
				while (actions.top().compare("insert") == 0) {
					align_one = align_one + "-";
					actions.pop();
				}
			}
		}
	}
	// reconstructing second string
	for (int z = 0; z < two.length(); z++) {
		align_two = align_two + two.at(z);
		if (!backActions.empty()) {
			string curAction = backActions.top();
			backActions.pop();
			if (curAction.compare("delete") == 0) {
				align_two = align_two + "-";
				while (backActions.top().compare("delete") == 0) {
					align_two = align_two + "-";
					backActions.pop();
				}
			}
		}
	}
	this->align_one = align_one;
	this->align_two = align_two;
	return;

}

int SmithWaterman::max_value(int a, int b) {
	if (a >= b) {
		return a;
	} else {
		return b;
	}
}

string SmithWaterman::get_align_one() {
	return this->align_one;
}

string SmithWaterman::get_align_two() {
	return this->align_two;
}

/*--------------------------------------------------------------------------------------------------------*/

Node::Node() {
}

Node::Node(string name) {
	this->name = name;
}

void Node::set_leafs(vector<Node> leafs) {
	this->leafs = leafs;
}

void Node::add_leaf(Node leaf) {
	leafs.push_back(leaf);
}

void Node::remove_leaf(Node leaf) {
	int size = leafs.size();
	for (int i = 0; i < size; i++) {
		if (leafs.at(i).get_name().compare(leaf.get_name()) == 0) {
			leafs.erase(leafs.begin() + i);
			break;
		}
	}
}

string Node::get_name() {
	return name;
}

Node Node::get_leaf(int pos) {
	return leafs.at(pos);
}

int Node::leaf_size() {
	return leafs.size();
}

void Node::set_name(string name) {
	this->name = name;
}

vector<Node> Node::get_leafs() {
	return leafs;
}
/**********************************************************************************************************/

NeighborJoining::NeighborJoining(vector<double> distance,
		vector<string> nodes) {
	join(distance, nodes);
}

void NeighborJoining::join(vector<double> distance, vector<string> nodes) {
	int branch_size = 0;
	vector<double> branchLen; // sum of all branch lengths
	branchLen.resize(distance.size() * distance.size());
	int nodeSize = nodes.size();
	root.set_name("X");
	vector<Node> leafs; //all nodes
	for (int i = 0; i < nodeSize; i++) {
		Node n(nodes.at(i));
		n.set_cost(0);
		leafs.push_back(n);
	}
	
	root.set_leafs(leafs);
	branch_size++;

	while (branch_size < nodeSize - 1) {
		int size = root.leaf_size();

	
		for (int i = 0; i < size; i++) { // calculate total branch len of a tree
			for (int j = 0; j < size; j++) {
				branchLen[i * size + j] = calculate_branch_len(size, i, j,
						distance);
			}
		}

		double min = branchLen[1]; // find minimal sum of branches in the tree
		int iMin = 0, jMin = 1;
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (branchLen[i * size + j] < min) {
					min = branchLen[i * size + j];
					iMin = i;
					jMin = j;
				}
			}
		}

		Node first = root.get_leaf(iMin);
		double c = calculate_cost1(size, iMin, jMin, distance);
		if (first.get_name().find(",")) {
			first.set_cost(c - first.get_cost() / 2.0);
		} else {
			first.set_cost(c);
		}

		Node second = root.get_leaf(jMin);
		c = calculate_cost2(size, iMin, jMin, distance);
		if (second.get_name().find(",")) {
			second.set_cost(c - second.get_cost() / 2.0);
		} else {
			second.set_cost(c);
		}

		Node neu(first.get_name() + "-" + second.get_name()); // create new node
		branch_size++;
		neu.add_leaf(first);
		neu.add_leaf(second);
		neu.set_cost(distance[iMin * size + jMin]);
		root.remove_leaf(first);
		root.remove_leaf(second);
		root.add_leaf(neu);

		vector<double> distPom; // recalculate distance matrix
		distPom.resize((size - 1) * (size - 1));
		int iPom = 0, jPom = 0;
		for (int i = 0; i < size; i++) {
			if (i != iMin && i != jMin) {
				for (int j = 0; j < size; j++) {
					if (j != iMin && j != jMin) {

						distPom[iPom * (size - 1) + jPom] = distance[i * size
								+ j];
						jPom++;
					}
				}
				iPom++;
				jPom = 0;
			}
		}

		iPom = 0;
		jPom = 0;
		for (int j = 0; j < size; j++) {
			if (iMin != j && jMin != j) {
				distPom[iPom * (size - 1) + size - 2] = (distance[iMin * size
						+ j] + distance[jMin * size + j]) / 2.0;
				iPom++;
				distPom[(size - 2) * (size - 1) + jPom] = (distance[iMin * size
						+ j] + distance[jMin * size + j]) / 2.0;
				jPom++;
			}
		}

		distance = distPom;

		if (root.leaf_size() == 2) {
			Node n = root.get_leaf(0);
			n.set_cost(
					(calculate_cost1(size - 1, 0, 1, distance)
							- n.get_cost() / 2.0) / 2.0);
			root.remove_leaf(n);
			root.add_leaf(n);

			Node p = root.get_leaf(0);
			root.remove_leaf(p);
			p.set_cost(n.get_cost());
			root.add_leaf(p);
		}
	}
}

double NeighborJoining::calculate_branch_len(int size, int i, int j,
		vector<double> distance) {
	double partOne = 0, partTwo = 0;
	double sum = 0;
	for (int k = 0; k < size; k++) {
		if (k != i && k != j) {
			partOne += distance[i * size + k] + distance[j * size + k];
		}
	}

	for (int k = 0; k < size - 1; k++) {
		if (k != i && k != j) {
			for (int l = k + 1; l < size; l++) {
				if (l != i && l != j) {
					partTwo += distance[k * size + l];
				}
			}
		}
	}
	partOne = partOne / (double) (2 * (size - 2));

	partTwo = partTwo / (double) (size - 2);

	sum = partOne + partTwo + (double) distance[i * size + j] / 2.0;

	return sum;
}

double NeighborJoining::calculate_cost1(int size, int i, int j,
		vector<double> distance) {
	double partOne = 0;
	for (int k = 0; k < size; k++) {
		if (k != i && k != j) {
			partOne += distance[i * size + k] - distance[j * size + k];
		}
	}
	if (size - 2 != 0) {
		partOne = partOne / (double) (size - 2);
	} else {
		partOne = 0;
	}
	partOne += distance[i * size + j];
	partOne = partOne / 2.0;
	return partOne;
}

double NeighborJoining::calculate_cost2(int size, int i, int j,
		vector<double> distance) {
	double partOne = 0;
	for (int k = 0; k < size; k++) {
		if (k != i && k != j) {
			partOne += -distance[i * size + k] + distance[j * size + k];
		}
	}

	if (size - 2 != 0) {
		partOne = partOne / (double) (size - 2);
	} else {
		partOne = 0;
	}
	partOne += distance[i * size + j];
	partOne = partOne / 2.0;
	return partOne;
}

Node NeighborJoining::get_root() {
	return root;
}