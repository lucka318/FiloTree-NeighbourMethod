package hr.fer.zavrsni.lucija.start;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import hr.fer.zavrsni.lucija.neighborjoining.NeighborJoining;

public class Main {

	public static void main(String[] args) throws IOException {

		CalculateDistance cd = new CalculateDistance(new File(
				"fasta_files\\genes1.fasta"));

		double[][] mat = { { 0, 7, 8, 11, 13, 16, 13, 17 },
				{ 7, 0, 5, 8, 10, 13, 10, 14 }, { 8, 5, 0, 5, 7, 10, 7, 11 },
				{ 11, 8, 5, 0, 8, 11, 8, 12 }, { 13, 10, 7, 8, 0, 5, 6, 10 },
				{ 16, 13, 10, 11, 5, 0, 9, 13 }, { 13, 10, 7, 8, 6, 9, 0, 8 },
				{ 17, 14, 11, 12, 10, 13, 8, 0 } };
		List<String> n = new ArrayList<String>();
		n.add("1");
		n.add("2");
		n.add("3");
		n.add("4");
		n.add("5");
		n.add("6");
		n.add("7");
		n.add("8");
		NeighborJoining nj = new NeighborJoining(mat, n);
		System.out.println(nj.getRoot());
	}
}
