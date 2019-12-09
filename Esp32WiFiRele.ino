/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <SPI.h>
#include <WiFi.h>

const char* ssid     = "OFF";
const char* password = "0123456789";
WiFiServer server(80);

//IPAddress ip(10, 18, 1, 186);

int relay_pin = 13;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //IPAddress = WiFi.localIP();

    server.begin();
    pinMode(13,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(14,OUTPUT);
    pinMode(27,OUTPUT);
    digitalWrite(13,HIGH);
    digitalWrite(12,HIGH);
    digitalWrite(14,HIGH);
    digitalWrite(27,HIGH);
}

int value = 0;

void loop() 
{  
     String req_str;
     WiFiClient client = server. available();  
     if (client) 
     {
           req_str = "";
           boolean currentLineIsBlank = true;
           while (client.connected()) 
           {
                 if (client.available()) 
                 {
                       char c = client.read();    
                       req_str += c;    
                       if(c == '\n' && currentLineIsBlank) 
                       {
                             unsigned int posicao_inicial = req_str.indexOf("GET") + 4;
                             unsigned int posicao_final = req_str.indexOf("HTTP/") - 1;
                             String parametro = req_str.substring(posicao_inicial,posicao_final);
                             if(parametro != "/favicon.ico") 
                             {
                             client.println("HTTP/1.1 200 OK");
                             client.println("Content-Type: text/html");
                             client.println("Connection: close");
                             client.println();
                             client.println("<!DOCTYPE html>");
                             client.println("<html lang=\"pt-br\">");                
                                  client.println("<head>");
                                       client.println("<meta charset= \"UTF-8\"/>");
                                       client.println("<title>Acionamento de relés</title>"); 
                                 client.println("</head>");
                                 client.println("<body>");
                                      client.println("<h1>Sistema de acionamento de relés V.1</h1>");
                                      client.println("<hr/>");
                                      client.println("<h2>Portas digitais</h2>");
                                     // client.println("<p>Porta D2:</p>");
                                      client.println ("<form method=\"get\">");
                                      criarformulario (client, parametro);               
                                      client.println ("</form>");                             
                                client.println ("</body>");
                          client.println ("</html>");
                         }
                         break;
                  }
                  if (c == '\n') 
                  {
                         currentLineIsBlank = true;
                   }
                   else if (c != '\r') 
                   {
                         currentLineIsBlank = false; 
                                } 
              }
          }
        delay(1);
        client.stop();
     }
}      
 
void criarformulario(WiFiClient client_aux, String parametro_aux)
{
 
 
   for(int i = 0; i<2; i++)
   {
       client_aux.println("Porta D" + String(i)+ ":");
       client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\" disabled>PINO NÃO UTILIZADO (DESABILITADO)</br>");
   }   
 
for(int i = 12; i<28; i++)
   {
      
       client_aux.println("Porta D" + String(i)+ " (Relé " + String (i - 1) + ") :");
       if(parametro_aux.indexOf("D" + String(i)+ "=1") > -1)
       {
          client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\" checked> LIGADA</br>");
          digitalWrite(i,LOW);
       }
       else
       {
          client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\"> DESLIGADA</br>");
          digitalWrite(i,HIGH);
       }
   }   
   
   for(int i = 6; i<10; i++)
   {
       client_aux.println("Porta D" + String(i)+ ":");
       client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\" disabled>PINO NÃO UTILIZADO (DESABILITADO)</br>");
   }   
   
 
   for(int i = 10; i<14; i++)
   {
       client_aux.println("Porta D" + String(i)+ ":");
       client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\" disabled>EM USO PELO ETHERNET SHIELD W5100 (DESABILITADO)</br>");
   }
}
