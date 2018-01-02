#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>


BridgeServer server;

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  Bridge.begin();
  digitalWrite(7, LOW);

  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  BridgeClient client = server.accept();

  if (client) {
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  String command = client.readString();
  client.write("<html>");
  client.write("<head>");
  client.write("<title>Arduino on/off site</title>");
  client.write("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/css/bootstrap.min.css' integrity='sha384-Zug+QiDoJOrZ5t4lssLdxGhVrurbmBWopoEl+M6BdEfwnCJZtKxi1KgxUyJq13dy' crossorigin='anonymous'>");
  client.write("<stlye>");
  client.write("a {position:absolute;top:50%;left:50%;transform:translateX(-50%) translateY(-50%);}");
  client.write("</stlye>");
  client.write("</head>");
  if (command.startsWith("on")) {
    digitalWrite(7,HIGH);
    client.write("<a href='http://172.27.2.30/arduino/off' target=_blank><button type='button' class='btn btn-danger btn-lg'>Off</button></a>");
  }

  else if (command.startsWith("off")) {
    digitalWrite(7,LOW);
    client.write("<a href='http://172.27.2.30/arduino/on' target=_blank><button type='button' class='btn btn-success btn-lg'>On</button></a>");
  }
  else {
    client.print("Du bist Kacke, es geht nur On/Off");
  }
  client.write("</body>");
  client.write("</html>");
}
