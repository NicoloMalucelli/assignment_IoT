package smart_cm;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Smart_cm {

	public static void main(String[] args) throws Exception {
		
		final CommChannel channel = new SerialCommChannel("COM5", 1000000/*9600*/);	
		
		/* attesa necessaria per fare in modo che Arduino completi il reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");	
		
		JFrame frame = new JFrame("Smart coffee machine manager");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setResizable(false);
        frame.setSize(400, 250);

        //Creating the panel at bottom and adding components
        
        JPanel outPanel = new JPanel(); // the panel is not visible in output
        outPanel.setLayout(new BoxLayout(outPanel, BoxLayout.Y_AXIS));
        
        JPanel productsPanel = new JPanel(new GridLayout(1, 3));
        JLabel lblChoc = new JLabel("Chocolate:");
        JLabel lblCoff = new JLabel("Coffee:");
        JLabel lblTea = new JLabel("Tea:");
        productsPanel.add(lblChoc);
        productsPanel.add(lblCoff);
        productsPanel.add(lblTea);
        
        JPanel testPanel = new JPanel();
        JLabel lblTest = new JLabel("Number of self tests:");
        testPanel.add(lblTest);
        
        JPanel statePanel = new JPanel();
        JLabel lblState = new JLabel("State:");
        statePanel.add(lblState);
        
        JPanel assPanel = new JPanel();
        JButton btnRefill = new JButton("Refill");
        JButton btnRestore = new JButton("Restore");
        btnRestore.setEnabled(false);
        assPanel.add(btnRefill);
        assPanel.add(btnRestore);
        
        outPanel.add(productsPanel);
        outPanel.add(testPanel);
        outPanel.add(statePanel);
        outPanel.add(assPanel);
        
        //Adding Components to the frame.
        frame.getContentPane().add(outPanel);
        frame.setVisible(true);
        
        final boolean refill[] = {false};
        final boolean restore[] = {false};

        
        btnRefill.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				refill[0] = true;
				//channel.sendMsg("Ref;");
			}
		});
        
        btnRestore.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				restore[0] = true;
				//channel.sendMsg("Res;");
				btnRestore.setEnabled(false);
			}
		});
        
		while (true){
			
			String toSend = "";
			if(refill[0]) {
				refill[0] = false;
				toSend += "Ref;";
			}
			if(restore[0]) {
				restore[0] = false;
				toSend += "Res;";
			}
			
			channel.sendMsg(toSend);
			
			String msg = channel.receiveMsg();
			for (String s : msg.split(";")) {
				//System.out.println(s);
				String key = s.split(":")[0];
				String value = s.split(":")[1];
				
				if(value.equals("")) {
					value = "0";
				}
				
				switch (key) {
				case "STATE":
					lblState.setText("State: " + value);
					break;
				case "ASS":
					if(value.equals("temp")) {
						btnRestore.setEnabled(true);
					}
					break;
				case "COF":
					lblCoff.setText("Coffee: " + value);
					break;
				case "TEA":
					lblTea.setText("Tea: " + value);
					break;
				case "CHO":
					lblChoc.setText("Chocolate: " + value);
					break;
				case "TEST":
					lblTest.setText("Number of self tests: " + value);
					break;
					
				default:
					break;
				}
				
			}
		}
	}

}
