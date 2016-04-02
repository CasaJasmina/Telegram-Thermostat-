/*************************************************
  Start an UDP connection to get the time in unix,
  then set the real time clock (rtc)
 ************************************************/

unsigned long epoch;

void GetCurrentTime() {

  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = readLinuxEpochUsingNTP();
    numberOfTries++;
  }
  while ((epoch == 0) || (numberOfTries > maxTries));

  if (numberOfTries > maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
  }
  else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);

    Serial.println();
  }

}

unsigned long readLinuxEpochUsingNTP()
{
  Udp.begin(localPort);
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  if ( Udp.parsePacket() ) {
    Serial.println("NTP time received");
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:

    Udp.stop();
    return (secsSince1900 - seventyYears);
  }

  else {
    Udp.stop();
    return 0;
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress & address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

/*************************************************
  Convert unix time into Week Days
 ************************************************/

int GetWeekDay_int() {
  uint32_t epoch = rtc.getEpoch() + GMT;
  struct tm * timeinfo;
  /* Conversion to time_t as localtime() expects a time_t* */
  time_t epoch_time_as_time_t = epoch;
  char buf[100];
  /* Call to localtime() now operates on time_t */
  timeinfo = localtime(&epoch_time_as_time_t);
  strftime(buf, sizeof(buf), "%w", timeinfo);
  int WeekDay= atoi(buf);
  
  return WeekDay;
}

String GetWeekDay_Str(int a ) {
  String day_of_the_week_Str = "";
  
  switch (a) {

    case 1:
      day_of_the_week_Str = "Mo";
      break;
    case 2:
      day_of_the_week_Str = "Tue";
      break;
    case 3:
      day_of_the_week_Str = "Wed";
      break;
    case 4:
      day_of_the_week_Str = "Thu";
      break;
    case 5:
      day_of_the_week_Str = "Fri";
      break;
    case 6:
      day_of_the_week_Str = "Sat";
      break;
    case 0:
      day_of_the_week_Str = "Sun";
      break;
  }
  return day_of_the_week_Str;
}



