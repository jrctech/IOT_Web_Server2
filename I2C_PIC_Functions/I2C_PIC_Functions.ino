
void send_char(char c){
  Serial.print("Sending char ");
  Serial.println(c);
  Wire.beginTransmission(0x77);
  Wire.write(c);
  byte r = Wire.endTransmission();
  if (r == 0)
  {
    Serial.println("Char sent!");
  }
  else
  {
    Serial.println("... " + String(r));
  }
}

String receive_string(int n){
  String buff="";
  byte rdata=0x00;
  Wire.requestFrom(0x77,n);
  
  for (char i=0; i<n; i++){  
    if (Wire.available()) rdata = Wire.read();  
    if (rdata != 0x00)
    {
      char c[]="";
      sprintf(c, "%c", rdata);
      buff += c;   
    }
  }
  return buff;
}
