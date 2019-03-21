package GAME;
import java.io.*;

public class GameMapLoad {
	public char[][] Load(String fname) {
		int i=0,j=0;
		String str = null;
		char Map[][] = new char[23][23];
		try {
			DataInputStream dis = new DataInputStream(new FileInputStream(new File(fname)));
			for(i=0;i<23;i++) {
				str = dis.readLine();
				for(j=0;j<23;j++) {
					Map[i][j] = str.charAt(j);
				}
			}
			dis.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return Map;
	}
}