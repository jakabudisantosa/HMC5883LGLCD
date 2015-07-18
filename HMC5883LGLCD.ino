#include <LCD5110_Graph.h>
#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
int error = 0;
int rDerajat;
int LED=7;
LCD5110 myGLCD(2, 3, 4, 5, 6);
extern uint8_t SmallFont[];
void setup()
{
  Serial.begin(9600);
 pinMode(LED,OUTPUT);
  Serial.println("Starting the I2C interface.");
  Wire.begin();
  Serial.println("Constructing new HMC5883L");
  compass = HMC5883L();
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3);
  if(error != 0)
      Serial.println(compass.GetErrorText(error));
      Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous);
      if(error != 0)
      Serial.println(compass.GetErrorText(error));
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont); 
  myGLCD.setContrast(90);
}

void loop(){
  MagnetometerRaw raw = compass.ReadRawAxis();
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  int MilliGauss_OnThe_XAxis = scaled.XAxis;
   float heading = atan2(scaled.YAxis, scaled.XAxis);
   float declinationAngle = 0.0457;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
    if(heading > 2*PI)
    heading -= 2*PI;
    float headingDegrees = heading * 180/M_PI;   
  digitalWrite(LED,LOW);      
  myGLCD.clrScr();
  myGLCD.print("U", 41+(cos((headingDegrees+90)/180*PI)*18) ,23-(sin((headingDegrees+90)/180*PI)*18));
  myGLCD.print("TL", 41+(cos((headingDegrees+45)/180*PI)*18) ,23-(sin((headingDegrees+45)/180*PI)*18));
  myGLCD.print("T", 41+(cos((headingDegrees+0)/180*PI)*18) ,23-(sin((headingDegrees+0)/180*PI)*18));
  myGLCD.print("TG", 41+(cos((headingDegrees+315)/180*PI)*18) ,23-(sin((headingDegrees+315)/180*PI)*18));
  myGLCD.print("S", 41+(cos((headingDegrees+270)/180*PI)*18) ,23-(sin((headingDegrees+270)/180*PI)*18));
  myGLCD.print("BD", 41+(cos((headingDegrees+225)/180*PI)*18) ,23-(sin((headingDegrees+225)/180*PI)*18));
  myGLCD.print("B", 41+(cos((headingDegrees+180)/180*PI)*18) ,23-(sin((headingDegrees+180)/180*PI)*18));  
  myGLCD.print("BL", 41+(cos((headingDegrees+135)/180*PI)*18) ,23-(sin((headingDegrees+135)/180*PI)*18));
  
  myGLCD.printNumI(headingDegrees, 0, 0);
  //Serial.println(headingDegrees);
   Output(raw, scaled, heading, headingDegrees);
  myGLCD.update();

  digitalWrite(LED,HIGH);
 


delay(500);
}

void Output(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees)
{
   Serial.print("Raw:\t");
   Serial.print(raw.XAxis);
   Serial.print("   ");   
   Serial.print(raw.YAxis);
   Serial.print("   ");   
   Serial.print(raw.ZAxis);
   Serial.print("   \tScaled:\t");
   
   Serial.print(scaled.XAxis);
   Serial.print("   ");   
   Serial.print(scaled.YAxis);
   Serial.print("   ");   
   Serial.println(scaled.ZAxis);

   Serial.print("   \tHeading:\t");
   Serial.print(heading);
   Serial.print(" Radians   \t");
   Serial.print(headingDegrees);
   Serial.println(" Degrees   \t");
}
