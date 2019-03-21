package DISPLAY;

import SNAKE.Snake;

public class DisFactory {
	public Display displayFactory(String page, Snake s) {
		Display dis = null;
		if(page.equals("intro")) {
			dis = DisINTRO.getInstance();
		}
		else if(page.equals("gameplay")) {
			dis = DisGAMEPLAY.getInstance(s);
		}
		else if(page.equals("congrat")) {
			dis = DisCONGRAT.getInstance();
		}
		return dis;
	}
}
