#include <Arduino.h>
#include "Event.h"


//using namespace std;

class BaseEmiter  {

public:
    BaseEmiter() {};


    EVENT_SIGNAL(click,int){2};                     //Create a Signal with int as data type and allow 2 slots to connect
    EVENT_SIGNAL(test, int ,float);                 //Create a Signal with 2 parameter (int and float) with default 1 connection
    EVENT_SIGNAL(voidTest);                          //Create a Signal with no connection
    EVENT_SIGNAL(redirect,bool);
};


class BaseReceiver {

    EVENT_REG_FOR_SLOTS(BaseReceiver)               // Register classe BaseReceiver to allow creating SLOTS

  private: 
    void onClickSlot(int i) //Implementation of the Slot onClick 
    {
        Serial.print("onClickSlot: i=");Serial.print(i);
    }

    void testSlot(int i ,float j) //Implementation of the Slot test
    {
        Serial.print("TestSlot: i=");Serial.print(i);
        Serial.print("TestSlot: j=2");Serial.println(j); 
    }


    void voidTestSlot(void) //Implementation of the Slot voidTest
    { 
       Serial.println("TestSlot executed: void");
    }

    void onRedirectSlot(bool status) {
      Serial.print("Redirected ---> Emiter ---> SecondReceiver ---> Receiver: "); Serial.println(status);
    }

public:
    EVENT_SLOT(onClick,int);                //Create a new slot onClick with parameter type int 
    EVENT_SLOT(test,int,float);             //Create a new slot test with parameter type int and float
    EVENT_SLOT(voidTest);                    //Create a new slot without any parameter

    EVENT_SLOT(onRedirect,bool);

};

class SecondReceiver {
  
  EVENT_REG_FOR_SLOTS(SecondReceiver)               // Register classe BaseReceiver to allow creating SLOTS


  private:  
    void onClickSlot(int i)
    {
        Serial.print("Param i = "); Serial.println(i);
    }

  public:
    EVENT_SLOT(onClick,int);                //Create a new slot onClick with parameter type int 
    EVENT_SIGNAL(onRedirect,bool);
};

class ThirdReceiver {
  
  EVENT_REG_FOR_SLOTS(ThirdReceiver)               // Register classe BaseReceiver to allow creating SLOTS


  private:  
    void onClickSlot(int i)
    {
        Serial.print("Param i = "); Serial.println(i);
    }

  public:
    EVENT_SLOT(onClick,int);                //Create a new slot onClick with parameter type int 
};


BaseEmiter emiter;
BaseReceiver receiver;
SecondReceiver secondReceiver;
ThirdReceiver thirdReceiver;

void setup() {
  
  //Test 1: Max two connection allowed, thridReceiver will not be executed...
  EVENT_CONNECT_SIGNAL2SLOT(emiter.m_click_signal,receiver.m_onClick_slot);
  EVENT_CONNECT_SIGNAL2SIGNAL(emiter.m_click_signal,secondReceiver.m_onClick_slot);
  EVENT_CONNECT_SIGNAL2SIGNAL(emiter.m_click_signal,thirdReceiver.m_onClick_slot);

  //Test2: Signal and slot connecton with multiple parameter
  EVENT_CONNECT_SIGNAL2SLOT(emiter.m_test_signal,receiver.m_test_slot);

  //Test3: Redirect Signal over another sigenl: Connect Signal_1 to Signal_2 and then Singal_2 to Signal_3;
  EVENT_CONNECT_SIGNAL2SIGNAL(emiter.m_redirect_signal,secondReceiver.m_onRedirect_signal);
  EVENT_CONNECT_SIGNAL2SLOT(secondReceiver.m_onRedirect_signal,emiter.m_redirect_signal);

  //Test4: Signal Slot without any parameter
  EVENT_CONNECT_SIGNAL2SLOT(emiter.m_voidTest_signal,receiver.m_voidTest_slot);
  
}

void loop() {

  Serial.println("TESTS: With EVENT_EMIT Macro:");
  Serial.println("");
  Serial.println ("Test 1: ");
  Serial.println("--------------------");
  EVENT_EMIT(emiter.m_click_signal,42);
  Serial.println("");
  
  Serial.println ("Test 2: ");
  Serial.println("--------------------");
  EVENT_EMIT(emiter.m_test_signal,24,3.14);
  Serial.println("");

  Serial.println ("Test 3: ");
  Serial.println("--------------------");
  EVENT_EMIT(emiter.m_redirect_signal,true);
  Serial.println("");

  Serial.println ("Test 2: ");
  Serial.println("--------------------");
  EVENT_EMIT(emiter.m_voidTest_signal);
  Serial.println("");

  delay(2);

  Serial.println("TESTS: With Emit Method:");
  Serial.println("");
  Serial.println ("Test 1: ");
  Serial.println("--------------------");
  emiter.m_click_signal.emit(42);
  Serial.println("");
  
  Serial.println ("Test 2: ");
  Serial.println("--------------------");
  emiter.m_test_signal.emit(24,3.14);
  Serial.println("");

  Serial.println ("Test 3: ");
  Serial.println("--------------------");
  emiter.m_redirect_signal.emit(true);
  Serial.println("");

  Serial.println ("Test 2: ");
  Serial.println("--------------------");
  emiter.m_voidTest_signal.emit();
  Serial.println("");
  
}