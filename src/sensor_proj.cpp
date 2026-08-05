#include <Arduino.h>
// uses the thermistor to get the temperature
// then the temperature is used to calculate the speed of sound
// finally the speed of sound is used to calculate the distance


class SensorProj
{
    public:
        const unsigned int triggerPin{13};
        const unsigned int echoPin{11};
        const unsigned int thermistorPin{A0};

        const double RESISTOR{10000.00};
        const double THERMISTOR{10000.00};

        const double NOMINAL{25.00};
        const double B{3977.00};

        const double FahrRate = 0.33;
        const double baseSpeedFahr = 320.3;
        const double inchesPerMeter = 39.3701;

        double time; // in microseconds
        double temperatureF;
        double speedOfSound; // in feet per second, it's 776.5 in mi/hr
        double distance;
    
        // for void setup()
        void activatePins()
        {
            pinMode(triggerPin, OUTPUT); // we're writing to the trigger pin using the echo pin --> output
            pinMode(echoPin, INPUT); // we're reading from the echo pin --> input
            pinMode(thermistorPin, INPUT);
        }

        // rest goes in void loop();
        void getTemperature() // returns in fahrenheit
        {
            double thermVolt = (5.00 / 1023.00) * analogRead(thermistorPin);
            double resistorVolt = 5.00 - thermVolt;

            double thermResistance = thermVolt / (resistorVolt / RESISTOR);

            double ln = log(thermResistance / THERMISTOR);
            double temp = (1.00 / ((ln / B) + (1 / (NOMINAL + 273.15)))); // in kelvins

            temperatureF = ((temp - 273.15) * (9.00/5.00)) + 32.00;
        }

        void printDistance()
        {
            Serial.print("closest object is ");
            Serial.print(distance);
            Serial.println(" inches away");
            delay(1000);
            
        }

        void printTemperature()
        {
            Serial.print("temperature: ");
            Serial.print(temperatureF);
            Serial.println(" *F");

        }

        void sendPing() // sets the time it took for the ping to travel and return
        {
            digitalWrite(triggerPin, LOW);
            delayMicroseconds(2000); // delay by 2 milisecond
            // can also do this: delayMicroseconds(2000);
            digitalWrite(triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(triggerPin, LOW); // trigger pulse ends here by bringing it low. by the end of this, a pulse was sent
            time = pulseIn(echoPin, HIGH) / 1000000.00;
        }

        void setSpeedOfSound()
        {
            speedOfSound = (baseSpeedFahr + (FahrRate * temperatureF)) * inchesPerMeter; // in inches per second
        }

        void setDistance()
        {
            distance = (speedOfSound * time) / 2.00;
        }

};