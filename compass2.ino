

#include <LiquidCrystal.h>
#include <Wire.h>  



LiquidCrystal lcd( 12, 11, 7, 8, 9, 10 );
//Pins for the 16 x 2 LCD Display
//LiquidCrystal(rs, enable,d0,d1,d2,d3,d4,d5,d6,d7)

const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address for compass
const byte hmc5883ModeRegister = 0x02;
const byte hmcContinuousMode = 0x00;
const byte hmcDataOutputXMSBAddress = 0x03;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();  //make sure screen is clear.
  lcd.print("magnetic compass"); 
  lcd.setCursor(0,1);
  lcd.print("AKSHAY SHARMA");
  delay(5000);
  Wire.begin(); // Start the i2c communication


  //Initialise the Digital Compass
  Wire.beginTransmission(hmc5883Address);  //Begin communication with compass
  Wire.write(hmc5883ModeRegister);  //select the mode register
  Wire.write(hmcContinuousMode); //continuous measurement mode
  Wire.endTransmission();

}

void loop()
{
  int x,y,z; //triple axis data.
  //Tell the HMC5883L where to begin reading the data
  Wire.beginTransmission(hmc5883Address);
  Wire.write(hmcDataOutputXMSBAddress);  //Select register 3, X MSB register
  Wire.endTransmission();

  //Read data from each axis
  Wire.requestFrom(hmc5883Address,6);
  if(6<=Wire.available())
  {
    x = Wire.read()<<8; //X msb
    x |= Wire.read();   //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read();   //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read();   //Y lsb    
  }


  int angle = atan2((double)y,(double)x)/M_PI*180;
  if (angle < 0)
  {
    angle = angle + 360;
  }

  //Reporting the data to the LCD Screen
  lcd.clear();  //make sure screen is clear again.
  lcd.setCursor(0,0); //Top left hand corner
  lcd.print("XYZ:");
  lcd.setCursor(0,1); //Bottom left corner
  lcd.print("dir: ");
  if((angle < 22.5) || (angle > 337.5 ))
    lcd.print("north");
  if((angle > 22.5) && (angle < 67.5 ))
    lcd.print("north-east");
  if((angle > 67.5) && (angle < 112.5 ))
    lcd.print("east");
  if((angle > 112.5) && (angle < 157.5 ))
    lcd.print("south-West");
  if((angle > 157.5) && (angle < 202.5 ))
    lcd.print("south");
  if((angle > 202.5) && (angle < 247.5 ))
    lcd.print("south west+");
  if((angle > 247.5) && (angle < 292.5 ))
    lcd.print("west");
  if((angle > 292.5) && (angle < 337.5 ))
    lcd.print("north west");

  //Print the approximate direction
  Serial.print("You are heading ");
  if((angle < 22.5) || (angle > 337.5 ))
    Serial.print("north");
  if((angle > 22.5) && (angle < 67.5 ))
    Serial.print("north-east");
  if((angle > 67.5) && (angle < 112.5 ))
    Serial.print("east");
  if((angle > 112.5) && (angle < 157.5 ))
    Serial.print("south-east");
  if((angle > 157.5) && (angle < 202.5 ))
    Serial.print("south");
  if((angle > 202.5) && (angle < 247.5 ))
    Serial.print("south west");
  if((angle > 247.5) && (angle < 292.5 ))
    Serial.print("west");
  if((angle > 292.5) && (angle < 337.5 ))
    Serial.print("north-west");

  lcd.setCursor(4,0);
  lcd.print(x);
  lcd.setCursor(9,0);
  lcd.print(y);
  lcd.setCursor(13,0);
  lcd.print(z);

  lcd.setCursor(9,1);
  lcd.print("");

  delay(250);

}
