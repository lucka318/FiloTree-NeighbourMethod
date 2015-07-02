#include <string>
#include <map>
#include <list>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

#ifndef CLASSES_H
#define CLASSES_H
/*
 * @class ParseFasta - parses given fasta file
 * @field keys - names of parsed sequences
 * @field values -  values of parsed sequences
 * @field size - number of sequences
 */
class ParseFasta {

private:
	vector<string> keys;
	vector<string> values;
	int size;
	/*!	@function	Parses the content ofa given file. Accepts path to a FASTA file.
	 @param	file - path to a FASTA file.

	 @note	the function sets fields keys, value and size. if the file cannot be opened it will signalize an error and terminate the program.
	 if FASTA file is invalid it will also signalize an error and terminate the program.
	 */
	void parse_fasta(string fasta);
public:
	/*!	@function	Constructor. Accepts path to a FASTA file.
	 @param	file - path to a FASTA file.
	 */
	ParseFasta(string file);
	/*!	@function	Getter for field keys.
	 @param	none
	 return vector<string>
	 */
	vector<string> get_keys();
	/*!	@function	Getter for field values.
	 @param	none
	 return vector<string>
	 */
	vector<string> get_values();
	/*!	@function	Getter for field size.
	 @param	none
	 return int
	 */
	int get_size();
};

/*
 * @class Distance - calculates all distances between sequences
 * @field distance_matrix - distances between all sequences
 * @field flag_align -  determines if sequences need alignment, 0-no, 1-yes
 */
class Distance {
private:
	vector<double> distance_matrix;
	int flag_align;
	/*!	@function	Calculates levenshtein distance between two strings.
	 @param	s1 - first string.
	 @param	s2 - second string.
	 */
	double levenshtein(string s1, string s2);
	/*!	@function	Sets distance_matrix with levenshtein distances.
	 * @param	values - sequences for distance calculation.
	 @note this function calls levenshtein(string s1, string s2) function.
	 */
	void calculate_distance_levenshtein_1(vector<string> values);
	/*!	@function	Sets distance_matrix with levenshtein distances, but first aligns sequences with Smith-Waterman algorithm.
	 * @param	values - sequences for distance calculation.
	 @note this function calls levenshtein(string s1, string s2) function.
	 */
	void calculate_distance_levenshtein_2(vector<string> values);
public:
	/*!	@function	Constructor. Accepts vector of sequences and value for flag_align.
	 @param	value - sequences
	 @param	flag_align - do sequences need to be aligned.
	 */
	Distance(vector<string> value, int flag_align);
	/*!	@function	Getter for field distance_matrix.
	 @param	none
	 return vector<double>
	 */
	vector<double> get_distance_matrix();
};

/*
 * @class SmithWaterman - class for aligning two sequences
 * @field align_one - first aligned sequence;
 * @field align_two - second aligned sequence;
 */
class SmithWaterman {
private:
	string align_one, align_two;
	/*!	@function	Returns max value out of two integers.
	 @param	a - first number
	 @param	b - second number;
	 return maximum value
	 */
	int max_value(int a, int b);
public:
	/*!	@function	Constructor.
	 */
	SmithWaterman();
	/*!	@function	Function that aligns two sequences by SmithWaterman algorithm.
	 @param	s1 - first sequence.
	 @param	s2 - second sequence.
	 @note this function sets fields align_one and align_two
	 */
	void align(string s1, string s2);
	/*!	@function	Getter for field align_one.
	 @param	none
	 return string
	 */
	string get_align_one();
	/*!	@function	Getter for field align_two.
	 @param	none
	 return string
	 */
	string get_align_two();

};

/*
 * @class Node - element of a tree
 * @field name - name of a sequence;
 * @field leafs - references on other nodes;
 */
class Node {
private:
	string name;
	double cost;
	vector<Node> leafs;
public:
	/*!	@functionConstructor.
	 */
	Node();
	/*!	@functionConstructor. Accepts name of a sequence.
	 @param	name - name of a sequence.
	 */
	Node(string name);
	/*!	@function	Sets references to other nodes.
	 @param	leafs - vector of nodes.
	 */
	void set_leafs(vector<Node> leafs);
	/*!	@function	Adds a leaf to leafs.
	 @param	leaf - leaf that needs to be added
	 */
	void add_leaf(Node leaf);
	/*!	@function	Removes a leaf from leafs.
	 @param	leaf -  leaf that needs to be removes.
	 */
	void remove_leaf(Node leaf);
	/*!	@function	Getter for field name.
	 @param	none
	 return string
	 */
	string get_name();
	/*!	@functionGetter for a Node in leafs.
	 @param	none
	 return Node
	 */
	Node get_leaf(int pos);
	/*!	@functionSetter for field name.
	 @param	name - name of a sequence
	 */
	void set_name(string name);
	/*!	@functionGetter for field size.
	 @param	none
	 return int
	 */
	int leaf_size();
	/*!	@functionGetter for field leafs.
	 @param	none
	 return vector<Node>
	 */
	vector<Node> get_leafs();
	/*!	@functionGetter	Getter for field cost.
	 @param	none
	 return vector<Node>
	 */
	double get_cost() {
		return cost;
	}
	/*!	@functionSetter for field cost.
	 @param	cost - lenght to node.
	 */
	void set_cost(double cost) {
		this->cost = cost;
	}
};

/*
 * @class NeighborJoining - class for reconstructing phylogenetic tree;
 * @field nodes - all sequences turned into leafs or nodes of the tree;
 */
class NeighborJoining {
private:
	Node root;
	vector<Node> nodes;
	/*!	@function	provides Neighbor-Joining algorithm.
	 @param	distance - distance matrix.
	 @param	nodes - names of sequences.
	 @note this function sets field root. After calling this function, variable root will have a reference to reconstructed tree
	 */
	void join(vector<double> distance, vector<string> nodes);
	/*!	@function calculates total branch lenght of a tree when the two sequences are to join.
	 @param	size - number of nodes for joining.
	 @param	i - index of the first sequence.
	 @param	j - index of the second sequence.
	 @param distance - distance matrix
	 */
	double calculate_branch_len(int size, int i, int j, vector<double> distance);
	/*!	@function calculates branch lenght between nodes i and (i-j).
	 @param	size - number of nodes for joining.
	 @param	i - index of the first sequence.
	 @param	j - index of the second sequence.
	 @param distance - distance matrix
	 */
	double calculate_cost1(int size, int i, int j, vector<double> distance);
	/*!	@function calculates branch lenght between nodes j and (i-j).
	 @param	size - number of nodes for joining.
	 @param	i - index of the first sequence.
	 @param	j - index of the second sequence.
	 @param distance - distance matrix
	 */
	double calculate_cost2(int size, int i, int j, vector<double> distance);
	double max(double a, double b);
public:
	/*!	@functionConstructor. Accepts name of a sequences and distance matrix for NJA. Calls function join.
	 @param	distance - takes all distances between sequences.
	 @param	nodes - takes all names of sequences.
	 */
	NeighborJoining(vector<double> distance, vector<string> nodes);
	/*!	@functionGetter for field root.
	 @param	none
	 return Node
	 */
	Node get_root();
};

#endif