package garden;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerRequest;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/*
 * Data Service as a vertx event-loop 
 */
public class DataService extends AbstractVerticle {

	private int port;
	private Map<String, String> datas;
	
	public DataService(int port, Map<String, String> datas) {
		this.datas = datas;
		this.port = port;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.get("/api/mode").handler(this::handleSendMode);
		router.get("/api/sendTempLight").handler(this::handleReceiveTempLight);
		router.get("/api/all").handler(this::handleSendAll);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready.");
	}
	
	private void handleReceiveTempLight(RoutingContext routingContext) {
		HttpServerRequest request = routingContext.request();
		log("new datas received");
		datas.put("temp", request.getParam("temp"));
		datas.put("light", request.getParam("light"));
		HttpServerResponse response = routingContext.response();
		response.setStatusCode(200).end();
	}
	
	private void handleSendMode(RoutingContext routingContext) {
		routingContext.response().setStatusCode(200).end(datas.get("mode"));
	}
	
	private void handleSendAll(RoutingContext routingContext) {
		//if(datas.get("temp").equals("5") && datas.get("irrigator").equals("0")) {
			//datas.put("mode", "ALARM"); 
		//}
		
		JsonObject data = new JsonObject();
		data.put("temp", datas.get("temp"));
		data.put("light", datas.get("light"));
		data.put("mode", datas.get("mode"));
		data.put("irrigator", datas.get("irrigator"));
		data.put("l1", datas.get("l1"));
		data.put("l2", datas.get("l2"));
		data.put("l3", datas.get("l3"));
		data.put("l4", datas.get("l4"));
		
		log("sending all datas");
		
		routingContext.response()
			.putHeader("content-type", "application/json")
			.putHeader("Access-Control-Allow-Origin", "*")
			.end(data.encodePrettily());
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		//System.out.println("[DATA SERVICE] "+msg);
	}

}