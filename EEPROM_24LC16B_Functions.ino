byte I2C_eeprom_read_byte(unsigned int address )
{
  byte rdata = 0xFF;
  int controlByte = 0x50 | (address >> 8);
  Wire.beginTransmission(controlByte);
  Wire.write(address);   // LSB
  Wire.endTransmission();
  Wire.requestFrom(0x50 | (address >> 8),1);
  if (Wire.available()) rdata = Wire.read();  
  return rdata;
}

void I2C_eeprom_write_byte(unsigned int address, byte data ) 
{
  int controlByte = 0x50 | (address >> 8);
  Wire.beginTransmission(controlByte);
  Wire.write(address);   // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}

void guardarEvento(char idEvento, char val){
    //byte total;
    unsigned int posicion;
    uint8_t dayM, monthM, yrM, hrsM, minM, secM, dowM, ctrlM;
    
    total = I2C_eeprom_read_byte(2047);
    if((total==0xFF) || (total==0xFE) ){ //0xFF: Memoria vacía, //0xFE: 254 registros --> memoria llena
        total=0;
        I2C_eeprom_write_byte(2047, total);
    }
    posicion=8*total; //Posición para guardar primer byte

    if(read_ds1307(&secM, &minM, &hrsM, &dowM, &dayM, &monthM, &yrM, &ctrlM))
    {
      I2C_eeprom_write_byte(posicion,   dayM);
      I2C_eeprom_write_byte(posicion+1, monthM);
      I2C_eeprom_write_byte(posicion+2, yrM);
      I2C_eeprom_write_byte(posicion+3, hrsM);
      I2C_eeprom_write_byte(posicion+4, minM);
      I2C_eeprom_write_byte(posicion+5, secM);
      I2C_eeprom_write_byte(posicion+6, idEvento);
      I2C_eeprom_write_byte(posicion+7, val);
      I2C_eeprom_write_byte(2047, total+1);
    }
    sendEventDB((int)idEvento, (int)val);
    
}

void guardarEventoApagado(String d_t){
    //byte total;
    unsigned int posicion;
    uint8_t dayM, monthM, yrM, hrsM, minM, secM, dowM, ctrlM;
    
    total = I2C_eeprom_read_byte(2047);
    if((total==0xFF) || (total==0xFE) ){ //0xFF: Memoria vacía, //0xFE: 254 registros --> memoria llena
        total=0;
        I2C_eeprom_write_byte(2047, total);
    }
    posicion=8*total; //Posición para guardar primer byte

    yrM = (uint8_t)(d_t.substring(2,4).toInt());
    monthM = (uint8_t)(d_t.substring(5,7).toInt()); 
    dayM = (uint8_t)(d_t.substring(8,10).toInt());
    hrsM = (uint8_t)(d_t.substring(11,13).toInt());
    minM = (uint8_t)(d_t.substring(14,16).toInt());
    secM = (uint8_t)(d_t.substring(17,19).toInt());

    I2C_eeprom_write_byte(posicion,   dayM);
    I2C_eeprom_write_byte(posicion+1, monthM);
    I2C_eeprom_write_byte(posicion+2, yrM);
    I2C_eeprom_write_byte(posicion+3, hrsM);
    I2C_eeprom_write_byte(posicion+4, minM);
    I2C_eeprom_write_byte(posicion+5, secM);
    I2C_eeprom_write_byte(posicion+6, 18);
    I2C_eeprom_write_byte(posicion+7, 0);
    I2C_eeprom_write_byte(2047, total+1);
    
    //sendEventDB((int)idEvento, (int)val);
    
}
