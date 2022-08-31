#include <uri/UriBraces.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include "globals.h"
#include "config.h"
#include "web.h"

//Network
const String static_files_path = "/static/";
WebServer server(80);

//MIME Types
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
bool handleFileStream(String path) 
{
  String contentType = getMIMEType(path);
  File file = SPIFFS.open(path, "r");
  bool sucess = false;

  //If file exists
  if(!file.isDirectory()){
    server.streamFile(file, contentType);
    sucess = true;
  }

  file.close();
  return sucess;
}

void writeFile (String data, String path)
{
  File file = SPIFFS.open(path, "a");
  file.print(data);
  file.close();
}

//Redirect to home page file
void handleRoot()
{
  server.send(200, "text/html", "<script type=\"text/javascript\">window.location.href = \"static/index.html\";</script>");
}

//All web paths
void bindAll() 
{
    // Home
    server.on("/", HTTP_GET, handleRoot);

    //Static files
    server.on ( UriBraces(static_files_path + "{}"), HTTP_GET, []()
    {
        String file_path = static_files_path + server.pathArg(0);

        if (!handleFileStream(file_path))
        {
          server.send(404, "text/plain", "FileNotFound");
        }
    });

    server.on("/config", HTTP_POST, []()
    {   
        String data = server.arg(0);

        Serial.print(data);

        Config newConfig;

        if(configFromJson(data, newConfig))
        {
          globalSetConfig(newConfig);
          writeFile(data, CONFIG_FILE_PATH);
          server.send(200);
        }else{
          server.send(406);
        }
    });
}

//Thread function
void serveFoverer (void* data) 
{
  while (true){
    server.handleClient();
  }
}

void startServer ()
{
  bindAll();
  server.begin();

  //Start web sever thread
  xTaskCreatePinnedToCore(
    &serveFoverer, 
    "WebServer", 
    10240, 
    NULL, 
    0, 
    NULL,
    0); 
}