#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h> /* Libraries included for OTA functionalilty */
#include <WiFiUdp.h> /* Libraries included for OTA functionalilty */
#include <ArduinoOTA.h> /* Libraries included for OTA functionalilty */
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <TimeLib.h>
#include <FS.h>
#include "web_pages.h"

/*---------------------------------------------------------------
                           WIFI PARAMETERS
---------------------------------------------------------------*/
const char* ssid = "Flia Ruzzante G";
const char* password = "n0pc-3xif-aa4a";

/*---------------------------------------------------------------
                        GLOBAL DECLARATIONS
---------------------------------------------------------------*/

int AC=0, A_AC=1, LA=0, A_LA=1, LB=0, A_LB=1, LC=0, A_LC=1, V=0, TR=25, AR=50, BR=50, CR=50, PW = 50;
int ACa=0, A_ACa=1, LAa=0, A_LAa=1, LBa=0, A_LBa=1, LCa=0, A_LCa=1, Va=0, TRa=35, ARa=50, BRa=50, CRa=50, PWa = 70;

int LDR_A=0, LDR_B=0, LDR_C=0, V12V=0,  V5V=0, V3V3=0;
float Tact=0;

char values[7];

unsigned short int token=0;

uint8_t sec, minutes, hr, dow, days, mth, years, ctrl =0;

char timeReport = 0;

const char weekDay[7][4]={"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

String registros="";

const String tblEventos[19]={ "Encendido",
                            "Inicio Sesión",
                            "Actualizar Reloj",
                            "Cierre Sesión",
                            "Aire Acondicionado",
                            "Aire Acondicionado Auto",
                            "Luz A",
                            "Luz A Auto",
                            "Luz B",
                            "Luz B Auto",
                            "Luz C",
                            "Luz C Auto",
                            "Ventilación",
                            "Ref. Temperatura",
                            "Ref. Luz A",
                            "Ref. Luz B",
                            "Ref. Luz C",
                            "Potencia Ventilador",
                            "Apagado"};

byte total, num_Pag, pag_Actual;

unsigned long prevTime, currTime = 0;

char valEnviar[21]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //0->AC, 1->LA, 2->LB, 3->LC, 4->PW, 5->V, 6->Temp(int part), 7->Temp(frac part)
                                      //8->dow, 9->days, 10->mth, 11->hr, 12->minutes
                                      //13->A_AC, 14->A_LA, 15->A_LB, 16->A_LC

const int LED = 2;

/*---------------------------------------------------------------
             Declarations for NTP Server communicaton...
---------------------------------------------------------------*/
WiFiUDP Udp;       // Una instancia de UDP para enviar y recibir mensajes
unsigned int localPort = 80;            //  Puerto local para escuchar UDP

//IPAddress timeServer(193,92,150,3);       // time.nist.gov NTP server (fallback)
IPAddress timeServer(13,86,101,172);      //time.windows.com
//IPAddress timeServer(130,206,3,166);    // Red Iris Bilbao
//IPAddress timeServer(130,206,206,248);  // Red Iris Madrid

const int NTP_PACKET_SIZE= 48;   // La hora son los primeros 48 bytes del mensaje
byte packetBuffer[ NTP_PACKET_SIZE];  // buffer para los paquetes

/*---------------------------------------------------------------
                SERVER OBJECT INSTANTIATION
---------------------------------------------------------------*/
ESP8266WebServer server(80);

/*---------------------------------------------------------------
                CLIENT OBJECTS INSTANTIATION
---------------------------------------------------------------*/

HTTPClient http;
WiFiClient client;

void setup() {
   
   Serial.begin(115200);      //inicializa el puerto serie
   delay(10);
   Serial1.begin(2400);      //inicializa el puerto serie
   delay(10);
   //Serial.setDebugOutput(true);  //Activar para ver los mensajes de debug de WiFi...

   SPIFFS.begin();
   
   //pinMode(LED,OUTPUT);       //inicializa el led
   //digitalWrite(LED,HIGH);


//----------------------------------------------------------
// Scan for I2C Devices: (for debug purposes...)
//----------------------------------------------------------
  Serial.println();
  Serial.println("Start I2C scanner ...");
  Serial.print("\r\n");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
      {
      Serial.print("Found I2C Device: ");
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1);
      }
  }
  Serial.print("\r\n");
  Serial.println("Finish I2C scanner");
  Serial.print("Found ");
  Serial.print(count);
  Serial.println(" Device(s).");
//-----------------------------------------------------------  

init_ds1307();  //RTC Init
   
/*---------------------------------------------------------------
                        WiFi INITIALIZATION
---------------------------------------------------------------*/
   //WiFi.mode(WIFI_AP_STA); //WIFI_STA Establece el módulo como cliente wifi, WIFI_AP Establece el módulo como punto de acceso (AP), WIFI_AP_STA Establece el módulo como punto de acceso y cliente WiFi
   
   WiFi.mode(WIFI_STA);  //Inicia como estación para que el puerto OTA tenga la IP asignada por el router

   //conecta con la red wifi
   WiFi.disconnect(); //Se desconecta de cualquier WiFi conectado previamente
   Serial.println("Configuring WiFi Station: "); 
   Serial.print("Connecting to ");
   Serial.println(ssid);
   //____________________________________________________________
   //Para conectar utilizando IP fija descomentar estas líneas:
   // (La configuración es guardada en la memoria flash)
   //____________________________________________________________
   //   IPAddress staticIP(192,168,1,113);
   //   IPAddress gateway(192,168,1,1);
   //   IPAddress subnet(255,255,255,0);
   //   IPAddress dns1(8,8,8,8);
   //   IPAddress dns2(8,8,4,4);
      
   //   WiFi.config(staticIP, gateway, subnet, dns1, dns2);

   //____________________________________________________________
   //  Para volver a utilizar DHCP, descomentar estas líneas:
   //  (Con esto se borra la información en la memoria flash y el modulo se conecta usando DHCP)
   //  (Si no se escriben estos valores a 0,0,0,0 entonces se seguiran utilizando los últimos asignados para IP fija)
   //____________________________________________________________
   //  IPAddress staticIP(0,0,0,0);
   //  IPAddress gateway(0,0,0,0);
   //  IPAddress subnet(0,0,0,0);
   //  WiFi.config(staticIP, gateway, subnet);
   
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {   // Espera por una conexión WiFi
      delay(500);
      Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());
   Serial.print("Subnet Mask: ");
   Serial.println(WiFi.subnetMask());
   Serial.print("Gateway IP: ");
   Serial.println(WiFi.gatewayIP());
   Serial.print("DNS #1: ");
   Serial.print(WiFi.dnsIP(0));
   Serial.print(" DNS #2: ");
   Serial.println(WiFi.dnsIP(1));
   Serial.print("Hostname: ");
   Serial.println(WiFi.hostname());
   Serial.print("Access point MAC address (router): ");
   Serial.println(WiFi.BSSIDstr());
   Serial.print("Signal strength: ");
   Serial.print(WiFi.RSSI());
   Serial.println(" dBm");

   /*OTA Routines:*/
   /**********************************************************************************/
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  /*End of OTA Routines*/
  /**********************************************************************************/

   WiFi.mode(WIFI_AP_STA);  //Cambia a modo AP + Estación, una vez configurada la conexión WiFi al router e iniciado OTA
   Serial.println();
   Serial.println();
   Serial.print("Configuring access point... ");
   Serial.println(WiFi.softAP("IoT Web Server", "12345678") ? "Ready" : "Failed");
   Serial.print("Access Point IP Address: ");
   Serial.println(WiFi.softAPIP());
   Serial.println();
   Serial.println(); 


   Udp.begin(localPort);
   setSyncProvider(NTP_getTimeDate);
   if (timeStatus() != timeSet)
   {
    Serial.println("Can't sync with NTP Server... Retry in 10s...");
   }
   else
   {
    Serial.println("NTP Server sync success!. Next sync in 60min...");
   }

   Serial.println();
   Serial.println();
   Serial.println("Getting last report");
   String date_time=getLastReportDB();
   if ((date_time != "WiFi not connected") && (date_time != "Failed"))
   {
     Serial.println();
     Serial.println();
     Serial.println("Saving last power off event");
     sendPWOffEventDB(date_time);
     guardarEventoApagado(date_time);
   }
   else
    Serial.println("Failed to get last report from database!");

   Serial.println();
   Serial.println();
   Serial.println("Getting config data from database");
   char retry = 0;
   while ((!getConfig_DB()) && (retry <3))
   {
    retry++;
    Serial.print(".");
    delay(500);
   }
   //digitalWrite(LED,LOW);

   
  /*----------------------------------------------------------------------------------------
                                      SERVER PATH DEFINITIONS
  ----------------------------------------------------------------------------------------*/
   server.onNotFound(no_encontrado);
   server.on("/", inicio);
   server.on("/main", principal);
   server.on("/ping", ping);
   server.on("/actBits", actBits);
   server.on("/actRTC", actRTC);
   server.on("/logout", cerrarSesion);
   server.on("/home", homepage);
   server.on("/viewlog", viewlog);
   server.on("/prev", prev);
   server.on("/next", next);
   server.on("/eraseMem", eraseMem);
   
   server.on("/wifiOff", wifiOff);
   server.on("/APwifiOff", APwifiOff);
   server.on("/wifiOn", wifiOn);
   server.on("/APwifiOn", APwifiOn);
   

/*---------------------------------------------------------------
                WEB SERVER INITIALIZATION
---------------------------------------------------------------*/
   server.begin();
   Serial.println();
   Serial.println("Power up event registered. ");
   read_ds1307(&sec, &minutes, &hr, &dow, &days, &mth, &years, &ctrl);
   guardarEvento(0,0);
   Serial.println();
   Serial.println("HTTP Server activated!");
   Serial.println("Listening port 80 for incoming connections...");

   prevTime=millis();
}

void loop() {
  ArduinoOTA.handle();  //Handle OTA Updates!!
  //Manage incoming connections
  server.handleClient();

  currTime=millis();
  if((currTime - prevTime) >= 1000){  //Check if one second has passed
    prevTime=currTime;

    //This routine will execute every second
    //digitalWrite(LED, !digitalRead(LED));

    read_ds1307(&sec, &minutes, &hr, &dow, &days, &mth, &years, &ctrl);

  /*---------------------------------------------------------------
                 SEND TIME UPDATE BY RF TRANSMITTER MODULE
    ---------------------------------------------------------------*/
    String horaAct = "Hora actual: " + String(hour());
    if (minutes<10)
      horaAct += ":0" + String(minute());
    else
      horaAct += ":" + String(minute());
    if (sec<10)
      horaAct += ":0" + String(second());
    else
      horaAct += ":" + String(second());

    const char *envioCharStr = horaAct.c_str();
    char checksum = envioCharStr[strlen(envioCharStr)-1] ^  envioCharStr[strlen(envioCharStr)-2] ^ envioCharStr[strlen(envioCharStr)-3] ^  envioCharStr[strlen(envioCharStr)-4] ^ envioCharStr[strlen(envioCharStr)-5] ^  envioCharStr[strlen(envioCharStr)-6] ^ envioCharStr[strlen(envioCharStr)-7] ^  envioCharStr[strlen(envioCharStr)-8];
    horaAct += " " + String(checksum);
    Serial1.println(horaAct);

      
    Tact = analogRead(A0)/10.23;
  

  if(I2C_PIC_send_byte('Q')){  //This wil be interpreted as a question for pic values
    I2C_PIC_receive_array(6);  //Receive pic data and stores in values[] char array
    
    LDR_A = (int)values[0];
    LDR_B = (int)values[1];
    LDR_C = (int)values[2];
    V3V3 = (int)values[3];
    V5V = (int)values[4];
    V12V = (int)values[5];

    //Make decisions based upon received data and prepares the data to be sent in valEnviar[] char array
    if(A_AC){
      if(Tact > (TR+1))
        valEnviar[0]=1;
      else if(Tact < (TR-1))
        valEnviar[0]=0;
    }
    else if(AC)
      valEnviar[0]=1;
    else
      valEnviar[0]=0;

    if(A_LA){
      if(LDR_A < (AR-5))
        valEnviar[1]=1;
      else if(LDR_A > (AR+5))
        valEnviar[1]=0;
    }
    else if(LA)
      valEnviar[1]=1;
    else
      valEnviar[1]=0;

    if(A_LB){
      if(LDR_B < (BR-5))
        valEnviar[2]=1;
      else if(LDR_B > (BR+5))
        valEnviar[2]=0;
    }
    else if(LB)
      valEnviar[2]=1;
    else
      valEnviar[2]=0;

    if(A_LC){
      if(LDR_C < (CR-5))
        valEnviar[3]=1;
      else if(LDR_C > (CR+5))
        valEnviar[3]=0;
    }
    else if(LC)
      valEnviar[3]=1;
    else
      valEnviar[3]=0;
      
  }
  else{
    Serial.println("Error while sending request to PIC...");  
  }

  valEnviar[4] = (char)PW;
  if (V)
    valEnviar[5]=1;
  else
    valEnviar[5]=0;

  //Separate int part and frac part
  valEnviar[6] = int(Tact);
  valEnviar[7] = int((Tact - valEnviar[6]) * 100);
  if(valEnviar[7] < 10)
    valEnviar[7] *= 10; 

  //8->dow, 9->days, 10->mth, 11->hr, 12->minutes
  valEnviar[8] = dow-1;
  valEnviar[9] = days;
  valEnviar[10] = mth;
  valEnviar[11] = hr;
  valEnviar[12] = minutes;

  valEnviar[13] = A_AC;
  valEnviar[14] = A_LA;
  valEnviar[15] = A_LB;
  valEnviar[16] = A_LC;

  valEnviar[17] = TR;
  valEnviar[18] = AR;
  valEnviar[19] = BR;
  valEnviar[20] = CR;
  
  //Send all data to the PIC:
  Wire.beginTransmission(0x77);
  Wire.write('V');  //This wil be interpreted as a write command, so the PIC will receive the data and store it in a char array
  for(char i=0;i<21;i++)
    Wire.write(valEnviar[i]);
  byte r = Wire.endTransmission();
  if (r != 0){
    Serial.print("Error sending toogle out values: ");
    Serial.println(r);  
  }

  //Report to DB routine:
  timeReport++;
  if (timeReport == 59)  //Check if one minute has passed...
  {
      timeReport = 0;
      while(!sendReportDB())
        Serial.print(".");
      
        
  } 
  }
}
