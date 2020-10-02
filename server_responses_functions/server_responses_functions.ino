/*----------------------------------------------------------------------------------------
                           FUNCTIONS TO MANAGE WEB SERVER RESPONSES
----------------------------------------------------------------------------------------*/

void led_on()
{
  digitalWrite(LED,LOW);
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200, "text/plain", "LED encendido");
}

void led_off()
{
  digitalWrite(LED,HIGH);
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200, "text/plain", "LED apagado");
}

void lamp_control() 
{
  String s = page_Head;
  s += "<div class=\"container\">";
  s += "<h1>Lamp Control</h1>";
  s += "<div class=\"row\">";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on()\"></div>";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off()\"></div>";
  s += "</div></div>";
  s += "<div class=\"jumbotron\"><h3>Temperature Read: ";
  float temp;
  temp = analogRead(A0)/10.23;
  s += String(temp);
  s += "</h3></div>";
  s += "<script>function on() {$.get(\"/on\");}</script>";
  s += "<script>function off() {$.get(\"/off\");}</script>";
  s += page_footer;
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200, "text/html", s);
}

void no_encontrado()
{
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(404,"text/plain","Error en la petición");
}

void inicio()
{
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200, "text/html", page_Head + auth_Body1 + auth_Body2 + page_footer);
}

void principal()
{
  Serial.println("Número de argumentos: " + String(server.args()));
  String user = server.arg("txtUser");
  String pass = server.arg("txtPass");
  Serial.println("Usuario: " + user);
  Serial.println("Pass: " + pass);
  if((user=="Javier") && (pass=="1234"))
  {
    //Generar token:
    randomSeed(millis());
    token=random(1,65535);
    server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + body1 + panel_Rem + body2 + body3 + body4 + page_footer);
  }
  else
  {
    server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
    server.send(200, "text/html", page_Head + auth_Body1 + auth_Page_Denied + auth_Body2 + page_footer);
  }
}

void ping()
{
  String envio = String(AC) + "," + String(A_AC) + "," +  String(LA) + "," + String(A_LA) + "," + String(LB) + "," + String(A_LB) + "," + String(LC) + "," + String(A_LC) + "," + String(V) + "," + String(TR) + "," + String(AR) + "," + String(BR) + "," + String(CR) + "," + String(PW);
  server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
  server.send(200,"text/html", envio);
}

void actRTC(){
  if(String(token) == server.arg("token"))
    {  
      uint8_t d = (uint8_t)server.arg("A").toInt();
      uint8_t m = (uint8_t)server.arg("B").toInt();
      uint8_t y = (uint8_t)server.arg("C").toInt();
      uint8_t w = (uint8_t)server.arg("D").toInt();
      uint8_t h = (uint8_t)server.arg("E").toInt();
      uint8_t mn = (uint8_t)server.arg("F").toInt();
      uint8_t s = (uint8_t)server.arg("G").toInt();
      Serial.println(server.arg("C"));
      if (write_ds1307(d, m, y, w, h, mn, s)){
        Serial.println("Reloj Actualizado");
        server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
        server.send(200, "text/html", "Hora y fecha actualizadas");
      }
      else
      {
        server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
        server.send(200, "text/html", "Error en la actualización del RTC del sistema");
      }
    }
    else
    {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
    }
}

void actBits()
{
    if(String(token) == server.arg("token"))
    {
      //Serial.println("Número de argumentos: " + String(server.args()));
      if(server.arg("AC")=="true")
      {AC=1;}
      else
      {AC=0;}
      //Serial.println("AC: " + String(AC));
      
      if(server.arg("A_AC")=="true")
      {A_AC=1;}
      else
      {A_AC=0;}
      //Serial.println("A_AC: " + String(A_AC));
      
      if(server.arg("LA")=="true")
      {
        LA=1;
        digitalWrite(LED, LOW);  
      }
      else
      {
        LA=0;
        digitalWrite(LED, HIGH);  
      }
      //Serial.println("LA: " + String(LA));
      
     if(server.arg("A_LA")=="true")
      {A_LA=1;}
      else
      {A_LA=0;}
      //Serial.println("A_LA: " + String(A_LA));
      
      if(server.arg("LB")=="true")
      {LB=1;}
      else
      {LB=0;}
      //Serial.println("LB: " + String(LB));
      
      if(server.arg("A_LB")=="true")
      {A_LB=1;}
      else
      {A_LB=0;}
      //Serial.println("A_LB: " + String(A_LB));
      
      if(server.arg("LC")=="true")
      {LC=1;}
      else
      {LC=0;}
      //Serial.println("LC: " + String(LC));
      
      if(server.arg("A_LC")=="true")
      {A_LC=1;}
      else
      {A_LC=0;}
      //Serial.println("A_LC: " + String(A_LC));
      
      if(server.arg("V")=="true")
      {V=1;}
      else
      {V=0;}
      //Serial.println("V: " + String(V));
      
      TR=server.arg("TR").toInt();
      //Serial.println("TR: " + String(TR));
      AR=server.arg("AR").toInt();
      //Serial.println("AR: " + String(AR));
      BR=server.arg("BR").toInt();
      //Serial.println("BR: " + String(BR));
      CR=server.arg("CR").toInt();
      //Serial.println("CR: " + String(CR));
      PW=server.arg("PW").toInt();
      //Serial.println("PW: " + String(PW));
  
      float Temp = analogRead(A0)/10.23;
      String envio = String(Temp) + "," + String(LDR_A) + "," +  String(LDR_B) + "," + String(LDR_C) + "," + String(V9V/10) + "," + String(V9V % 10) + "," + String(V5V/10) + "," + String(V5V % 10) + "," + String(V3V3/10) + "," + String(V3V3 % 10);
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200,"text/html", envio);

    }
    else
    {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
    }
}

void cerrarSesion()
{
  if(String(token) == server.arg("token"))
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", page_Head + auth_Body1 + auth_Page_LoggedOut + auth_Body2 + page_footer );
      token=0;
  }
  else
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", page_Head + auth_Body1 + auth_Body2 + page_footer );
  }
}

void homepage()
{
  if(String(token) == server.arg("token"))
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", page_Head_Rem + nav1 + String(token) + nav2 + body1 + panel_Rem + body2 + body3 + body4 + page_footer);
  }
  else
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }
}

void viewlog()
{
  if(String(token) == server.arg("token"))
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Under Construction...");
  }
  else
  {
      server.sendHeader("Set-Cookie", "cross-site-cookie=name; SameSite=None; Secure");
      server.send(200, "text/html", "Token no valido");
  }
}
