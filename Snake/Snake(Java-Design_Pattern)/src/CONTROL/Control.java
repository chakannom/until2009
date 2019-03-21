package CONTROL;

import SNAKE.Snake;

public class Control {
	private static CtrlCommand[] ctrlcommand = null;
	private final String keys[] = {"UP", "DOWN", "LEFT" ,"RIGHT"};
	private static Control control = new Control();
	public static Control getInstance(CtrlCommand[] c) {
		ctrlcommand = c;
		return control;
	}
	public void executeCommand(String key, Snake s) {
		int i=0;
		for(i=0;i<keys.length;i++){
			if(key.equals(keys[i])){
				ctrlcommand[i].execute(s);
			}
		}
	}
}
