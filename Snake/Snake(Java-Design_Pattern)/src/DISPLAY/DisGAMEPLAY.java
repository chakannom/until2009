package DISPLAY;

import java.awt.Color;
import java.awt.Graphics;

import SNAKE.Snake;

public class DisGAMEPLAY extends Display {
	private static Snake snake = null;
	private static DisGAMEPLAY disgameplay = new DisGAMEPLAY();
	private DisplayFont disfont = DisplayFont.getInstance();
	public static DisGAMEPLAY getInstance(Snake s) {
		snake = s;
		return disgameplay;
	}
	public void disprint(Graphics gp) {
		int i=0,j=0;
		for(i=0;i<23;i++) {
			for(j=0;j<23;j++) {
				if(snake.pArr[i][j]=='1') {
					gp.setColor(Color.GRAY);
					gp.fillRect(3+(j*25), 23+(i*25), 25, 25);
					gp.setColor(Color.BLACK);
					gp.drawRect(3+(j*25), 23+(i*25), 25, 25);
				}
				else if(snake.pArr[i][j]=='3') {
					gp.setColor(Color.GREEN);
					gp.fillRect(3+(j*25), 23+(i*25), 25, 25);
					gp.setColor(Color.BLACK);
					gp.drawRect(3+(j*25), 23+(i*25), 25, 25);
				}
			}
		}
		gp.setColor(Color.ORANGE);
		for(i=0;i<snake.Snakelen;i++) {
			if(snake.Snake[i]=='4') {
				gp.fillOval(3+(snake.xy[0][i]*25), 23+(snake.xy[1][i]*25), 25, 25);
			}
			else if(snake.Snake[i]=='2') {
				gp.fillRect(3+(snake.xy[0][i]*25), 23+(snake.xy[1][i]*25), 25, 25);
			}
		}
		if(snake.Snakelen==33) {
			disfont.StringPrint(gp, "YOU WIN", 200, 200, Color.BLACK, 30);
			disfont.StringPrint(gp, "NEXT STAGE LOADING..", 110, 300, Color.BLACK, 30);
			disfont.StringPrint(gp, "press ENTER key to START GAME", 45, 400, Color.BLACK, 30);
		}
		if(snake.Clash()) {
			disfont.StringPrint(gp, "GAME OVER", 200, 300, Color.BLACK, 30);
			disfont.StringPrint(gp, "press ENTER key to RESTART GAME", 26, 400, Color.BLACK, 30);
		}
	}
}
