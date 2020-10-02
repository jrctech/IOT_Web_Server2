/*----------------------------------------------------------------------------------------
                           FUNCTIONS TO MANAGE RTC ds1307
----------------------------------------------------------------------------------------*/

/**
   RTC ds1307 Initialization
*/

bool init_ds1307()
{
  Wire.beginTransmission(0x68); //RTC Address
  Wire.write(0x00);             //Seconds register address

  if (Wire.endTransmission() != 0)
    return false;

  Wire.requestFrom(0x68, 1);    

  uint8_t seconds =Wire.read();
  seconds &= 0x7F;              //Discard CH bit (MSB) to start up the oscilator
  delay(10);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x00);

  Wire.write(seconds);          //Write seconds register
  if (Wire.endTransmission() != 0)
    return false;

  Wire.beginTransmission(0x68);
  Wire.write(0x07);             //Control register address
  Wire.write(0x10);             //Square wave out 1Hz
  if (Wire.endTransmission() != 0)
    return false;

  return true;
}

/**
   Set time and date RTC ds1307
*/
bool write_ds1307(uint8_t  days, uint8_t  mth, uint8_t  years, uint8_t  dow, uint8_t  hr, uint8_t  minutes, uint8_t  sec)
{
  
  Wire.beginTransmission(0x68);
  Wire.write(0x00);

  Wire.write(bin2bcd(sec & 0x7F)); // <--- Esto hace que el reloj comience a trabajar
  Wire.write(bin2bcd(minutes));
  Wire.write(bin2bcd(hr));
  Wire.write(bin2bcd(dow));
  Wire.write(bin2bcd(days));
  Wire.write(bin2bcd(mth));
  Wire.write(bin2bcd(years));
  Wire.write(0x10);

  if (Wire.endTransmission() != 0)
    return false;

  return true;
}


/**
   Read time and date RTC ds1307
*/
bool read_ds1307(uint8_t  *sec, uint8_t  *minutes, uint8_t  *hr, uint8_t  *dow, uint8_t  *days, uint8_t  *mth, uint8_t  *years, uint8_t  *ctrl)
{
  Wire.beginTransmission(0x68);

  Wire.write(0x00);

  if (Wire.endTransmission() != 0)
    return false;

  Wire.requestFrom(0x68, 8);

  *sec = bcd2bin(Wire.read());
  *minutes = bcd2bin(Wire.read());
  *hr = bcd2bin(Wire.read());
  *dow = bcd2bin(Wire.read());
  *days = bcd2bin(Wire.read());
  *mth = bcd2bin(Wire.read());
  *years = bcd2bin(Wire.read());

  *ctrl = Wire.read();

  return true;
}


/**
  Converts a binary to BCD
*/
uint8_t bin2bcd(uint8_t bin)
{
  return (bin / 10 * 16) + (bin % 10);
}


/**
   Converts a BCD to binary
*/
uint8_t bcd2bin(uint8_t bcd)
{
  return (bcd / 16 * 10) + (bcd % 16);
}
