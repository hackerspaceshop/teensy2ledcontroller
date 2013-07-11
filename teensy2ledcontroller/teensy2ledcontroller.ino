

#include "SPI.h"
#include "ws2801.h"


int NUMLEDS=160;


int dataPin   = PIN_B1;   
int clockPin  = PIN_B2;    
int buttonPin = PIN_D3;


// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
ws2801 strip = ws2801(NUMLEDS, dataPin, clockPin);




// rcv is set from infrared_async lib everytime we receive seom kind of data over IR
// we can than interrupt whatever we are doing and switch over to the new action
unsigned long rcv=0;



// this defines which animation we are currently running.
// An animation is some kind of pattern that used gc() to get its color. so you can have a blinking animation in different colors for example.



int current_animation=0;


// ADD YOUR OWN DEFINES HERE.. THERE ARE 4 UNUSED BUTONS
#define ANIM_KITT           4
#define ANIM_SPARKLE        5
#define ANIM_STROBO         6
#define ANIM_RAINBOW        7
#define ANIM_STATIC         8
#define ANIM_POLIZEI        9

#define COLOR_RAINBOW 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_BLUE 3
#define COLOR_YELLOW 4
#define COLOR_MAGENTA 5
#define COLOR_TURKIS 6
#define COLOR_WHITE  7




void setup()
{
  
  Serial.begin(9600);
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

  
   // in infrared_async
  attachInterrupt(0,ir_receive,CHANGE);

  pinMode(buttonPin,INPUT_PULLUP);  
  
  
  
  //turn_on();
  set_global_color(7); //white
  current_animation=ANIM_STATIC;
}





void show()
{
 if(!rcv)
   strip.show();       
}






// should be in animations tab but wont compile that way.
//fuckig arduino..


int      colorstepper=1;   //1-255  (the larger the less colors you get from rc()


void loop() {
 
  
  if(rcv) handle_ir();
  run_current_animation();
  
  
}


















      




void handle_ir()
{
  
  
  
  Serial.print("handle_ir() current rcv = ");
  Serial.println(rcv,HEX);
  
switch (rcv)
    {
       
case 0xa25d: // ch-   Speed -
 set_global_tempo(1);  
break;

case 0x629d: // ch+   Speed +
 set_global_tempo(0); 
break;




case 0xe21d:  // ch+ (top right)  OFF
  current_animation=0;

/*
  if(!off)
  {
   // turn off 
   fast_fadeout();
   off=1;
  }
  else
  {
   turn_on(); 
   off=0;     
  }
*/
   fast_fadeout();
break;


case 0x22dd: // <<
 // BLANK UDEF1
 //current_animation=ANIM_UDEF1;
break;


case 0x2fd: // >>
 // BLANK UDEF2
 //current_animation=ANIM_UDEF2;
break;


case 0xc23d: // >||
 // BLANK UDEF3
 //current_animation=ANIM_UDEF3; 
break; 
 


case 0xe01f: // -
 set_global_color(1); //red
current_animation=ANIM_KITT;
break;


case 0xa857: // +
 current_animation=ANIM_SPARKLE;

break;


case 0x906f: // EQ
 current_animation=ANIM_STROBO;
 
break;


case 0x6897: // 0
  current_animation=ANIM_RAINBOW;
break;

case 0x9867: // 100+
  all_off();
 current_animation= ANIM_STATIC;

break;

case 0xb04f: // 200+
 current_animation=ANIM_POLIZEI;
break; 

case 0x30cf: //1
//UDEF4
 //current_animation=ANIM_UDEF4;
break; 




case 0x18e7: //2
  set_global_color(7); //WHITE
break;

case 0x7a85: //3
 set_global_color(0);// RAINBOW
 break;

case 0x10ef: //4
 set_global_color(6); //r+g = YELLOW
 break;
case 0x38c7: //5
 set_global_color(4); //r+b = CYAN
 break;
case 0x5aa5: //6
  set_global_color(5); //g+b = turkis
 break;
 
case 0x42BD: //7
 set_global_color(3); //red
break;

case 0x4AB5: //8
 set_global_color(2); //green
break;

case 0x52ad: //9
 set_global_color(1); //blue
break;
    
    
    
    
    
    
    }
  
 
  rcv=0; // reset IR signal received 
  return; 
  
  
  
  
}








void run_current_animation()
{
 switch (current_animation)
 {
   
  case ANIM_RAINBOW:
   rainbow(10); 
  break; 

  case ANIM_STATIC:
   static_color(); 
  break; 

  case ANIM_POLIZEI:
   
   polizei();
  break;
   
   
  case ANIM_KITT: 
    kitt(50);
  break; 


  case ANIM_SPARKLE: 
     sparkle(); 
  break;
  
  case ANIM_STROBO:
    strobo();
  break;   
  
  

 }
 
 return;
 
}




