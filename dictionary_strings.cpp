import java.util.*;
import java.io.*;
public class Solution {
	public static void main(String[] args) throws Exception {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		int numtests = Integer.parseInt(reader.readLine());
		for (int te = 0; te < numtests; te++) {
			String[] p = reader.readLine().split(" ");
			int numwords = Integer.parseInt(p[0]);
			int numspans = Integer.parseInt(p[1]);
			char[] maxletters = new char[26];
			for (int wi = 0; wi < numwords; wi++) {
				char[] lettercount = new char[26];
				String word = reader.readLine();
				for (int i = 0; i < word.length(); i++) {
					int ln = word.charAt(i) - 'a';
					lettercount[ln] += 1;
				}
				for (int i = 0; i < lettercount.length; i++) {
					if (lettercount[i] > maxletters[i]) {
						maxletters[i] = lettercount[i];
					}
				}
			}
			for (int ds = 0; ds < numspans; ds++) {
				char[] lettercount = new char[26];
				String word = reader.readLine();
				for (int i = 0; i < word.length(); i++) {
					int ln = word.charAt(i) - 'a';
					lettercount[ln] += 1;
				}
				int needadd = 0;
				boolean isperfect = true;
				for (int i = 0; i < lettercount.length; i++) {
					int mc = lettercount[i];
					int nc = maxletters[i];
					isperfect = isperfect && mc == nc;
					if (mc < nc) {
						needadd += (nc - mc);
					}
				}
				String a1 = needadd == 0? "Yes":"No";
				String a2;
				if (needadd == 0) {
					a2 = isperfect? "Yes":"No";
				} else {
					a2 = Integer.toString(needadd);
				}
				System.out.println(a1 + " " + a2);
			}
		}
	}
}
