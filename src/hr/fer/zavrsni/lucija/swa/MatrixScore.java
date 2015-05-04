package hr.fer.zavrsni.lucija.swa;

public class MatrixScore {

	private MatrixScore parent;
	private int score;
	private int i;
	private int j;

	public MatrixScore(MatrixScore parent, int score, int i, int j) {
		super();
		this.score = score;
		this.parent = parent;
		this.i = i;
		this.j = j;
	}

	public MatrixScore getParent() {
		return parent;
	}

	public int getScore() {
		return score;
	}

	public void setScore(int score) {
		this.score = score;
	}

	public int getI() {
		return i;
	}

	public int getJ() {
		return j;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + i;
		result = prime * result + j;
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
		MatrixScore other = (MatrixScore) obj;
		if (i != other.i)
			return false;
		if (j != other.j)
			return false;
		return true;
	}

	@Override
	public String toString() {
		return "MatrixScore [i=" + i + ", j=" + j + "]";
	}
	

}
