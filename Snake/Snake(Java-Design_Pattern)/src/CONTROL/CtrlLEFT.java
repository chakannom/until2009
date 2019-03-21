package CONTROL;

import SNAKE.Snake;

public class CtrlLEFT implements CtrlCommand {
	private static CtrlLEFT ctrlleft = new CtrlLEFT();
	public static CtrlLEFT getInstance() {
		return ctrlleft;
	}
	public void execute(Snake s) {
		s.xy[0][0]=s.xy[0][0]-1;
	}
}
