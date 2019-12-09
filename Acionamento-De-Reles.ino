
//https://portal.vidadesilicio.com.br/shield-ethernet-w5100-acionamento-remoto-de-reles-com-html/

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 18, 1, 186);
EthernetServer server(80);
 
void setup() 
{
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
}
void loop() 
{  
     String req_str;
     EthernetClient client = server. available();  
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
 
void criarformulario(EthernetClient client_aux, String parametro_aux)
{
 
 
   for(int i = 0; i<2; i++)
   {
       client_aux.println("Porta D" + String(i)+ ":");
       client_aux.println("<input type=\"checkbox\"  name=\"D" + String(i) + "\"  value=\"1\" onclick=\"submit()\" disabled>PINO NÃO UTILIZADO (DESABILITADO)</br>");
   }   
 
for(int i = 2; i<6; i++)
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
