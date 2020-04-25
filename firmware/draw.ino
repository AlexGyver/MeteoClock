
void drawDig(byte dig, byte x, byte y) {
  switch (dig) {
    case 0:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(1);
      lcd.write(2);

      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(x + 1, y);
      lcd.write(1);
      lcd.write(2);
      
      lcd.setCursor(x + 2, y + 1);
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(7);
      break;
    case 3:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 4:
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(2);
      
      lcd.setCursor(x + 2, y + 1);
      lcd.write(5);
      break;
    case 5:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(6);
      
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 6:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(6);
      
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 7:
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(1);
      lcd.write(2);
      
      lcd.setCursor(x + 1, y + 1);
      lcd.write(0);
      break;
    case 8:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 9:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      
      lcd.setCursor(x + 1, y + 1);
      lcd.write(4);
      lcd.write(5);
      break;
    case 10:
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      break;
  }
}

void drawClock() {
  int x = 0;
  int y = 0;
  
  lcd.setCursor(x, y);
  lcd.print("               ");
  lcd.setCursor(x, y + 1);
  lcd.print("               ");

  if (hrs / 10 == 0)
    drawDig(10, x, y);
  else
    drawDig(hrs / 10, x, y);
  drawDig(hrs % 10, x + 4, y);
  
  drawDig(mins / 10, x + 8, y);
  drawDig(mins % 10, x + 12, y);

  // Dots
  lcd.setCursor(7, 0);
  lcd.write(165);
  
  lcd.setCursor(7, 1);
  lcd.write(165);
}

void drawData() {
  lcd.setCursor(15, 0);
  if (now.day() < 10)
    lcd.print(0);
  lcd.print(now.day());
  
  lcd.print(".");
  if (now.month() < 10)
    lcd.print(0);
  lcd.print(now.month());

  lcd.setCursor(18, 1);
  int dayofweek = now.dayOfTheWeek();
  lcd.print(dayNames[dayofweek]);
}

void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int stp_val, int *plot_array, String label) {
  int max_value = -32000;
  int min_value = 32000;

  for (byte i = 0; i < 15; i++) {
    if (plot_array[i] > max_value) max_value = plot_array[i];
    if (plot_array[i] < min_value) min_value = plot_array[i];
  }

  lcd.setCursor(16, 0);
  lcd.print(max_value);

  lcd.setCursor(16, 1);
  lcd.print(label);

  lcd.setCursor(16, 2);
  lcd.print(plot_array[14]);

  lcd.setCursor(16, 3);
  lcd.print(min_value);

  for (byte i = 0; i < width; i++) {
    int fill_val = plot_array[i];
    fill_val = constrain(fill_val, min_val, max_val);
    byte infill, fract;
    
    infill = plot_array[i] > min_val ? floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10) : 0;
    fract = (float)(infill % 10) * 8 / 10;
    infill = infill / 10;

    for (byte n = 0; n < height; n++) {
      if (n < infill && infill > 0) {
        lcd.setCursor(i, (row - n));
        lcd.write(0);
      }

      if (n >= infill) {
        lcd.setCursor(i, (row - n));
        lcd.write(fract > 0 ? fract : 16); 

        for (byte k = n + 1; k < height; k++) {
          lcd.setCursor(i, (row - k));
          lcd.write(16);
        }

        break;
      }
    }
  }
}
