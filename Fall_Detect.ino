#include <Wire.h>
#include <L3G.h>
#include <LiquidCrystal.h>
#include <LSM303.h>
LSM303 compass;
L3G gyro;
const int led = 13;
const int button = 12;
int botIN;
//Accel
float a_x,a_y,a_z,a_vecraw;
//Gyro 
float g_x,g_y,g_z;
int g_vec;
bool phase1,phase2,phase3,fall = false;
int count,count2,count3 = 0;
unsigned long currentMillis,prevMillis;
void setup() {
 Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
  Wire.begin();
  if (!gyro.init())
  {
    Serial.println("Failed to autodetect altimu type!");
    while (1);
  }
  
  gyro.enableDefault();
  compass.init();
  compass.enableDefault();
}

void loop() {
  gyro.read();
  compass.read();
  botIN = digitalRead(button);
 g_x = (gyro.g.x*.00875) - 10;
 g_y = (gyro.g.y*.00875)+ 6;
 g_z = (gyro.g.z*.00875) + 2; 
 g_vec = pow(pow(g_x,2)+pow(g_y,2)+pow(g_z,2),.5);
 
 a_x =(compass.a.x*.00061)/9.8;
 a_y =(compass.a.y*.00061)/9.8;
 a_z =(compass.a.z*.00061)/9.8;
 a_vecraw = pow(pow(a_x,2)+pow(a_y,2)+pow(a_z,2),.5);
 int a_vec = a_vecraw * 10;

// Serial.print("GYRO: ");
// Serial.print(g_vec);
// Serial.print(" ACC: ");
// Serial.println(a_vec);

 if(botIN==HIGH){
  Serial.print("PANIC button pressed");
  fall=true;
  }
 if(a_vec <= 4 && phase2 == false){ //Accel goes below 0.4g
  phase1 = true;
   Serial.println(" phase 1 ON");
  }
 else if(a_vec>= 13 && phase1 == true){
    phase2 = true;
    phase1 = false;
    count = 0; 
    Serial.println(" phase 2 ON");
    }
 else if(g_vec>= 200 && g_vec<=550 && phase2==true){
    phase3=true;
    phase2=false;
    currentMillis = millis();
    
    Serial.println(" phase 3 ON");
    }
 else if((currentMillis-prevMillis >= 1500) && g_vec<=200 && phase3==true){
    fall=true;
    phase3=false;
    prevMillis = currentMillis;
    }
      
 else if(fall==true){
  digitalWrite(led,HIGH);
  Serial.println(" Fall Detected");
  fall=false;
  }

 if((count>=6 && phase1==true)){
    phase1= false;
    count = 0;
    }
 if((count>=6 && phase2==true)){
    phase2= false;
    count = 0;
    }

count++;
delay(100);
}
