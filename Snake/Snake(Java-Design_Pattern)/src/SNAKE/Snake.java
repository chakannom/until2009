package SNAKE;
import CONTROL.*;
import DISPLAY.DisFactory;
import DISPLAY.Display;
import GAME.*;

import javax.swing.JFrame;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;

public class Snake extends JFrame implements Runnable, KeyListener {
	public int Snakelen = 3;
	private boolean start = false;
	private String fname = "map[0].txt";
	private String page = "intro";
	private String keyname = "RIGHT";
	public char pArr[][] = new char[23][23];
	public char Snake[] = new char [40];
	public int xy[][] = new int[2][40];
	private int tmp_xy[][] = new int[2][40];
	private BufferedImage bufimage = new BufferedImage(582, 602, BufferedImage.TYPE_INT_RGB);
	private GameMapLoad gmload = new GameMapLoad();
	private CtrlFactory cfac = new CtrlFactory();
	private Control control = null;
	private DisFactory dfac = new DisFactory();
	private Display dis = null;
	private Memento memento = new Memento();
	public Snake() {
		this.addKeyListener(this);
		this.setSize(582, 602);
		this.setLocation(300, 90);
		this.setTitle("Snake Game");
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		gmload = new GameFoodLoad(gmload);
		control = cfac.controlFactory(page);
	}
	public void IintGame() {
		memento.InitGame(Snake, xy);
		Snakelen = memento.getSnakelen();
		keyname = memento.getKeyname();
		control = cfac.controlFactory(page);
		pArr = gmload.Load(fname);
	}
	public boolean Clash() {
		int i=0;
		if(pArr[xy[1][0]][xy[0][0]]=='1') {
			return true;
		}
		for(i=1;i<Snakelen;i++) {
			if(xy[0][0]==xy[0][i]&&xy[1][0]==xy[1][i]) {
				return true;
			}
		}
		return false;
	}
	public void run() {
		int i=0;
		int stage_int=0;
		while(page.equals("intro")) {
			try {Thread.sleep(100);} catch (InterruptedException e) {e.printStackTrace();}
		}
		this.IintGame();
		while(page.equals("gameplay")) {
			if(Snakelen==33) {
				start = false;
				if(stage_int==4) {
					page = "congrat";
					repaint();
					break;
				}
				while(true) {
					try {Thread.sleep(100);} catch (InterruptedException e) {e.printStackTrace();}
					if(start) break;
				}
				fname = fname.replace(String.valueOf(stage_int), String.valueOf(++stage_int));
				this.IintGame();
			}
			if(pArr[xy[1][0]][xy[0][0]]=='3') {
				pArr[xy[1][0]][xy[0][0]]='0';
				Snake[Snakelen++]='2';
			}
			if(Clash()) {
				start = false;
				while(true) {
					try {Thread.sleep(100);} catch (InterruptedException e) {e.printStackTrace();}
					if(start) break;
				}
				pArr = gmload.Load(fname);
				this.IintGame();
			}
			try {Thread.sleep(100);} catch (InterruptedException e) {e.printStackTrace();}
			for(i=0;i<Snakelen;i++) {
				tmp_xy[0][i]=xy[0][i];
				tmp_xy[1][i]=xy[1][i];
			}
			for(i=0;i<Snakelen-1;i++) {
				xy[0][i+1]=tmp_xy[0][i];
				xy[1][i+1]=tmp_xy[1][i];
			}
			control.executeCommand(keyname, this);
			repaint();
		}
	}
	public void paint(Graphics g) {
		Graphics gp = bufimage.getGraphics();
		gp.setColor(Color.WHITE);
		gp.fillRect(0, 0, 582, 602);
		dis = dfac.displayFactory(page,this);
		dis.disprint(gp);
		g.drawImage(bufimage, 0, 0, this);
	}
	public void keyPressed(KeyEvent e) {
		switch(e.getKeyCode()) {
			case KeyEvent.VK_UP:
				keyname = "UP";
				break;
			case KeyEvent.VK_DOWN:
				keyname = "DOWN";
				break;
			case KeyEvent.VK_LEFT:
				keyname = "LEFT";
				break;
			case KeyEvent.VK_RIGHT:
				keyname = "RIGHT";
				break;
			case KeyEvent.VK_ENTER:
				page = "gameplay";
				start = true;
				break;
			case KeyEvent.VK_ESCAPE:
				System.exit(0);
				break;
		}
	}
	public void keyReleased(KeyEvent e) {}
	public void keyTyped(KeyEvent e) {}
	public static void main(String[] args) {
		Thread thread = new Thread(new Snake());
		thread.start();
	}
}
