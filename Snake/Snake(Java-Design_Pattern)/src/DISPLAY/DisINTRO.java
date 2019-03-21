package DISPLAY;

import java.awt.Color;
import java.awt.Graphics;

public class DisINTRO extends Display {
	private static DisINTRO disintro = new DisINTRO();
	private DisplayFont disfont = DisplayFont.getInstance();
	public static DisINTRO getInstance() {
		return disintro;
	}
	public void disprint(Graphics gp) {
		disfont.StringPrint(gp, "SNAKE GAME", 25, 200, Color.GRAY, 80);
		disfont.StringPrint(gp, "press ENTER key to START GAME", 125, 400, Color.GRAY, 20);
	}
}
