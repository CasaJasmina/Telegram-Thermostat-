/*************************************************
  HOME
 ************************************************/
void Home() {

  if (buttonUpPressed) {
    timer = millis();
    default_temp++;
    my_flash_store.write(default_temp);
    Display_Default_Temp();
    buttonUpPressed = false;
  }
  else if (buttonEnterPressed) {
    STATE = 1;
    Display_NavMenu();
    buttonEnterPressed = false;
  }
  else if (buttonDownPressed) {
    timer = millis();
    default_temp--;
    my_flash_store.write(default_temp);
    Display_Default_Temp();
    buttonDownPressed = false;
  }

  if (flag == true && millis() - timer > 4 * 1000) {
    Display_Home();
    flag = false;
  }

  if (millis() - HomeTimer > 30 * 1000) { // Refresh the home page every 30 sec
    Display_Home();
  }

  if (ManualMode == true) {
    Keep_Temp();
  }
  if (AutoMode == true) {
    if (days.days[GetWeekDay_int()].hours[rtc.getHours() + 1] == true) {
      Keep_Temp();
    }
  }


}

/*************************************************
  NAVMENU
 ************************************************/

void NavMenu() {


  if (buttonUpPressed) {
    timer = millis();
    if (counter < 2) {
      counter++;
      Display_NavMenu();
    }
    buttonUpPressed = false;
  }

  else if (buttonEnterPressed) {
    timer = millis();
    switch (counter) {
      case 0:
        Display_Auto();
        STATE = 2; // AutoMode menu
        break;
      case 1:
        Display_Manual();
        STATE = 3; // ManualMode menu
        break;
      case 2:
        counter = -1;
        Display_Prog();  
        STATE = 4; // Setting menu
        break;
      }
      buttonEnterPressed = false;
    }

  else if (buttonDownPressed) {
    timer = millis();
    if (counter > 0) {
      counter--;
      Display_NavMenu();
    }
    buttonDownPressed = false;
  }

  if (flag == true && millis() - timer > 4 * 1000) {
    flag = false;
    counter = 0;
    STATE = 0;
    Display_Home();
  }
}



/*************************************************
  MANUAL
 ************************************************/

void Manual() {

  AutoMode = false;

  if (buttonEnterPressed) {
    timer = millis();
    if (ManualMode == true) {
      ManualMode = false;
      digitalWrite(RELAY_PIN, LOW);
      RelayStatus = false;
    }
    else {
      ManualMode = true;
      RelayStatus = true;
    }

    Display_Manual();
    buttonEnterPressed = false;
  }

  if (flag == true && millis() - timer > 4 * 1000) {
    flag = false;
    counter = 0;
    STATE = 0;
    Display_Home();
  }
}

/*************************************************
  PROG
 ************************************************/

void Prog() {

  // use counter to navigate in to hours, week days, and the exit button

  if (buttonUpPressed) {
    counter++;
    if (counter > 23) {
      counter = -2;
    }
    Display_Prog();
    buttonUpPressed = false;
  }

  else if (buttonEnterPressed) {

    if (counter >= 0 && counter <= 23) {
      set_prog( day_counter , counter);
      Display_Prog();
    }
    else if (counter == -1) {
      day_counter++;
      if (day_counter > 6) day_counter = 0;
      Display_Prog();
      Serial.println(day_counter);
    }
    else if (counter == -2) {
      my_flash_store_two.write(days);
      STATE = 0;
      Display_Home();
      counter = 0;
      flag = false;
    }


    buttonEnterPressed = false;
  }

  else if (buttonDownPressed) {
    counter--;
    if (counter < -2) counter = 23;
    Display_Prog();
    buttonDownPressed = false;
  }

}

/*************************************************
  AUTO
 ************************************************/

void Auto() {

  ManualMode = false;

  if (buttonEnterPressed) {
    timer = millis();
    AutoMode = ! (AutoMode);
    Display_Auto();
    buttonEnterPressed = false;
  }

  if (flag == true && millis() - timer > 4 * 1000) {
    flag = false;
    counter = 0;
    STATE = 0;
    Display_Home();
  }

}

/*************************************************
  TELEGRAM FUNCTIONS
 ************************************************/

void TelegramMode() {

  String temp = String(DHTSensor.GetTemp(), 2);
  String Therm_Status = "";
  String d_temp = String(default_temp);


  if (TeleFlag == true) {
    Display_TelegramMode();
    String msg = String("Hi there, it is " + temp + " degree inside. The thermostat is " + Get_Therm_Status() + "%0AType /list for a list of commands");
    bot.sendMessage(m.chat_id, msg);
    TeleFlag = false;
  }
  else if ( m.text.equals("Switch to manual")) {
    ManualMode = true;
    AutoMode = false;
    bot.sendMessage(m.chat_id, "The thermostat is now in Manual Mode ON");
  }
  else if ( m.text.equals("Turn it off")) {
    ManualMode = false;
    AutoMode = false;
    bot.sendMessage(m.chat_id, "The thermostat is now OFF");
  }

  else if ( m.text.equals("Switch to auto")) {
    ManualMode = false;
    AutoMode = true;
    bot.sendMessage(m.chat_id, "The thermostat is now in Auto Mode");
  }

  else if ( m.text.substring(0, 15).equals("Set temperature")) {
    int a = m.text.substring(m.text.length() - 2).toInt();
    default_temp = a;
    my_flash_store.write(default_temp);
    String b = "Done, now your default temperature is " + m.text.substring(m.text.length() - 2 ) + " degree";
    bot.sendMessage(m.chat_id, b);
  }

  else if ( m.text.equals("/list")) {
    bot.sendMessage(m.chat_id, "Start : starts the online mode. %0ASwitch to manual : enter the manual mode. %0ATurn it off. %0ASwitch to auto : enter the auto mode. %0ASet temperature to *your value* : change the default temperature. %0AStop : stops the online mode");
  }

  else if ( m.text.equals("Stop")) {
    online = false;
    STATE = 0;
    Display_Home();
    bot.sendMessage(m.chat_id, "Closing connection");

  }

}

/*************************************************
  OTHER FUNCTIONS
 ************************************************/

// toggle the thermostat in a certain day and hour
void set_prog(int d , int h) {
  days.days[d].hours[h] = !(days.days[d].hours[h]);
}
// get the status of the thermostat in a certain day and hour
bool get_prog(int d, int h) {
  return days.days[d].hours[h];
}


// keep the temperature at the default value
void Keep_Temp() {

  if (DHTSensor.GetTemp() > default_temp) {
    digitalWrite(RELAY_PIN, LOW);
    RelayStatus = false;
  }
  else if (DHTSensor.GetTemp() < default_temp) {
    digitalWrite(RELAY_PIN, HIGH);
    RelayStatus = true;
  }
}


// return the mode of the thermostat as a string, usefull for telegram interactions
String Get_Therm_Status() {
  String therm_status = "";

  if (ManualMode == false && AutoMode == false) {
    therm_status = "OFF";
  }
  else if (ManualMode == true) {
    therm_status = "in Manual Mode";
  }
  else if (AutoMode == true) {
    therm_status = "in Auto Mode";
  }
  return therm_status;
}




