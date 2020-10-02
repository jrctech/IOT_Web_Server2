//Send one byte to the PIC
bool I2C_PIC_send_byte(char c){
  Wire.beginTransmission(0x77); //Send address of 7 bits and the R/W bit clear
  Wire.write(c);
  byte r = Wire.endTransmission();
  if (r == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void I2C_PIC_receive_array(int n){ //Receive n bytes from the PIC and store them in values[] char array
  byte rdata=0x00;
  Wire.requestFrom(0x77,n);  //Send address of 7 bits and the R/W bit set
  
  for (char i=0; i<n; i++){  
    if (Wire.available()) rdata = Wire.read();  
    values[i] = rdata;   
  }
}
