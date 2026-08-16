#include <Arduino.h>
#include <LiquidCrystal.h>

// uses the thermistor to get the temperature
// then the temperature is used to calculate the speed of sound
// finally the speed of sound is used to calculate the distance

class SensorProj
{
    public:
        LiquidCrystal LCD;

        const uint8_t triggerPin{13};
        const uint8_t echoPin{11};
        const uint8_t thermistorPin{A0};
        const uint8_t buttonPin{12};
        const uint8_t buzzerPin{8};

        uint8_t state = 0;
        const uint8_t tempState = 0;
        const uint8_t distState = 1;
        
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
    
        // constructor
        SensorProj(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5, uint8_t p6): 
            LCD(p1, p2, p3, p4, p5, p6) {}

        // for void setup()
        void activatePins()
        {
            pinMode(triggerPin, OUTPUT); // we're writing to the trigger pin using the echo pin --> output
            pinMode(echoPin, INPUT); // we're reading from the echo pin --> input
            pinMode(buttonPin, INPUT);
            pinMode(buzzerPin, OUTPUT);
            LCD.begin(16,2);
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

        void sendPing() // sets the time it took for the ping to travel and return
        {
            digitalWrite(triggerPin, LOW);
            delayMicroseconds(2); // delay by 2 milisecond
            // can also do this: delayMicroseconds(2000);
            digitalWrite(triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(triggerPin, LOW); // trigger pulse ends here by bringing it low. by the end of this, a pulse was sent
            time = pulseIn(echoPin, HIGH) / 1000000.00;
        }

        void setSoundandDistance()
        {
            speedOfSound = (baseSpeedFahr + (FahrRate * temperatureF)) * inchesPerMeter; // in inches per second
            distance = (speedOfSound * time) / 2.00;

            if(distance <= 20.00) // maximum distance needed to 
            {
                digitalWrite(buzzerPin, HIGH);
                delay(30);
                digitalWrite(buzzerPin, LOW);
            }
        }

        void printDistance()
        {
            LCD.setCursor(0,0);
            LCD.print("Distance: ");
            LCD.setCursor(0,1);
            LCD.print(distance);
            LCD.print(" inches");
            delay(1000);
            
        }

        void printTemperature()
        {
            LCD.setCursor(0,0);
            LCD.print("Temperature: ");
            LCD.setCursor(0,1);
            LCD.print(temperatureF);
            LCD.print(" *F");
            delay(1000);
        }

        void displayInfo()
        {
            if(digitalRead(buttonPin) == HIGH) 
            {
                if(state == tempState) 
                {
                    state = distState;
                }
                else
                {
                    state = tempState;
                }
                LCD.clear();
            }
            
            if(state == tempState)
            {
                
                printTemperature();
            }
            else
            {
                LCD.clear();
                printDistance();
            }

        }
};