// include any project and upload it
// do NOT mix project. if you do, make a new project
#include "sensor_proj.cpp"
#include <LiquidCrystal.h>

SensorProj sp;
LiquidCrystal LCD(10, 9, 5, 4, 3, 2);
void setup()
{
    Serial.begin(9600);
    sp.activatePins();
    LCD.begin(16, 2);
    LCD.setCursor(0, 0);
    LCD.print("distance:");
}

void loop()
{
    
    sp.getTemperature();
    sp.sendPing();

    sp.setSpeedOfSound();
    sp.setDistance();

    LCD.setCursor(0, 1);
    LCD.print(sp.distance);
    LCD.print(" inches");
    delay(1000);
}


