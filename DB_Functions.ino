//String remServerURL = "http://byethost16.com";

bool sendEventDB(int event, int value)
{
  if (WiFi.status() != WL_CONNECTED)    //Don't try if WiFi is not connected 
    return true;                        //Don't wait for WiFi connection
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/insert.php")){
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    char date_time[20];
    sprintf(date_time, "20%02i-%02i-%02i %02i:%02i:%02i", years, mth, days, hr, minutes, sec);
    String payload = "event=" + String(event) + "&value=" + String(value) + "&description=" + String(tblEventos[event] + "&date_time=" + String(date_time));
    int httpCode=-1;
    char retry=0;
    while((httpCode != 200) && (retry <3))
    {
      retry++;
      httpCode = http.POST(payload); 
    }
    Serial.println("Event registered in database. HTTP Code: " + String(httpCode) + " Server response: " + http.getString());
    http.end();
    return true;
  }
  return false;
}

bool getConfig_DB()
{
  if (WiFi.status() != WL_CONNECTED)    //Don't try if WiFi is not connected 
    return true;                        //Don't wait for WiFi connection
  int httpCode=-1;
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/get_config.php")){
    httpCode = http.GET(); 
    if (httpCode == 200)
    {
      String config_values = http.getString();  
      AC = config_values.charAt(0) - 48;
      A_AC = config_values.charAt(2) - 48;
      LA = config_values.charAt(4) - 48;
      A_LA = config_values.charAt(6) - 48;
      LB = config_values.charAt(8) - 48;
      A_LB = config_values.charAt(10) - 48;
      LC = config_values.charAt(12) - 48;
      A_LC = config_values.charAt(14) - 48;
      V = config_values.charAt(16) - 48;

      config_values = config_values.substring(18);

      int index = config_values.indexOf(',');
      TR = config_values.substring(0,index).toInt();
      config_values = config_values.substring(index+1);
      
      index = config_values.indexOf(',');
      AR = config_values.substring(0,index).toInt();
      config_values = config_values.substring(index+1);

      index = config_values.indexOf(',');
      BR = config_values.substring(0,index).toInt();
      config_values = config_values.substring(index+1);

      index = config_values.indexOf(',');
      CR = config_values.substring(0,index).toInt();
      config_values = config_values.substring(index+1);

      PW = config_values.toInt();
      Serial.println("Config Data: HTTP Code: " + String(httpCode));
      http.end();
      return true;
    }
    Serial.println("Config Data: HTTP Code: " + String(httpCode));
    http.end();
   }  
   return false;
}

bool sendReportDB()
{
  if (WiFi.status() != WL_CONNECTED)    //Don't try if WiFi is not connected 
    return true;                        //Don't wait for WiFi connection
  Serial.println("Send report to DB...");
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/report.php")){
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    char date_time[20];
    sprintf(date_time, "20%02i-%02i-%02i %02i:%02i:%02i", years, mth, days, hr, minutes, sec);
    String payload = "date_time=" + String(date_time);
    int httpCode=-1;
    char retry=0;
    while((httpCode != 200) && (retry <3))
    {
      retry++;
      httpCode = http.POST(payload); 
    }
    Serial.println("HTTP Code: " + String(httpCode) + " " + http.getString());
    http.end();
    return true;
  }
  return false;
}

String getLastReportDB()
{
  if (WiFi.status() != WL_CONNECTED)    //Don't try if WiFi is not connected 
    return "WiFi not connected";                        //Don't wait for WiFi connection
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/get_last_report.php")){
    int httpCode=-1; 
    char retry=0;
    while((httpCode != 200) && (retry <3))
    {
      retry++;
      httpCode = http.GET();  
    }
    String r = http.getString();
    Serial.println("HTTP Code: " + String(httpCode) + " " + r);
    http.end();
    return r;
  }
  return "Failed";
}

bool sendPWOffEventDB(String d_t)
{
  if (WiFi.status() != WL_CONNECTED)    //Don't try if WiFi is not connected 
    return true;                        //Don't wait for WiFi connection
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/insert.php")){
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    String payload = "event=18&value=0&description=Apagado&date_time=" + d_t;
    int httpCode=-1;
    char retry=0;
    while((httpCode != 200) && (retry <3))
    {
      retry++;
      httpCode = http.POST(payload); 
    }
    Serial.println("Event registered in database. HTTP Code: " + String(httpCode) + " Server response: " + http.getString());
    http.end();
    return true;
  }
  return false;
}
