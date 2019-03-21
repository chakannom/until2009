package DISPLAY;

import java.awt.Color;
import java.awt.Graphics;

public class DisCONGRAT extends Display {
	private static DisCONGRAT discongrat = new DisCONGRAT();
	private DisplayFont disfont = DisplayFont.getInstance();
	public static DisCONGRAT getInstance() {
		return discongrat;
	}
	public void disprint(Graphics gp) {
		disfont.StringPrint(gp, "CONGRATULATION!!", 17, 200, Color.GRAY, 55);
	}
}
