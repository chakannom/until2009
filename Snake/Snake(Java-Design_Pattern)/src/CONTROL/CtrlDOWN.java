package CONTROL;

import SNAKE.Snake;

public class CtrlDOWN implements CtrlCommand {
	private static CtrlDOWN ctrldown = new CtrlDOWN();
	public static CtrlDOWN getInstance() {
		return ctrldown;
	}
	public void execute(Snake s) {
		s.xy[1][0]=s.xy[1][0]+1;
	}
}
