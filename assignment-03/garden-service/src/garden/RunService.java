package garden;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import io.vertx.core.Vertx;

/*
 * Data Service as a vertx event-loop 
 */
public class RunService {

	public static void main(String[] args) throws Exception{
		final Map<String, String> datas = new ConcurrentHashMap<>();
		datas.put("mode", "AUTO");
		datas.put("temp", "1");
		datas.put("light", "1");
		datas.put("irrigator", "0");
		datas.put("l1", "on");
		datas.put("l2", "on");
		datas.put("l3", "100");
		datas.put("l4", "100");

		final CommChannel channel = new SerialCommChannel("COM5", 250000);
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");
		
		Vertx vertx = Vertx.vertx();
		DataService service = new DataService(8124, datas);
		vertx.deployVerticle(service);
		
		Thread serialComm = new Thread(){
		    public void run(){
		    	while (!Thread.currentThread().isInterrupted()) {
		    	    try {
		    	        String toSend = /*datas.get("mode") + ";" + */datas.get("light") + ";" + datas.get("temp") + ";";
		    	        channel.sendMsg(toSend);
		    	        String msg = channel.receiveMsg();
		    	        System.out.println("sent " + toSend);
		    	        System.out.println("rece " + msg);
		    	        for (String s : msg.split(";")) {
		    	        	if(s.split(":")[0].equals("light")) {
		    	        		continue;
		    	        	}
							datas.put(s.split(":")[0], s.split(":")[1]);
						}
		    	        
		    	        Thread.sleep(100);
		    	    } catch (InterruptedException ex) {
		    	        Thread.currentThread().interrupt();
		    	    }
		    	}
		    }
		};

		serialComm.start();
	}
}