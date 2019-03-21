package GAME;

public class Memento {
	private char Snake[] = new char[40];
	private int xy[][] = new int[2][40];
	private int Snakelen = 3;
	private String keyname = "RIGHT";
	public Memento() {
		int i=0;
		for(i=0;i<40;i++) {
			this.Snake[i] = 0;
			this.xy[0][i] = -2;
			this.xy[1][i] = -2;
		}
		xy[0][0]=1;
		xy[1][0]=1;
		Snake[0] = '4';
		Snake[1] = '2';
		Snake[2] = '2';
		Snakelen = 3;
	}
	public void InitGame(char[] Snake, int[][] xy) {
		int i=0;
		for(i=0;i<40;i++) {
			Snake[i] = this.Snake[i];
			xy[0][i] = this.xy[0][i];
			xy[1][i] = this.xy[1][i];
		}
	}
	public int getSnakelen() {
		return Snakelen;
	}
	public String getKeyname() {
		return keyname;
	}
}
