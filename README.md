Arductrl
========

A simple Library for talking with an Arduino over the serial port

How to use?
1.  include Arducontrol.h
2.  initialize an Arducontrol object with the right Serialport
3.  setup the Serialpor
4.  create your command-functions void function(char *data)
5.  register your commands with Arducontrol.AddCmd("command name", function);
6.  run Arducontrol.ReadSerial in your loop




  #include <Arducontrol.h>
  
  Arducontrol ctrl = Arducontrol(Serial);
  
  void hello(char *data){
    Serial.print("Hello ");
    Serial.println(ctrl.GetString());
  }
  
  
  void setup(){
    Serial.begin(115200);
    
    ctrl.AddCmd("hello", hello);
    
  }
  
  void loop(){
    ctrl.ReadSerial();
    delay(200);
    
  }
