package hr.fer.zavrsni.lucija.neighborjoining;

import hr.fer.zavrsni.lucija.neighborjoining.javacode.TreeNode;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

public class NeighborJoining {

	private double distance[][];
	private List<TreeNode> nodes = new LinkedList<TreeNode>();
	private int branchSize = 0;
	private TreeNode root;

	public NeighborJoining(double[][] distance, List<String> list)
			throws IOException {
		this.distance = distance;
		for (String s : list) {
			nodes.add(new TreeNode(s));
		}
		join();
	}

	private void join() {

		double branchLen[][] = new double[distance.length][distance.length];
		root = new TreeNode();
		root.setLeafs(nodes);
		branchSize++;

		printMatrix(distance.length, distance);
		while (branchSize < nodes.size() - 2) { // vidi ovo dal je < ili <=
			int size = root.size();
			for (int i = 0; i < size; i++) {
				for (int j = i + 1; j < size; j++) {
					branchLen[i][j] = caluclateBranchLen(size, i, j);
				}
			}

			printMatrix(size, branchLen);
			System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			double min = branchLen[0][1];
			int iMin = 0, jMin = 1;
			for (int i = 0; i < size - 1; i++) {
				for (int j = i + 1; j < size; j++) {
					if (branchLen[i][j] < min) {
						min = branchLen[i][j];
						iMin = i;
						jMin = j;
					}
				}
			}
			TreeNode first = root.getLeaf(iMin);
			TreeNode second = root.getLeaf(jMin);
			TreeNode neu = new TreeNode(first.getName() + ","
					+ second.getName());
			branchSize++;
			neu.addLeaf(first);
			neu.addLeaf(second);
			root.removeLeaf(first);
			root.removeLeaf(second);
			root.addLeaf(neu);

			double[][] distPom = new double[size - 1][size - 1];
			int iPom = 0, jPom = 0;
			for (int i = 0; i < size; i++) {
				if (i != iMin && i != jMin) {
					for (int j = 0; j < size; j++) {
						if (j != iMin && j != jMin) {
							distPom[iPom][jPom] = distance[i][j];
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
					distPom[iPom][size - 2] = (distance[iMin][j] + distance[jMin][j]) / 2.0;
					iPom++;
					distPom[size - 2][jPom] = (distance[iMin][j] + distance[jMin][j]) / 2.0;
					jPom++;
				}
			}

			printMatrix(size - 1, distPom);

			System.out.println("!!!!!!!!!!!!");
			distance = distPom;
		}
	}

	private void printMatrix(int size, double[][] distPom) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				System.out.print(distPom[i][j] + " ");
			}
			System.out.println();
		}
	}

	private double caluclateBranchLen(int size, int i, int j) {
		double partOne = 0, partTwo = 0;
		double sum = 0;
		for (int k = 0; k < size; k++) {
			if (k != i && k != j) {
				partOne += distance[i][k] + distance[j][k];
			}
		}

		for (int k = 0; k < size - 1; k++) {
			if (k != i && k != j) {
				for (int l = k + 1; l < size; l++) {
					if (l != i && l != j) {
						partTwo += distance[k][l];
					}
				}
			}
		}
		System.out.println(partTwo);
		partOne = partOne / (double) (2 * (size - 2));

		partTwo = partTwo / (double) (size - 2);

		sum = partOne + partTwo + (double) distance[i][j] / 2.0;

		return sum;
	}

	public TreeNode getRoot() {
		return root;
	}
}
