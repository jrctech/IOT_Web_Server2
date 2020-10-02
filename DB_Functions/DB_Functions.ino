bool sendEventDB(int event, int value)
{
  if (http.begin(client, "http://jrctech.000webhostapp.com/iot/php/insert.php")){
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    String payload = "event=" + String(event) + "&value=" + String(value) + "&description=" + String(tblEventos[event]);
    int httpCode = http.POST(payload); 
    Serial.println("Event registered in database. HTTP Code: " + String(httpCode) + " Server response: " + http.getString());
    http.end();
    return true;
  }
  return false;
}

bool getConfig_DB()
{
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
