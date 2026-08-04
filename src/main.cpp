// include any project and upload it
// do NOT mix project. if you do, make a new project
# include "sensor_proj.cpp"

SensorProj sp;

void setup()
{
    Serial.begin(9600);
    sp.activatePins();

}

void loop()
{

    sp.temperatureF = sp.getTemperature();
    sp.temperatureC = (sp.temperatureF - 32) * (5.00/9.00);
    
    sp.sendPing();

    sp.time = pulseIn(sp.echoPin, HIGH) / 1000000.00;
    sp.speedOfSound = (331.3 + (0.606 * sp.temperatureC)) * 39.3701; // in inches per second
    sp.distance = (sp.speedOfSound * sp.time) / 2.00;

    sp.printTemperature();
    sp.printDistance();

}


