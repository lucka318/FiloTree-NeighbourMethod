package hr.fer.zavrsni.lucija.start;

import hr.fer.zavrsni.lucija.distance.Levenshtein;
import hr.fer.zavrsni.lucija.swa.SmithWaterman;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;

import org.biojava.nbio.core.sequence.ProteinSequence;
import org.biojava.nbio.core.sequence.io.FastaReaderHelper;

public class CalculateDistance {

	private File fasta;
	private double distance[][];
	private List<String> names = new LinkedList<String>();
	
	public CalculateDistance(File f) throws IOException {
		this.fasta = f;
		calculateDist();
	}

	public void calculateDist() throws IOException {
		
		LinkedHashMap<String, ProteinSequence> reads = FastaReaderHelper
				.readFastaProteinSequence(fasta);
		List<ProteinSequence> genes = new ArrayList<ProteinSequence>(
				reads.values());
		names.addAll(reads.keySet());
		int size = genes.size();
		this.distance = new double[size][size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				SmithWaterman swa = new SmithWaterman(genes.get(i).toString(),
						genes.get(j).toString());
				String[] ret = swa.computeSmithWaterman();
				Levenshtein levenDist = new Levenshtein(ret[0], ret[1]);
				int dist = levenDist.calculateDistance();
				distance[i][j] = dist;
			}
		}
	}
	
	public double[][] getDistance() {
		return distance;
	}

	public List<String> getNames() {
		return names;
	}
	
}
