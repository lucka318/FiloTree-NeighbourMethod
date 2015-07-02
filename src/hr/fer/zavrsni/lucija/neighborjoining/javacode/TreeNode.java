package hr.fer.zavrsni.lucija.neighborjoining.javacode;

import java.util.LinkedList;
import java.util.List;

public class TreeNode {

	private String name;
	private List<TreeNode> leafs = new LinkedList<TreeNode>();

	public TreeNode(String name) {
		this.name = name;
	}

	public TreeNode() {
		this.name = "X";
	}

	public String getName() {
		return name;
	}

	public void setLeafs(List<TreeNode> leafs) {
		this.leafs = new LinkedList<TreeNode>(leafs);
	}

	public TreeNode getLeaf(int pos) {
		return leafs.get(pos);
	}

	public void removeLeaf(int pos) {
		leafs.remove(pos);
	}
	
	public void removeLeaf(TreeNode node) {
		leafs.remove(node);
	}

	public void addLeaf(TreeNode node) {
		leafs.add(leafs.size(), node);
	}
	
	public int size() {
		return leafs.size();
	}

	@Override
	public String toString() {
		return "TreeNode [name=" + name + ", leafs=" + leafs + "]";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((name == null) ? 0 : name.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		TreeNode other = (TreeNode) obj;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}
	
	
	
}
