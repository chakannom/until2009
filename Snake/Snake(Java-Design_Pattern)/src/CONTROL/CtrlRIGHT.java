package CONTROL;

import SNAKE.Snake;

public class CtrlRIGHT implements CtrlCommand {
	private static CtrlRIGHT ctrlright = new CtrlRIGHT();
	public static CtrlRIGHT getInstance() {
		return ctrlright;
	}
	public void execute(Snake s) {	
		s.xy[0][0]=s.xy[0][0]+1;
	}
}
