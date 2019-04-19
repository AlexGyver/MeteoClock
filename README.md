![PROJECT_PHOTO](https://github.com/AlexGyver/MeteoClock/blob/master/proj_img.jpg)
# Метеостанция на Arduino
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Схемы подключения](#chapter-2)
* [Материалы и компоненты](#chapter-3)
* [Как скачать и прошить](#chapter-4)
* [FAQ](#chapter-5)
* [Полезная информация](#chapter-6)
[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)

<a id="chapter-0"></a>
## Описание проекта
Крутая домашняя метеостанция на Arduino с кучей датчиков, часами и большим дисплеем
Страница проекта на сайте: https://alexgyver.ru/MeteoClock/  
   
#### Особенности:
- Большой дешёвый LCD дисплей
- Вывод на дисплей:
	- Большие часы
	- Дата
	- Температура воздуха
	- Влажность воздуха
	- Атмосферное давление (в мм.рт.ст.)
	- Углекислый газ (в ppm)
	- Прогноз осадков на основе изменения давления
- Построение графиков показаний с датчиков за час и сутки
- Индикация уровня CO2 трёхцветным светодиодом (общий анод/общий катод, настраивается в прошивке)
- Переключение режимов сенсорной кнопкой
- В версиях выше 1.5 сделано автоматическое регулирование яркости дисплея и светодиода. Смотрите новую схему

<a id="chapter-1"></a>
## Папки
**ВНИМАНИЕ! Если это твой первый опыт работы с Arduino, читай [инструкцию](#chapter-4)**
- **libraries** - библиотеки проекта. Заменить имеющиеся версии
- **firmware** - прошивки для Arduino
- **schemes** - схемы подключения компонентов

<a id="chapter-2"></a>
## Схемы
![SCHEME](https://github.com/AlexGyver/MeteoClock/blob/master/schemes/scheme1.jpg)
![SCHEME](https://github.com/AlexGyver/MeteoClock/blob/master/schemes/scheme2.jpg)

<a id="chapter-3"></a>
## Материалы и компоненты
### Ссылки оставлены на магазины, с которых я закупаюсь уже не один год
Первые ссылки - в основном магазин Great Wall, вторые - WAVGAT. Покупая в одном магазине, вы экономите на доставке!
- Arduino NANO http://ali.ski/t_9Ij  http://ali.ski/cB3eu
- Сенсорный модуль http://ali.ski/IFKkYZ  http://ali.ski/aCDUb
- BME280 (5V) http://ali.ski/Ajl2y  http://ali.ski/bLXot
- MH-Z19 http://ali.ski/sBt2yZ  http://ali.ski/H88vfk
- Дисплей 2004 i2c http://ali.ski/po8xV  http://ali.ski/gldFua
- DS3231 http://ali.ski/6d7v7  http://ali.ski/cIpCv
- Микро DS3231 http://ali.ski/HmiPE  (http://ali.ski/O_RIyj БЕЗ ИОНИСТОРА)

Дополнительно:
- Светодиод RGB (пачка) http://ali.ski/Jw5dW
- RGB модуль (ему резистор НЕ НУЖЕН) http://ali.ski/MHuR9  http://ali.ski/VCKQy
- Ещё модуль 1 светодиод http://ali.ski/cNjGt
- Резисторы (220 R) http://ali.ski/c2_Ju
- Проводочки http://ali.ski/aQpyr  http://ali.ski/NcIJqb
- Корпус http://ali.ski/XxmknP
- Корпус (Чип и Дип) https://www.chipdip.ru/product/g909g
- Микро USB модуль http://ali.ski/LEOka

## Вам скорее всего пригодится
* [Всё для пайки (паяльники и примочки)](http://alexgyver.ru/all-for-soldering/)
* [Недорогие инструменты](http://alexgyver.ru/my_instruments/)
* [Все существующие модули и сенсоры Arduino](http://alexgyver.ru/arduino_shop/)
* [Электронные компоненты](http://alexgyver.ru/electronics/)
* [Аккумуляторы и зарядные модули](http://alexgyver.ru/18650/)

<a id="chapter-4"></a>
## Как скачать и прошить
* [Первые шаги с Arduino](http://alexgyver.ru/arduino-first/) - ультра подробная статья по началу работы с Ардуино, ознакомиться первым делом!
* Скачать архив с проектом
> На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**
* Установить библиотеки в  
`C:\Program Files (x86)\Arduino\libraries\` (Windows x64)  
`C:\Program Files\Arduino\libraries\` (Windows x86)
* **Подключить внешнее питание 5 Вольт**
* Подключить Ардуино к компьютеру
* Запустить файл прошивки (который имеет расширение .ino)
* Настроить IDE (COM порт, модель Arduino, как в статье выше)
* Настроить что нужно по проекту
* Нажать загрузить
* Пользоваться  

## Настройки в коде
    RESET_CLOCK 0       // сброс часов на время загрузки прошивки (для модуля с несъёмной батарейкой). Не забудь поставить 0 и прошить ещё раз!
    SENS_TIME 30000     // время обновления показаний сенсоров на экране, миллисекунд
    LED_MODE 0          // тип RGB светодиода: 0 - главный катод, 1 - главный анод
    
    // управление яркостью
    BRIGHT_CONTROL 1      // 0/1 - запретить/разрешить управление яркостью (при отключении яркость всегда будет макс.)
    BRIGHT_THRESHOLD 350  // величина сигнала, ниже которой яркость переключится на минимум (0-1023)
    LED_BRIGHT_MAX 255    // макс яркость светодиода СО2 (0 - 255)
    LED_BRIGHT_MIN 10     // мин яркость светодиода СО2 (0 - 255)
    LCD_BRIGHT_MAX 255    // макс яркость подсветки дисплея (0 - 255)
    LCD_BRIGHT_MIN 10     // мин яркость подсветки дисплея (0 - 255)
    
    BLUE_YELLOW 1       // жёлтый цвет вместо синего (1 да, 0 нет) но из за особенностей подключения жёлтый не такой яркий
    DISP_MODE 1         // в правом верхнем углу отображать: 0 - год, 1 - день недели, 2 - секунды
    WEEK_LANG 1         // язык дня недели: 0 - английский, 1 - русский (транслит)
    DEBUG 0             // вывод на дисплей лог инициализации датчиков при запуске. Для дисплея 1602 не работает! Но дублируется через порт!
    PRESSURE 1          // 0 - график давления, 1 - график прогноза дождя (вместо давления). Не забудь поправить пределы гроафика
    CO2_SENSOR 1        // включить или выключить поддержку/вывод с датчика СО2 (1 вкл, 0 выкл)
    DISPLAY_TYPE 1      // тип дисплея: 1 - 2004 (большой), 0 - 1602 (маленький)
    DISPLAY_ADDR 0x27   // адрес платы дисплея: 0x27 или 0x3f. Если дисплей не работает - смени адрес! На самом дисплее адрес не указан
    
    // пределы отображения для графиков
    TEMP_MIN 15
    TEMP_MAX 35
    HUM_MIN 0
    HUM_MAX 100
    PRESS_MIN -100
    PRESS_MAX 100
    CO2_MIN 300
    CO2_MAX 2000
	
<a id="chapter-5"></a>
## FAQ
### Основные вопросы
В: Как скачать с этого грёбаного сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видос http://alexgyver.ru/arduino-first/

В: Вылетает ошибка загрузки / компиляции!
О: Читай тут: https://alexgyver.ru/arduino-first/#step-5

В: Сколько стоит?  
О: Ничего не продаю.

### Вопросы по этому проекту

<a id="chapter-6"></a>
## Полезная информация
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)
* [Мои видеоуроки по пайке](https://www.youtube.com/playlist?list=PLOT_HeyBraBuMIwfSYu7kCKXxQGsUKcqR)
* [Мои видеоуроки по Arduino](http://alexgyver.ru/arduino_lessons/)