#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
 
ESP8266WebServer server(80);
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin("Ben's iPhone", "12345678");  //Connect to the WiFi network
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
    delay(500);
    Serial.println("Waiting to connect…");
 
  }
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
 
  server.on("/", HTTP_GET, handleRootPath);    //Associate the handler function to the path
  server.on("/", HTTP_POST, postHandler);
  server.begin();                    //Start the server
  Serial.println("Server listening");
 
}
 
void loop() {
 
  server.handleClient();         //Handling of incoming requests
 
}
 
void handleRootPath() {            //Handler for the rooth path
 
  server.send(200, "text/plain", "Listening sucessfully!");
 
}

void postHandler(){
	server.send(200, "text/plain", "Coordinates recived!");
	DynamicJsonBuffer JSONBuffer;
	JsonObject& parsedJSON = JSONBuffer.parseObject(server.arg("plain"));
	String message = server.arg("plain");
	Serial.println(message);

	if (!parsedJSON.success()) {      //Check for errors in parsing	
		 server.send(200, "text/plain", "Parsing issue, check yo' JSON, SON!");
	    Serial.println("Parsing failed");
	    delay(1000);
	    return;
	}
	else
	{
		server.send(200, "text/plain", "Coordinates recived!");
	}

	int arraySize = parsedJSON["array"].size();   //get size of JSON Array
	Serial.print("\nSize of array: ");
	Serial.println(arraySize);
	 
	for (int i = 0; i < arraySize; i++) { 
		String out = parsedJSON["array"][i];
	    Serial.println(out);
  }

 }