// globals are ugly but OOP sucks in arduino.
// i have no time to fuck around with the tabs and create libraries right now.
// i bet i will pay for this later :)

uint32_t prev_color =0;
uint32_t current_color =0;
int      colorcounter=1;   //1-255  (the larger the less colors you get from rc()
int      global_tempo=0;
uint32_t global_color=0;







/// ANIMATIONEN!!!



void turn_on()
{
  
 Serial.println("turn_on() called"); 
 set_all(Color(80,80,80)); 
 strip_show();
}



void fast_fadeout()
{
   Serial.println("fast_fadeout() called"); 
  
  
  for(int i=0;i<80;i+=15)
   {
    set_all(Color(80-i,80-i,80-i));
    strip_show();
   } 
   
  
  all_off();
  strip_show();
  
}






void sparkle()
{
  

    for (int i=0; i <strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(50,100,100) );
    } 
  

  // get one random pixel from outline
    int pxl=random(0,strip.numPixels());
  

  
  
      strip.setPixelColor(pxl, Color(200,255,255));  
      strip_show();   // write all the pixels out   
  
   delay_break(80);
   

}





void static_color()
{
// super!! 
set_all(gc()); 
strip_show();
}





void strobo()
{
 set_all(gc());
 strip_show(); 
delay_break(10*global_tempo);
 all_off(); 
 strip_show(); 
delay_break(10*global_tempo);
}


void kitt(int wait)
{
   

 
 for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, gc());
      if(rcv) return;
      strip_show();
      delay_break(wait);
      all_off();
  }


 for (int i=strip.numPixels(); i>=0; i--) {
      strip.setPixelColor(i, gc());
      if(rcv) return;
      strip_show();
      delay_break(wait);
      all_off();
  }




  
}







void polizei()
{
  
 uint32_t color = gc();
 
 set_all(color); strip_show();
 delay_break(20);
 all_off();strip_show();
 delay_break(20);
 set_all(color); strip_show(); 
 all_off();strip_show();
 delay_break(400);

  
}











// r,g,b = 255 max
void suspend_glow(int rx, int gx, int bx, int cutoff)
{
  int r = 0;
  int g = 0;
  int b = 0;
  
  
  for(int i=cutoff;i<255;i+=2)
  {
   if(rx){ if( i<rx ) r=i; } 
   if(gx){ if( i<gx ) g=i; } 
   if(bx){ if( i<bx ) b=i; }    




   set_all(Color(r,g,b));
   strip_show();

  }
  
  delay_break(1000);


  for(int i=255;i>cutoff;i-=5)
  {
   if(rx>0){ if(i<rx) r=i; } 
   if(gx>0){ if(i<gx) g=i; } 
   if(bx>0){ if(i<bx) b=i; }    
    
   set_all(Color(r,g,b));
   strip_show();

  }
  

  
  
  
}


void all_off()
{
   for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }  
//    strip_show();   // write all the pixels out
}


void set_all(uint32_t color)
{
   for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }    
}






// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      if(rcv) return;
      strip_show();
      delay_break(wait);
  }
}






// draw ONE inanimated rainbow
void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip_show();   // write all the pixels out
    delay_break(wait);
   if(rcv) return;
  }
}















///////// HELPERS


/* Helper functions */




// HELPERS gonna help




void strip_show()
{
//  if(!rcv) // set by remote control interrupt
     strip.show(); 

}




// get current rainbowcolor
// resets current_color after call
uint32_t rc()
{
  
  colorcounter+=colorstepper;  
  if(colorcounter>255)
    colorcounter=0;
    
  prev_color= current_color;  // maybe handy later .. dunno yet
  current_color= Wheel(colorcounter);  
    
  return  current_color;

  
}



 // get a color  
uint32_t gc()
{
 if(global_color) return global_color;
 else return rc();
 
} 






// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}





//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}




void set_global_tempo(int increse)
{
  if(increse)
  {
    global_tempo+=5;
  }
  else 
      global_tempo-=5;
  
  
  
  
  if(global_tempo<0) global_tempo=0;
  if(global_tempo>20) global_tempo=20;  
  
  
  Serial.print("Global tempo set to: ");
  Serial.println(global_tempo);
  
}




void set_global_color(int color)
{
  
  
 Serial.print("Setting global_color to: ");
 Serial.println(color); 
  

  
  
  
 if(color==COLOR_RAINBOW)
   global_color=0; // replaced by rc() in gc();
  
 if(color==COLOR_RED)
   global_color=Color(255,0,0); //r
 if(color==COLOR_GREEN)
   global_color=Color(0,255,0); //g
 if(color==COLOR_BLUE)
   global_color=Color(0,0,255); //b
 if(color==COLOR_YELLOW)
   global_color=Color(255,255,0); //r+g
 if(color==COLOR_MAGENTA)
   global_color=Color(255,0,255); //r+b 
 if(color==COLOR_TURKIS)
   global_color=Color(0,255,255); //g+b  
 if(color==COLOR_WHITE)
   global_color=Color(255,255,255); //weiss
  
 
  
}




void delay_break(int time)
{
 for (int i=0;i<time;i++)
 {
   if(rcv) return;
   
   delay(1);
 } 
}


