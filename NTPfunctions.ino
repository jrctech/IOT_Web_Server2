void sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  // send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t NTP_getTimeDate()
{
  Serial.println("Trying to get time and date from NTP server...");
  sendNTPpacket(timeServer);          // Enviar una peticion
  delay(1000);                        // Damos tiempo a la respuesta

  if ( Udp.parsePacket() )
  { 
    Udp.read(packetBuffer,NTP_PACKET_SIZE);           // Leer el paquete
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Convierte a tiempo Unix
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    if (!epoch)
    {
      setSyncInterval(10);  //Reintentar cada 10s hasta que logre sincronizar
      return 0;
    }
    epoch= epoch - 14400;    //Hora de Venezuela (GMT -4:00)
    Serial.print("Unix time: ");
    Serial.println(epoch);
    setSyncInterval(3600);  //Sincronizar cada hora
    Serial.print("RTC Sync... ");
    if(write_ds1307((uint8_t)day(epoch), (uint8_t)month(epoch), (uint8_t)(year(epoch)-2000), (uint8_t)weekday(epoch), (uint8_t)hour(epoch), (uint8_t)minute(epoch), (uint8_t)  second(epoch)))
      Serial.println("Success!");
    else
      Serial.println("Error updating RTC!");
    return epoch;
  }
  setSyncInterval(10);
  return 0;
}
