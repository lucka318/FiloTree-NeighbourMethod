package hr.fer.zavrsni.lucija.swa;


public class SmithWatermanAlgorithm {

	private int matrix[][];
	private Action backtrack[][];
	private int n;
	private int m;
	private int score = 3;
	private int gap;
	private int gapLen = 0;
	private int gapOpen = -4;
	private int gapExtension = -3;

	private String s1;
	private String s2;

	public SmithWatermanAlgorithm(String s1, String s2) {

		this.s1 = s1;
		this.s2 = s2;
		this.n = s1.length() + 1;
		this.m = s2.length() + 1;
		this.matrix = new int[n][m];
		this.backtrack = new Action[n][m];

	}

	public String[] alignmentCalculation() {
		String one = "-" + s1;
		String two = "-" + s2;

		for (int i = 0; i < n; i++) {
			matrix[i][0] = 0;
		}
		for (int j = 0; j < m; j++) {
			matrix[0][j] = 0;
		}

		for (int i = 1; i < n; i++) {
			for (int j = 1; j < m; j++) {
				int upLeft = matrix[i - 1][j - 1];
				int left = matrix[i][j - 1];
				int up = matrix[i - 1][j];
				gap = gapOpen + (gapLen - 1) * gapExtension;
				if (one.charAt(i) == two.charAt(j)) {
					this.gapLen = 0;

					int maxVal = maxValue(
							0,
							maxValue(upLeft + score,
									maxValue(up + gap, left + gap)));
					matrix[i][j] = maxVal;
					if (maxVal == upLeft + score) {
						backtrack[i][j] = Action.MATCH;
					} else if (maxVal == up + gap) {
						backtrack[i][j] = Action.DELETION;
					} else {
						backtrack[i][j] = Action.INSERTION;
					}

				} else {
					gapLen++;
					int maxVal = maxValue(
							0,
							maxValue(upLeft + gap,
									maxValue(up + gap, left + gap)));
					matrix[i][j] = maxVal;
					if (maxVal == upLeft + gap) {
						backtrack[i][j] = Action.MATCH;
					} else if (maxVal == up + gap) {
						backtrack[i][j] = Action.DELETION;
					} else {
						backtrack[i][j] = Action.INSERTION;
					}
				}
			}
		}

		//printMatrix();
		int iMax = 0;
		int jMax = 0;
		int maxValue = matrix[0][0];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (matrix[i][j] > maxValue) {
					maxValue = matrix[i][j];
					iMax = i;
					jMax = j;
				}
			}
		}
	
		String alignOne = new String();
		String alignTwo = new String();
		while (iMax > 0 || jMax > 0) {
			if (backtrack[iMax][jMax] == Action.MATCH) {
				iMax--;
				jMax--;
				alignOne += s1.charAt(iMax);
				alignTwo += s2.charAt(jMax);
			} else if (backtrack[iMax][jMax] == Action.INSERTION) {
				jMax--;
				alignOne += "-";
				alignTwo += s2.charAt(jMax);
			} else {
				iMax--;
				alignOne += s1.charAt(iMax);
				alignTwo += "-";
			}
		}

		String alignOneReverse = new String();
		String alignTwoReverse = new String();
		for (int i = alignOne.length() - 1; i >= 0; i--) {
			alignOneReverse += alignOne.charAt(i);
		}
		for (int i = alignTwo.length() - 1; i >= 0; i--) {
			alignTwoReverse += alignTwo.charAt(i);
		}

		System.out.println(alignOneReverse);
		System.out.println(alignTwoReverse);
		String[] ret = new String[2];
		ret[0] = alignOneReverse;
		ret[1]= alignTwoReverse;
		return ret;
	}

	public void printMatrix() {
		for (int i = 1; i < n; i++) {
			for (int j = 1; j < m; j++) {
				System.out.print(matrix[i][j] + " ");
			}
			System.out.println();
		}

		for (int i = 1; i < n; i++) {
			for (int j = 1; j < m; j++) {
				System.out.print(backtrack[i][j] + " ");
			}
			System.out.println();
		}

	}

	private int maxValue(int a, int b) {
		if (a > b) {
			return a;
		} else {
			return b;
		}
	}
}
