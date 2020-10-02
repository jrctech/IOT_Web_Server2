/*----------------------------------------------------------------------------------------
                           FUNCTIONS TO MANAGE RTC ds1307
----------------------------------------------------------------------------------------*/

/**
   Set time and date RTC ds1307
*/
bool write_ds1307(uint8_t  day, uint8_t  mth, uint8_t  year, uint8_t  dow, uint8_t  hr, uint8_t  minute, uint8_t  sec)
{
  
  Wire.beginTransmission(0x68);
  Wire.write(0x00);

  Wire.write(bin2bcd(sec & 0x7F)); // <--- Esto hace que el reloj comience a trabajar
  Wire.write(bin2bcd(minute));
  Wire.write(bin2bcd(hr));
  Wire.write(bin2bcd(dow));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(mth));
  Wire.write(bin2bcd(year));
  Wire.write(0x10);

  if (Wire.endTransmission() != 0)
    return false;

  return true;
}


/**
   Read time and date RTC ds1307
*/
bool read_ds1307(uint8_t  *sec, uint8_t  *minute, uint8_t  *hr, uint8_t  *dow, uint8_t  *day, uint8_t  *mth, uint8_t  *year, uint8_t  *ctrl)
{
  Wire.beginTransmission(0x68);

  Wire.write(0x00);

  if (Wire.endTransmission() != 0)
    return false;

  Wire.requestFrom(0x68, 8);

  *sec = bcd2bin(Wire.read());
  *minute = bcd2bin(Wire.read());
  *hr = bcd2bin(Wire.read());
  *dow = bcd2bin(Wire.read());
  *day = bcd2bin(Wire.read());
  *mth = bcd2bin(Wire.read());
  *year = bcd2bin(Wire.read());

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
