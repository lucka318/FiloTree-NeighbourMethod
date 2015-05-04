package hr.fer.zavrsni.lucija.distance;

public class Levenshtein {

	private String s1;
	private String s2;
	private int distance = 0;

	public Levenshtein(String s1, String s2) {
		this.s1 = s1;
		this.s2 = s2;
	}

	public int calculateDistance() {
		this.distance = 0;
		int lenS1 = s1.length();
		int lenS2 = s2.length();
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
			if (s1.charAt(i) != s2.charAt(i)
					|| (s1.charAt(i) == '-' && s2.charAt(i) == '-')) {
				distance++;
			}
		}
		distance += maxLength - minLength;
		return distance;
	}

}
