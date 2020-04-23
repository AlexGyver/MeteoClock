
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

void drawDots(byte x, byte y, boolean state) {
  byte code = state ? 165 : 32;
  
  lcd.setCursor(x, y);
  lcd.write(code);
  
  lcd.setCursor(x, y + 1);
  lcd.write(code);
}

void drawClock(byte hours, byte minutes, byte x, byte y, boolean dotState) {
  lcd.setCursor(x, y);
  lcd.print("               ");
  lcd.setCursor(x, y + 1);
  lcd.print("               ");

  if (hours / 10 == 0) drawDig(10, x, y);
  else drawDig(hours / 10, x, y);
  drawDig(hours % 10, x + 4, y);
  
  drawDig(minutes / 10, x + 8, y);
  drawDig(minutes % 10, x + 12, y);
}

static const char *dayNames[] = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};

void drawData() {
  lcd.setCursor(15, 0);
  if (now.day() < 10) lcd.print(0);
  lcd.print(now.day());
  
  lcd.print(".");
  if (now.month() < 10) lcd.print(0);
  lcd.print(now.month());

  lcd.setCursor(18, 1);
  int dayofweek = now.dayOfTheWeek();
  lcd.print(dayNames[dayofweek]);
}

void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int *plot_array, String label) {
  int max_value = -32000;
  int min_value = 32000;

  for (byte i = 0; i < 15; i++) {
    if (plot_array[i] > max_value) max_value = plot_array[i];
    if (plot_array[i] < min_value) min_value = plot_array[i];
  }

  lcd.setCursor(16, 0); lcd.print(max_value);
  lcd.setCursor(16, 1); lcd.print(label);
  lcd.setCursor(16, 2); lcd.print(plot_array[14]);
  lcd.setCursor(16, 3); lcd.print(min_value);

  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    int fill_val = plot_array[i];
    fill_val = constrain(fill_val, min_val, max_val);
    byte infill, fract;
    
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    infill = plot_array[i] > min_val ? floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10) : 0
    fract = (float)(infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;

    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor(i, (row - n));        // заполняем полными ячейками
        lcd.write(0);
      }

      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor(i, (row - n));
        lcd.write(fract > 0 ? fract : 16); 

        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor(i, (row - k));
          lcd.write(16);
        }

        break;
      }
    }
  }
}