#include <uri/UriBraces.h>
#include <WebServer.h>
#include "SPIFFS.h"

#include "globals.h"
#include "constants.h"
#include "config.h"
#include "web.h"

//Network
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
  } else if (filename.endsWith(".json")) {
    return "application/json";
  }

  return "text/plain";
}

//Streams file content
bool handleFileStream(String path) 
{
  String contentType = getMIMEType(path);
  File file = SPIFFS.open(path, "r");
  
  if(!file) 
    return false;
  
  server.streamFile(file, contentType);
  file.close();
  return true;
}

//Redirect to home page file
void handleRoot()
{
  server.send(200, "text/html", "<script type=\"text/javascript\">window.location.href = \"static/index.html\";</script>");
}

void bindAdderesses() 
{
    // Home
    server.on("/", HTTP_GET, handleRoot);

    //Static files
    server.on ( UriBraces(String(STATIC_FILES_PATH) + "{}"), HTTP_GET, []()
    {
        String file_path = STATIC_FILES_PATH + server.pathArg(0);

        if (!handleFileStream(file_path))
          server.send(404, "text/plain", "File not found.");
    });

    server.on("/config", HTTP_GET, []()
    {   
      if (!handleFileStream(CONFIG_FILE_PATH))
        server.send(404, "text/plain", "Could not open config file.");
    });

    server.on("/config", HTTP_POST, []()
    {   
        String jsonConfig = server.arg(0);

        try
        {
          Config newConfig (jsonConfig);

          File configFile = SPIFFS.open(CONFIG_FILE_PATH, "w");
          configFile.print(jsonConfig);
          configFile.close();

          newConfig.setAsGlobal();

          server.send(200);
        }
        catch(const char* msg)
        {
          server.send(406, "text/plain", msg);
        }
    });
}

//Thread function
void serveFoverer (void* data) 
{
  while (true)
    server.handleClient();
}

void startServer ()
{
  bindAdderesses();
  server.begin();

  //Start web sever thread
  xTaskCreatePinnedToCore(
    &serveFoverer, 
    "ClockWebPage", 
    1024 * 30, 
    NULL, 
    0, 
    NULL,
    0); 
}