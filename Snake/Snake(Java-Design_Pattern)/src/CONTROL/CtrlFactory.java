package CONTROL;

public class CtrlFactory {
	public Control controlFactory(String page) {
		Control control = null;
		CtrlCommand[] ctrlcommand = new CtrlCommand[4];
		if(page.equals("intro")) {
			ctrlcommand[0] = CtrlNO.getInstance();
			ctrlcommand[1] = CtrlNO.getInstance();
			ctrlcommand[2] = CtrlNO.getInstance();
			ctrlcommand[3] = CtrlNO.getInstance();
			control = Control.getInstance(ctrlcommand);
		}
		else if(page.equals("gameplay")) {
			ctrlcommand[0] = CtrlUP.getInstance();
			ctrlcommand[1] = CtrlDOWN.getInstance();
			ctrlcommand[2] = CtrlLEFT.getInstance();
			ctrlcommand[3] = CtrlRIGHT.getInstance();
			control = Control.getInstance(ctrlcommand);
		}
		else if(page.equals("congrat")) {
			ctrlcommand[0] = CtrlNO.getInstance();
			ctrlcommand[1] = CtrlNO.getInstance();
			ctrlcommand[2] = CtrlNO.getInstance();
			ctrlcommand[3] = CtrlNO.getInstance();
			control = Control.getInstance(ctrlcommand);
		}
		return control;
	}
}