// include any project and upload it
// do NOT mix project. if you do, make a new project
#include "sensor_proj.cpp"

SensorProj sp(10, 9, 5, 4, 3, 2);

void setup()
{
    Serial.begin(9600);
    sp.activatePins();
}

void loop()
{
    
    sp.getTemperature();
    sp.sendPing();

    sp.setSpeedOfSound();
    sp.setDistance();

    sp.displayInfo();
}