#include <Arducontrol.h>

Arducontrol ctrl = Arducontrol(Serial);

void hello(char *data){
  Serial.print("Hello ");
  Serial.println(ctrl.GetString());
}

void echo(char *data){
  int value = ctrl.GetInt();

  ctrl.StartCmd("ECHO");
  ctrl.SendInt(value);
  ctrl.EndCmd();
}

void setup(){
  Serial.begin(115200);
  ctrl.AddCmd("hello", hello);
  ctrl.AddCmd("echo", echo);
}

void loop(){
  ctrl.ReadSerial();
  delay(200);
}
