package CONTROL;

import SNAKE.Snake;

public class CtrlUP implements CtrlCommand {
	private static CtrlUP ctrlup = new CtrlUP();
	public static CtrlUP getInstance() {
		return ctrlup;
	}
	public void execute(Snake s) {
		s.xy[1][0]=s.xy[1][0]-1;
	}
}
