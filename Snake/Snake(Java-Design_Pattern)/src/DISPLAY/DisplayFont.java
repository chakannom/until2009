package DISPLAY;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;

public class DisplayFont {
	private static DisplayFont displayfont = new DisplayFont();;
	public static DisplayFont getInstance() {
		return displayfont;
	}
	public void StringPrint(Graphics gp, String str, int x, int y, Color color, int size) {
		gp.setColor(color);
		gp.setFont(new Font(null, Font.BOLD, size));
		gp.drawString(str, x, y);
	}
}
