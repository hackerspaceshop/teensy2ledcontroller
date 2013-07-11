

int TimeStart= 4000; //2500  
int TimeZero = 488; //500    THIS IS CALCULATED DYNAMICALLY LATER
int TimeOne  = 1612; //1000  THIS IS CALCULATED DYNAMICALLY LATER



// check if the time was in range +/- 25%
#define IS_X(t,x) ((t > 3*(x)/4) && (t < 5*(x)/4))
#define IS_0(t,TimeZero) IS_X(t,TimeZero)
#define IS_1(t,TimeOne) IS_X(t,TimeOne)
#define IS_S(t,TimeStart) IS_X(t,TimeStart)




// helper functions

unsigned long elapsedSince(unsigned long since, unsigned long now)
{
  return since & now ? now-since : 0xFFFFFFFFUL - (now - since);
}

unsigned long elapsedSince(unsigned long since)
{
  return elapsedSince( since, micros() );
}



// pin state change interrupt based IR receiver
// observing the IR pin changes, measure time between interrupts
// use state machine to decide what to do

enum
{
  ISR_IDLE,   // nothing is/was happening (quiet)
  ISR_START,  // start of sequence, was waiting for a header signal
  ISR_BIT_ON, // transsmitting a bit (IR carrier turned on)
  ISR_BIT_OFF // in an OFF bit slot (IR carrier turned off)
}
isrState = ISR_IDLE;

unsigned long isrLastTimeStamp;
//unsigned long rcv=0;
unsigned long isrNewCmd;



byte isrBitLen = 32;
byte isrBitCnt;

int isrZeroReference=0;
int isrOneReference=0;



void ir_receive()
{ 
  // receiving a modulated IR signal makes the pin go low (active low)
  byte transmitting = digitalRead(PIN_D0);



  
  // compute elapsed time since last change
  unsigned elapsed;
  unsigned long timeStamp = micros();
  elapsed = elapsedSince(isrLastTimeStamp,timeStamp);
  isrLastTimeStamp = timeStamp; 






// do this only if we just switched back to pullup .. and received some data
// we received a signal, took a timestamp and now the pin is high again (pullup)  
if( !transmitting ) 
{

 //     Serial.println(elapsed);
      digitalWrite(11, HIGH);   



  switch( isrState )
  {
   case ISR_IDLE :
        isrZeroReference=0;
        isrOneReference=0;
        isrBitCnt=0;
        isrNewCmd=0;
   
       if(elapsed>30000) // 31128, 33608
          isrState=ISR_START;
       else
        digitalWrite(11, LOW);         
    break;
    
    
// after receiving the beginning with a interval of over 30000
// we receive a second startsignal over 4000 (checked with IS_S() )
  case ISR_START:
    digitalWrite(11, HIGH);
    isrBitCnt = 0;
     if(IS_S(elapsed,TimeStart))
      isrState = ISR_BIT_ON; // bits are now rolling
     else
     {
      isrState = ISR_IDLE;   // wrong timing of start or pin state 
 //             Serial.print("\n---  UIUI:  ");
 //     Serial.println(elapsed); 

     }  
    break;





// the next signal we read can be 0 or 1 debending on pulselength
// 504 = 0
//1628 = 1

  case ISR_BIT_ON:

// Serial.println("here: ISR_BIT_ON");
// the first eight bit are definitive 0 (486)


//followed by eight 1s



     if(!transmitting)
     {
       
       isrBitCnt ++;
       if(isrBitCnt<=8)
       {
         isrZeroReference+=elapsed;

         break;
       }
       else if(isrBitCnt<=16)
       {
        isrOneReference+=elapsed;
        break;
       }
       else if(isrBitCnt==17)
       {
          // we got the first 16 bit for berechnung of the durchschnittswert .. :) 
                
        TimeZero = (isrZeroReference/8); //500
        TimeOne  = (isrOneReference/8); //1000
         
         
        isrZeroReference=0;
        isrOneReference=0;
 //       Serial.print("\n0: ");
 //       Serial.println(TimeZero);
 //       Serial.print("1: ");
 //       Serial.println(TimeOne);
   
       }
       else if((isrBitCnt==33))
       {
        // we collected all the bits!

       
        
         // global rcv + set now
         rcv=isrNewCmd;
         
        //clean up and return         
         isrState = ISR_IDLE;
      
        break;  
       }
       
       
       
       
       
         //    Serial.println(elapsed); 
       
  //    isrState = ISR_BIT_OFF;
      isrNewCmd <<= 1;
      


      if( IS_1(elapsed,TimeOne) )
      {
        isrNewCmd |= 1;
  //      Serial.print(1);
      }
      else if( IS_0(elapsed,TimeZero) )
      {
       isrNewCmd |= 0;
  //      Serial.print(0);
      }
      else
      { 
  //      Serial.println("\n-----FUCK-----");
  //      Serial.print(elapsed);
  //      Serial.print( " is not 1/0 @ step: ");
  //      Serial.print(isrBitCnt);
        isrState = ISR_IDLE; // bad timing, start over
      }
    }
    else 
    {
       isrState = ISR_IDLE; // bad state (should never get here...)
       Serial.println("how did we get here??");
    }
    break;
    
  } // switch
 } // if !transmitting
} // function




