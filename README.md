Arductrl
========

A simple Library for talking with an Arduino over the serial port

### How to use?
* include Arducontrol.h
* initialize an Arducontrol object with the right Serialport
* setup the Serialport
* create your command-functions void function(char *data), data will contain all parameters sent with the command
* register your commands with Arducontrol.AddCmd("command name", function);
* run Arducontrol.ReadSerial in your loop

Example
will listen for $hello,commandarguments
and answers with "Hello commadarguments"

```
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
```
