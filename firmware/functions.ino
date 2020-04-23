void checkBrightness() {
  if (analogRead(PHOTO) < BRIGHT_THRESHOLD) {   // light place
    analogWrite(BACKLIGHT, LCD_BRIGHT_MIN);
    LED_ON = LED_MODE == 0 ? LED_BRIGHT_MIN : 255 - LED_BRIGHT_MIN;
  } else {                                      // dark place
    analogWrite(BACKLIGHT, LCD_BRIGHT_MAX);
    LED_ON = LED_MODE == 0 ? LED_BRIGHT_MAX : 255 - LED_BRIGHT_MAX;
  }

  if (dispCO2 < 800) setLED(2);
  else if (dispCO2 < 1200) setLED(3);
  else if (dispCO2 >= 1200) setLED(1);
}

void modesTick() {
  button.tick();
  boolean changeFlag = false;
  if (button.isClick()) {
    mode++;

#if (CO2_SENSOR == 1)
    if (mode > 8) mode = 0;
#else
    if (mode > 6) mode = 0;
#endif
    changeFlag = true;
  }

  if (button.isHolded()) {
    mode = 0;
    changeFlag = true;
  }

  if (changeFlag) {
    if (mode == 0) {
      lcd.clear();
      loadClock();
      drawClock(hrs, mins, 0, 0, 1);
      drawData();
      drawSensors();
    } else {
      lcd.clear();
      loadPlot();
      redrawPlot();
    }
  }
}

void redrawPlot() {
  lcd.clear();
  switch (mode) {
    case 1: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tmpHour, "t hr"); break;
    case 2: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tmpDay, "t day"); break;
    case 3: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humHour, "h hr"); break;
    case 4: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humDay, "h day"); break;
    case 5: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)prsHour, "p hr"); break;
    case 6: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)prsDay, "p day"); break;
    case 7: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Hour, "c hr"); break;
    case 8: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Day, "c day"); break;
  }
}

void readSensors() {
  bme.takeForcedMeasurement();
  dispTmp = bme.readTemperature();
  dispHum = bme.readHumidity();
  dispPrs = (float)bme.readPressure() * 0.00750062;
#if (CO2_SENSOR == 1)
  dispCO2 = mhz19.getPPM();

  if (dispCO2 < 800) setLED(2);
  else if (dispCO2 < 1200) setLED(3);
  else if (dispCO2 >= 1200) setLED(1);
#endif
}

void drawSensors() {
  lcd.setCursor(0, 2);
  lcd.print(String(dispTmp, 1));
  lcd.write(223);

  lcd.setCursor(9, 2);
  lcd.print(String(dispHum) + " %");

  lcd.setCursor(0, 3);
  lcd.print(String(dispPrs) + " mm");

  #if (CO2_SENSOR == 1)
  lcd.setCursor(8, 3);
  lcd.print(String(dispCO2) + " ppm");
  #endif
}

void plotSensorsTick() {
  // hourly
  if (hourPlotTimer.isReady()) {
    for (byte i = 0; i < 14; i++) {
      tmpHour[i] = tmpHour[i + 1];
      humHour[i] = humHour[i + 1];
      prsHour[i] = prsHour[i + 1];
      co2Hour[i] = co2Hour[i + 1];
    }

    tmpHour[14] = dispTmp;
    humHour[14] = dispHum;
    co2Hour[14] = dispCO2;
    prsHour[14] = dispPrs;
  }

  // daily
  if (dayPlotTimer.isReady()) {
    long avgTmp = 0, avgHum = 0, avgPrs = 0, avgCO2 = 0;

    for (byte i = 0; i < 15; i++) {
      avgTmp += tmpHour[i];
      avgHum += humHour[i];
      avgPrs += prsHour[i];
      avgCO2 += co2Hour[i];
    }

    avgTmp /= 15;
    avgHum /= 15;
    avgPrs /= 15;
    avgCO2 /= 15;

    for (byte i = 0; i < 14; i++) {
      tmpDay[i] = tmpDay[i + 1];
      humDay[i] = humDay[i + 1];
      prsDay[i] = prsDay[i + 1];
      co2Day[i] = co2Day[i + 1];
    }

    tmpDay[14] = avgTmp;
    humDay[14] = avgHum;
    prsDay[14] = avgPrs;
    co2Day[14] = avgCO2;
  }
}

boolean dotFlag;
void clockTick() {
  dotFlag = !dotFlag;
  if (dotFlag) {          // каждую секунду пересчёт времени
    secs++;
    if (secs > 59) {      // каждую минуту
      secs = 0;
      mins++;
      if (mins <= 59 && mode == 0) drawClock(hrs, mins, 0, 0, 1);
    }
    if (mins > 59) {      // каждый час
      now = rtc.now();
      secs = now.second();
      mins = now.minute();
      hrs = now.hour();
      if (mode == 0) drawClock(hrs, mins, 0, 0, 1);
      if (hrs > 23) {
        hrs = 0;
      }
      if (mode == 0) drawData();
    }
  }
  if (mode == 0) drawDots(7, 0, dotFlag);
  if (dispCO2 >= 1200) {
    if (dotFlag) setLED(1);
    else setLED(0);
  }
}