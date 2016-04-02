
/*************************************************
  This section is about the screen
 ************************************************/

void ScreenBegin() {

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  uint16_t time = millis();
  time = millis() - time;
  tft.setRotation(1); // horizontal display
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(15, 50, 133, 35, ST7735_WHITE);
  tft.setCursor(51, 60);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println("HELLO");

  STATE = 0;

}

/*************************************************
  DISPLAY HOME
 ************************************************/

void Display_Home() {

  uint32_t epoch = rtc.getEpoch() + GMT;
  struct tm * timeinfo;
  time_t epoch_time_as_time_t = epoch; /* Conversion to time_t as localtime() expects a time_t* */
  char buf[200];
  timeinfo = localtime(&epoch_time_as_time_t);   /* Call to localtime() now operates on time_t */
  strftime(buf, sizeof(buf), "%H:%M", timeinfo);

  HomeTimer = millis();
  DHTSensor.ReadSensors();

  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(90, 10);
  tft.print(buf);
  tft.setCursor(5, 40);
  strftime(buf, sizeof(buf), "%a %d/%m/%y", timeinfo);
  tft.print(buf);


  tft.setCursor(5, 75);
  tft.print("TEMP ");
  tft.print(DHTSensor.GetTemp());
  tft.print(" C");

  if (ManualMode == true) {
    tft.setTextSize(1);
    tft.setCursor(5, 110);
    tft.print("Manual Mode");
  }
  if (AutoMode == true) {
    tft.setTextSize(1);
    tft.setCursor(5, 110);
    tft.print("Auto Mode");
  }

}

/*************************************************
  DISPLAY NAV-MENU
 ************************************************/
void Display_NavMenu() {


  switch (counter) {
    case 0:
      tft.fillScreen(ST7735_BLACK);
      tft.fillRect(15, 10, 133, 35, ST7735_WHITE);
      tft.setCursor(60, 20);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_BLACK);
      tft.println("AUTO");

      tft.drawRect(15, 50, 133, 35, ST7735_WHITE);
      tft.setCursor(48, 60);
      tft.setTextColor(ST7735_WHITE);
      tft.println("MANUAL");

      tft.drawRect(15, 90, 133, 35, ST7735_WHITE);
      tft.setCursor(55, 100);
      tft.println("PROG");

      break;

    case 1:

      tft.fillScreen(ST7735_BLACK);
      tft.setTextSize(2);
      tft.drawRect(15, 10, 133, 35, ST7735_WHITE);
      tft.setCursor(60, 20);
      tft.setTextColor(ST7735_WHITE);
      tft.println("AUTO");

      tft.fillRect(15, 50, 133, 35, ST7735_WHITE);
      tft.setCursor(48, 60);
      tft.setTextColor(ST7735_BLACK);
      tft.println("MANUAL");

      tft.drawRect(15, 90, 133, 35, ST7735_WHITE);
      tft.setCursor(55, 100);
      tft.setTextColor(ST7735_WHITE);
      tft.println("PROG");
      flag = true;
      break;

    case 2:
      tft.fillScreen(ST7735_BLACK);
      tft.setTextSize(2);
      tft.drawRect(15, 10, 133, 35, ST7735_WHITE);
      tft.setCursor(60, 20);
      tft.setTextColor(ST7735_WHITE);
      tft.println("AUTO");

      tft.drawRect(15, 50, 133, 35, ST7735_WHITE);
      tft.setCursor(48, 60);
      tft.setTextColor(ST7735_WHITE);
      tft.println("MANUAL");

      tft.fillRect(15, 90, 133, 35, ST7735_WHITE);
      tft.setCursor(55, 100);
      tft.setTextColor(ST7735_BLACK);
      tft.println("PROG");
      flag = true;
      break;

  }
}

/*************************************************
  DISPLAY AUTO
 ************************************************/

void Display_Auto() {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  if (AutoMode == true) {
    tft.fillRect(15, 50, 133, 35, ST7735_WHITE);
    tft.setCursor(40, 60);
    tft.setTextColor(ST7735_BLACK);
    tft.println("AUTO ON");
  } else {
    tft.drawRect(15, 50, 133, 35, ST7735_WHITE);
    tft.setCursor(40, 60);
    tft.setTextColor(ST7735_WHITE);
    tft.println("AUTO OFF");
  }
  flag = true;

}

/*************************************************
  DISPLAY MANUAL
 ************************************************/

void Display_Manual() {

  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  if (ManualMode == true) {
    tft.fillRect(15, 50, 133, 35, ST7735_WHITE);
    tft.setCursor(30, 60);
    tft.setTextColor(ST7735_BLACK);
    tft.println("MANUAL ON");
  } else {
    tft.drawRect(15, 50, 133, 35, ST7735_WHITE);
    tft.setCursor(26, 60);
    tft.setTextColor(ST7735_WHITE);
    tft.println("MANUAL OFF");
  }
  flag = true;
}

/*************************************************
  DISPLAY PROG
 ************************************************/

void Display_Prog() {

  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 10);
  tft.setTextSize(2);
  tft.println("<-");
  tft.setCursor(40, 10);
  tft.println(GetWeekDay_Str(day_counter));
  Serial.println(GetWeekDay_Str(day_counter));

  int x = 3;
  int y = 50;
  int h = 0;

  for ( int a = 0; a < 3; a++) {
    for (int b = 0; b < 8; b++) {

      if (days.days[day_counter].hours[h] == true) {
        tft.fillRect(x, y, 15, 15, ST7735_WHITE);
        tft.setCursor(x + 2, y + 3);
        tft.setTextColor(ST7735_BLACK);
        tft.setTextSize(1);
        tft.println(h);
      }
      else {
        tft.drawRect(x, y, 15, 15, ST7735_WHITE);
        tft.setCursor(x + 2, y + 3);
        tft.setTextColor(ST7735_WHITE);
        tft.setTextSize(1);
        tft.println(h);
      }

      if (counter == h) tft.drawRect(x - 1, y - 1, 17, 17, ST7735_RED);
      else if (counter == -1) tft.drawRect(37, 6, 40, 20, ST7735_RED);
      else if (counter == -2) tft.drawRect(4, 9, 30, 20, ST7735_RED);

      x = x + 20;
      h++;
    }
    y = y + 25;
    x = 3;
  }

  flag = true;
}

/*************************************************
  DISPLAY TELEGRAM MODE
 ************************************************/
void Display_TelegramMode() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println("TELEGRAM MODE");

  tft.setTextSize(1);
  tft.setCursor(0, 110);
  tft.print("Send a Stop message to exit the Telegram Mode");
}


/*************************************************
  DISPLAY DEFAULT TEMP
 ************************************************/

void Display_Default_Temp() {

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println("DEFAULT TEMP");
  tft.println(default_temp);
  flag = true;
}






