#include <ESP8266WiFi.h>                   
 
const char* ssid = "MEGACABLE-37F0";
const char* password = "**********************";
  
WiFiServer server(80);                   
 
int PinLED = LED_BUILTIN;                  
int estado = LOW;                         
 
void setup() {
  Serial.begin(115200);
 
  pinMode(PinLED, OUTPUT);              
  digitalWrite(PinLED, LOW);            
 
  WiFi.begin(ssid, password);           
  Serial.printf("\n\nConectando a la red: %s\n", WiFi.SSID().c_str());
   while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");                    
  }
  Serial.println("");
  Serial.println("WiFi conectada");       
 
  server.begin();    
  Serial.println("Servidor inicializado");
 
 Serial.printf("\n\nUtiliza esta URL para conectar: http://%s/\n", WiFi.localIP().toString().c_str());
} 
 
void loop() 
{

  WiFiClient client = server.available();  
  if (!client) {
    return;
  }
 

  Serial.println("nuevo cliente");
  while(!client.available()){
    delay(1);
  }
 

  String peticion = client.readStringUntil('\r');
  Serial.println(peticion);
  client.flush();
 

  if (peticion.indexOf('/LED=ON') != -1) 
  {estado = LOW;} 
  if (peticion.indexOf('/LED=OFF') != -1)
  {estado = HIGH;}
 
  digitalWrite(PinLED, estado);
 
 
 client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Control Remoto LED</title>");
  client.println("<style>");
  client.println("body { background-color: #fff; font-family: Arial, sans-serif; text-align: center; }");
  client.println("h1 { color: #333333; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");

  client.println("<h1>Control Remoto LED</h1>");

   if(estado == HIGH) 
 {client.print("<input type='image' src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTT4_7TVS0q4FrcZN9PQo34VrvSCrJKr6xR1_r50NJ4V93WBBsYfdPyKB502l1N2O_JBqY&usqp=CAU' style='display:block; margin:auto' width='10%' onClick=location.href='/LED=ON'>");}
 else 
 {client.print("<input type='image' src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTc6VxdxVmMkUtuXQ1iObjzTjk_Vheeufxd3JBh7dEx26kgyk9Hwt46BXqw3u3Lz10ZSLM&usqp=CAU' style='display:block; margin:auto' width='10%' onClick=location.href='/LED=OFF'>");}
 
 client.println("</html>");
 delay(1);
 Serial.println("Peticion finalizada"); 
 Serial.println("");

}
