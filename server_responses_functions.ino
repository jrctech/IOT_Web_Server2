/*----------------------------------------------------------------------------------------
                           FUNCTIONS TO MANAGE WEB SERVER RESPONSES
----------------------------------------------------------------------------------------*/

void no_encontrado()
{
  if (!HandleFileRead(server.uri()))
  {
    server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    server.send(404,"text/plain","File not found on IoT Web Server");
  }
}

bool HandleFileRead(String path)
{
  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  return false;
}

void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}

String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html"  ;
  else if (filename.endsWith(".html")) return "text/html"  ;
  else if (filename.endsWith(".css")) return "text/css"  ;
  else if (filename.endsWith(".js")) return "application/javascript"  ;
  else if (filename.endsWith(".png")) return "image/png"  ;
  else if (filename.endsWith(".gif")) return "image/gif"  ;
  else if (filename.endsWith(".jpg")) return "image/jpeg"  ;
  else if (filename.endsWith(".ico")) return "image/x-icon"  ;
  else if (filename.endsWith(".xml")) return "text/xml"  ;
  else if (filename.endsWith(".pdf")) return "application/x-pdf"  ;
  else if (filename.endsWith(".zip")) return "application/x-zip"  ;
  else if (filename.endsWith(".gz")) return "application/x-gzip"  ;
  return "text/plain";
}

void inicio()
{
  //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
    server.send(200, "text/html", page_Head_Loc + auth_Body1_Loc + auth_Body2 + page_footer);
  else
    server.send(200, "text/html", page_Head_Rem + auth_Body1_Rem + auth_Body2 + page_footer);
}

void principal()
{
  String user = server.arg("txtUser");
  String pass = server.arg("txtPass");
  Serial.println();
  Serial.println("New login attempt...");
  Serial.println("   User: " + user);
  Serial.println("   Pass: " + pass);
  if((user=="Javier") && (pass=="1234"))
  {
    //Generar token:
    randomSeed(millis());
    token=random(1,65535);
    //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
      server.send(200, "text/html", page_Head_Loc + nav1 + String(token) + nav2 + body1 + panel_Loc + body2 + body3 + body4 + page_footer);
    else
      server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + body1 + panel_Rem + body2 + body3 + body4 + page_footer);
    
    Serial.println("Login event registered. User: " + user + " Pass: " + pass + " Token: " + String(token));
    guardarEvento(1,0);
  }
  else
  {
    //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
      server.send(200, "text/html", page_Head_Loc + auth_Body1_Loc + auth_Page_Denied + auth_Body2 + page_footer);
    else
      server.send(200, "text/html", page_Head_Rem + auth_Body1_Rem + auth_Page_Denied + auth_Body2 + page_footer);
    Serial.println("Login failed!");
  }
}

void ping()
{
  String envio = String(AC) + "," + String(A_AC) + "," +  String(LA) + "," + String(A_LA) + "," + String(LB) + "," + String(A_LB) + "," + String(LC) + "," + String(A_LC) + "," + String(V) + "," + String(TR) + "," + String(AR) + "," + String(BR) + "," + String(CR) + "," + String(PW);
  //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200,"text/html", envio);
}

void actRTC(){
  if(String(token) == server.arg("token"))
    {  
      uint8_t d = (uint8_t)server.arg("A").toInt();
      uint8_t m = (uint8_t)server.arg("B").toInt();
      uint8_t y = (uint8_t)server.arg("C").toInt();
      uint8_t w = (uint8_t)server.arg("D").toInt() + 1;
      uint8_t h = (uint8_t)server.arg("E").toInt();
      uint8_t mn = (uint8_t)server.arg("F").toInt();
      uint8_t s = (uint8_t)server.arg("G").toInt();
      
      if (write_ds1307(d, m, y, w, h, mn, s)){
        //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
        server.send(200, "text/html", "Hora y fecha actualizadas");
        Serial.println("RTC update event registered.");
        guardarEvento(2,0);
      }
      else
      {
        //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
        server.send(200, "text/html", "Error en la actualización del RTC del sistema");
      }
    }
    else
    {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
    }
}

void actBits()
{
    if(String(token) == server.arg("token"))
    {
      //Serial.println("Número de argumentos: " + String(server.args()));
      if(server.arg("AC")=="true")
      {ACa=1;}
      else
      {ACa=0;}
      //Serial.println("AC: " + String(AC));
      
      if(server.arg("A_AC")=="true")
      {A_ACa=1;}
      else
      {A_ACa=0;}
      //Serial.println("A_AC: " + String(A_AC));
      
      if(server.arg("LA")=="true")
      {
        LAa=1;
        //digitalWrite(LED, LOW);  
      }
      else
      {
        LAa=0;
        //digitalWrite(LED, HIGH);  
      }
      //Serial.println("LA: " + String(LA));
      
     if(server.arg("A_LA")=="true")
      {A_LAa=1;}
      else
      {A_LAa=0;}
      //Serial.println("A_LA: " + String(A_LA));
      
      if(server.arg("LB")=="true")
      {LBa=1;}
      else
      {LBa=0;}
      //Serial.println("LB: " + String(LB));
      
      if(server.arg("A_LB")=="true")
      {A_LBa=1;}
      else
      {A_LBa=0;}
      //Serial.println("A_LB: " + String(A_LB));
      
      if(server.arg("LC")=="true")
      {LCa=1;}
      else
      {LCa=0;}
      //Serial.println("LC: " + String(LC));
      
      if(server.arg("A_LC")=="true")
      {A_LCa=1;}
      else
      {A_LCa=0;}
      //Serial.println("A_LC: " + String(A_LC));
      
      if(server.arg("V")=="true")
      {Va=1;}
      else
      {Va=0;}
      //Serial.println("V: " + String(V));
      
      TRa=server.arg("TR").toInt();
      //Serial.println("TR: " + String(TR));
      ARa=server.arg("AR").toInt();
      //Serial.println("AR: " + String(AR));
      BRa=server.arg("BR").toInt();
      //Serial.println("BR: " + String(BR));
      CRa=server.arg("CR").toInt();
      //Serial.println("CR: " + String(CR));
      PWa=server.arg("PW").toInt();
      //Serial.println("PW: " + String(PW));


      if (AC != ACa){
        AC = ACa;
        Serial.println("AC event registered. Value: " + String(AC));
        guardarEvento(4,AC);
      }
      if (A_AC != A_ACa){
        A_AC = A_ACa;
        Serial.println("A_AC event registered. Value: " + String(A_AC));
        guardarEvento(5,A_AC);
      }
      if (LA != LAa){
        LA = LAa;
        Serial.println("LA event registered. Value: " + String(LA));
        guardarEvento(6,LA);
      }
      if (A_LA != A_LAa){
        A_LA = A_LAa;
        Serial.println("A_LA event registered. Value: " + String(A_LA));
        guardarEvento(7,A_LA);
      }
      if (LB != LBa){
        LB = LBa;
        Serial.println("LB event registered. Value: " + String(LB));
        guardarEvento(8,LB);
      }
      if (A_LB != A_LBa){
        A_LB = A_LBa;
        Serial.println("A_LB event registered. Value: " + String(A_LB));
        guardarEvento(9,A_LB);
      }
      if (LC != LCa){
        LC = LCa;
        Serial.println("LC event registered. Value: " + String(LC));
        guardarEvento(10,LC);
      }
      if (A_LC != A_LCa){
        A_LC = A_LCa;
        Serial.println("A_LC event registered. Value: " + String(A_LC));
        guardarEvento(11,A_LC);
      }
      if (V != Va){
        V = Va;
        Serial.println("V event registered. Value: " + String(V));
        guardarEvento(12,V);
      }
      if (TR != TRa){
        TR = TRa;
        Serial.println("TR event registered. Value: " + String(TR));
        guardarEvento(13,TR);
      }
      if (AR != ARa){
        AR = ARa;
        Serial.println("AR event registered. Value: " + String(AR));
        guardarEvento(14,AR);
      }
      if (BR != BRa){
        BR = BRa;
        Serial.println("BR event registered. Value: " + String(BR));
        guardarEvento(15,BR);
      }
      if (CR != CRa){
        CR = CRa;
        Serial.println("CR event registered. Value: " + String(CR));
        guardarEvento(16,CR);
      }
      if (PW != PWa){
        PW = PWa;
        Serial.println("PW event registered. Value: " + String(PW));
        guardarEvento(17,PW);
      }

      String envio = String(Tact) + "," + String(LDR_A) + "," +  String(LDR_B) + "," + String(LDR_C) + "," + String(V12V/10) + "," + String(V12V % 10) + "," + String(V5V/10) + "," + String(V5V % 10) + "," + String(V3V3/10) + "," + String(V3V3 % 10);
      envio += "," + String(weekDay[dow-1]) + "," + String(days) + "," + String(mth) + "," + String(years) + "," + String(hr) + "," + String(minutes) + "," + String(sec);
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200,"text/html", envio);

    }
    else
    {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
    }
}

void cerrarSesion()
{
  if(String(token) == server.arg("token"))
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + auth_Body1_Loc + auth_Page_LoggedOut + auth_Body2 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + auth_Body1_Rem + auth_Page_LoggedOut + auth_Body2 + page_footer);
      token=0;
      Serial.println("Logout event registered.");
      guardarEvento(3,0);
  }
  else
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + auth_Body1_Loc + auth_Body2 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + auth_Body1_Rem + auth_Body2 + page_footer);
  }
}

void homepage()
{
  if(String(token) == server.arg("token"))
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure"); 
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + nav1 + String(token) + nav2 + body1 + panel_Loc + body2 + body3 + body4 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + body1 + panel_Rem + body2 + body3 + body4 + page_footer);
  }
  else
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }
}

void viewlog()
{
  String envio1, envio2="";
  if(String(token) == server.arg("token"))
  {
      total=I2C_eeprom_read_byte(2047);
      num_Pag=total/10;
      pag_Actual=num_Pag;
      if ((total % 10) !=0)
        envio1 = "Mostrando página " + String(pag_Actual+1) + " de " + String(num_Pag+1) + "     Total: " + String(total) + " Eventos";
      else
        envio1 = "Mostrando página " + String(pag_Actual) + " de " + String(num_Pag) + "     Total: " + String(total) + " Eventos";
      
      
      //Enviar Registros de la EEPROM
      if ((total>0) && (total <=254)){
        enviar_Registros(num_Pag);
      }
      else
        registros="";
                        
      if(((total % 10) !=0)){
        if((pag_Actual>0))
          envio2=visor_btnPrev;
      }
      else
        if((pag_Actual>1))
          envio2=visor_btnPrev;
                        
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + nav1 + String(token) + nav2 + visor_Body1_Loc + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + visor_Body3 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + visor_Body1_Rem + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + visor_Body3 + page_footer);
  }
  else
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }
}

void prev()
{
  String envio1, envio2, envio3="";
  if(String(token) == server.arg("token"))
  {
      if (pag_Actual > 0){
        pag_Actual--;
      }
      if ((total % 10) !=0)
        envio1 = "Mostrando página " + String(pag_Actual+1) + " de " + String(num_Pag+1) + "     Total: " + String(total) + " Eventos";
      else
        envio1 = "Mostrando página " + String(pag_Actual) + " de " + String(num_Pag) + "     Total: " + String(total) + " Eventos";
      
      
      //Enviar Registros de la EEPROM
      if ((total>0) && (total <=254)){
        enviar_Registros(pag_Actual);
      }
      else
        registros="";
                        
      if(((total % 10) !=0)){
        if((pag_Actual>0))
          envio2=visor_btnPrev;
      }
      else
        if((pag_Actual>1))
          envio2=visor_btnPrev;

      if(pag_Actual<num_Pag)
        envio3=visor_btnNext;
                        
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + nav1 + String(token) + nav2 + visor_Body1_Loc + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + envio3 + visor_Body3 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + visor_Body1_Rem + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + envio3 + visor_Body3 + page_footer);
  }
  else
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }  
}

void next()
{
  String envio1, envio2, envio3="";
  if(String(token) == server.arg("token"))
  {
      if (pag_Actual < num_Pag){
        pag_Actual++;
      }
      if ((total % 10) !=0)
        envio1 = "Mostrando página " + String(pag_Actual+1) + " de " + String(num_Pag+1) + "     Total: " + String(total) + " Eventos";
      else
        envio1 = "Mostrando página " + String(pag_Actual) + " de " + String(num_Pag) + "     Total: " + String(total) + " Eventos";
      
      
      //Enviar Registros de la EEPROM
      if ((total>0) && (total <=254)){
        enviar_Registros(pag_Actual);
      }
      else
        registros="";
                        
      if(((total % 10) !=0)){
        if((pag_Actual>0))
          envio2=visor_btnPrev;
      }
      else
        if((pag_Actual>1))
          envio2=visor_btnPrev;

      if(pag_Actual<num_Pag)
        envio3=visor_btnNext;
                        
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      if((server.hostHeader() == WiFi.softAPIP().toString()) || (server.hostHeader() == WiFi.localIP().toString()))
        server.send(200, "text/html", page_Head_Loc + nav1 + String(token) + nav2 + visor_Body1_Loc + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + envio3 + visor_Body3 + page_footer);
      else
        server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + visor_Body1_Rem + visor_Info1 + envio1 + visor_Info2 + visor_Table_Head + registros + visor_Body2 +  envio2 + visor_btnBorrar + envio3 + visor_Body3 + page_footer);
  }
  else
  {
      //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }  
}


void eraseMem()
{
  if(String(token) == server.arg("token"))
  {
    I2C_eeprom_write_byte(2047,0);
    Serial.println("EEPROM Memory cleared!.");
    //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    server.send(200, "text/html", "Memoria de registros borrada");
  }
  else
  {
    //server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    server.send(200, "text/html", "Token no valido");
  }


  if (WiFi.status() == WL_CONNECTED)    //Don't try if WiFi is not connected 
  {                                     //Don't wait for WiFi connection
    Serial.println("Deleting events from database...");
    if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/delete_events.php")){
      int httpCode=-1;
      char retry=0;
      while((httpCode != 200) && (retry<3))
      {
        retry++;
        httpCode = http.GET(); 
      }
      String r = http.getString();
      Serial.println("HTTP Code: " + String(httpCode) + " " + r);
      http.end();
    }
  }

}

void wifiOff()
{
  WiFi.disconnect();  
  server.send(200, "text/html", "WiFi station disconnected!!");
}

void APwifiOff()
{
  WiFi.softAPdisconnect();  
  WiFi.mode(WIFI_STA);
  ArduinoOTA.begin();
  server.send(200, "text/html", "WiFi AP disconnected!!");
}

void wifiOn()
{
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {   // Espera por una conexión WiFi
      delay(500);
      Serial.print(".");
   }
   server.send(200, "text/html", "WiFi station connected!!");
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
}

void APwifiOn()
{
  Serial.print("Configuring access point... ");
  Serial.println(WiFi.softAP("IoT Web Server", "12345678") ? "Ready" : "Failed");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
  server.send(200, "text/html", "WiFi AP ready!!");
}

void enviar_Registros(byte pag){
    unsigned int k;
    byte dia, mes, anio, horas, minutos, segundos, val;
    String ev="";
    registros="";
    if(pag>=1){  //Hay al menos una página completa
        for (k=(10*(pag-1) + (total % 10)); k<(10*pag + (total % 10));k++){
            
            dia=I2C_eeprom_read_byte(8*k);
            mes=I2C_eeprom_read_byte(8*k + 1);
            anio=I2C_eeprom_read_byte(8*k + 2);
            horas=I2C_eeprom_read_byte(8*k + 3);
            minutos=I2C_eeprom_read_byte(8*k + 4);
            segundos=I2C_eeprom_read_byte(8*k + 5);
            ev = tblEventos[I2C_eeprom_read_byte(8*k + 6)];
            val=I2C_eeprom_read_byte(8*k + 7);

            registros += "<tr><td>" + String(k+1) + "</td><td>" + String(dia) +  "/" + String(mes) +  "/20" + String(anio) + "</td><td>" + String(horas) +  ":" + String(minutos) +  ":" + String(segundos) + "</td><td>" + ev + "</td><td>" + String(val) + "</td></tr>";

        }
    }
    else{ // No hay una página completa (hace lo mismo pero desde k=0 hasta el total % 20)
        for (k=0; k<(total % 10);k++){
            
            dia=I2C_eeprom_read_byte(8*k);
            mes=I2C_eeprom_read_byte(8*k + 1);
            anio=I2C_eeprom_read_byte(8*k + 2);
            horas=I2C_eeprom_read_byte(8*k + 3);
            minutos=I2C_eeprom_read_byte(8*k + 4);
            segundos=I2C_eeprom_read_byte(8*k + 5);
            ev = tblEventos[I2C_eeprom_read_byte(8*k + 6)];
            val=I2C_eeprom_read_byte(8*k + 7);
            
            registros += "<tr><td>" + String(k+1) + "</td><td>" + String(dia) +  "/" + String(mes) +  "/20" + String(anio) + "</td><td>" + String(horas) +  ":" + String(minutos) +  ":" + String(segundos) + "</td><td>" + ev + "</td><td>" + String(val) + "</td></tr>";

        } 
    }
}
