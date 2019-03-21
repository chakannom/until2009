package GAME;

public abstract class GameLoadDecorator extends GameMapLoad {
	public GameMapLoad gmload;
	public abstract char[][] Load(String fname);
}
