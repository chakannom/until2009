package CONTROL;

import SNAKE.Snake;

public class CtrlNO implements CtrlCommand {
	private static CtrlNO ctrlno = new CtrlNO();
	public static CtrlNO getInstance() {
		return ctrlno;
	}
	public void execute(Snake s) {	
		System.out.println("No Action!!!");
	}
}
