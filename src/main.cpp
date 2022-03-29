#include "FS.h"
#include <ESP8266WiFi.h>
#include <uri/UriBraces.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

String getMIMEType(String filename) 
{
  if (server.hasArg("download")) 
  {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  }
  return "text/plain";
}

//Streams file content
int handleFileStream(String path) 
{
  String contentType = getMIMEType(path);
  File file = SPIFFS.open(path, "r");
  bool sucess = false;

  //If file exists
  if(!file.isDirectory()) 
  {
    server.streamFile(file, contentType);
    sucess = true;
  }

  file.close();
  return sucess;
}

void save_comment_data (String data)
{
  File file = SPIFFS.open("/comments.csv", "a");
  file.print("\n");
  file.print(data);
  file.close();
}

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    return;
  }

  WiFi.softAP("wifitree", "");
  Serial.print(WiFi.softAPIP());

  server.on("/", HTTP_GET, []()
  {
    handleFileStream("/static/page.html");
  });

  //Static files
  server.on ( UriBraces("/static/{}"), HTTP_GET, []()
  {
      String file_path = "/static/" + server.pathArg(0);

      if (!handleFileStream(file_path))
      {
        server.send(404, "text/plain", "FileNotFound");
      }
  });

  server.on ( "/croqui", HTTP_GET, []()
  {
      handleFileStream("/static/noel.webp");
  });

  server.on ( "/comments", []()
  {
      if(server.method() == HTTP_GET)
      {
        handleFileStream("/comments.csv");
      }
      else if (server.method() == HTTP_POST)
      {
        String post = server.arg(0);
        save_comment_data(post);
        server.send ( 200 );
      }
  });
  
  server.begin();
}

void loop()
{
  server.handleClient();
}