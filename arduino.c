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

  client.println("Status: 200");
  client.println("Content-type: text/html");
  // any other header
  client.println(); //mandatory blank line

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Arduino on/off site</title>");
  client.println("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/css/bootstrap.min.css' integrity='sha384-Zug+QiDoJOrZ5t4lssLdxGhVrurbmBWopoEl+M6BdEfwnCJZtKxi1KgxUyJq13dy' crossorigin='anonymous'>");
  client.println("<style>");
  client.println("a {position:absolute;top:50%;left:50%;transform:translateX(-50%) translateY(-50%);}");
  client.println("</style>");
  client.println("</head>");
  if (command.startsWith("on")) {
    digitalWrite(7,HIGH);
    client.println("<a href='/arduino/off'><button type='button' class='btn btn-danger btn-lg'>Off</button></a>");
  }

  else if (command.startsWith("off")) {
    digitalWrite(7,LOW);
    client.println("<a href='/arduino/on'><button type='button' class='btn btn-success btn-lg'>On</button></a>");
  }
  else {
    if (digitalRead(7)) {
      client.println("<a href='/arduino/off'><button type='button' class='btn btn-danger btn-lg'>Off</button></a>");
    }
    else {
      client.println("<a href='/arduino/on'><button type='button' class='btn btn-success btn-lg'>On</button></a>");
    }
  }
  client.println("</body>");
  client.println("</html>");
}