package GAME;
import java.util.Random;

public class GameFoodLoad extends GameLoadDecorator {
	public GameFoodLoad(GameMapLoad gml) {
		this.gmload = gml;
	}
	public char[][] Load(String fname) {
		int i=0,j=0,foodnum=0;
		char Map[][] = new char[23][23];
		Random rd = new Random(System.currentTimeMillis());
		Map = gmload.Load(fname);
		while(true) {
			i = rd.nextInt(23);
			j = rd.nextInt(23);
			if(Map[i][j]=='0') {
				Map[i][j]='3';
				foodnum++;
			}
			if(foodnum==30) break;
		}
		return Map;
	}
}
