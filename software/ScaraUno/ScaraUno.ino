#include <Arduino.h>
#include "Config.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "Axis.h"
#include "AxisController.h"
#include "PinChangeInterrupt.h"

//steps needs to be configured

#define JOINT_A 0
#define JOINT_B 1
#define JOINT_C 2
#define JOINT_D 3

float current_angle;
#define Z_HEIGHT 0
#define A_ANGLE 60
#define B_ANGLE 0
#define C_ANGLE 0

volatile bool moving = true;

AxisController axisController;

enum ROBOT_STATE {
  STATE_NOT_HOMED,
  STATE_HOMING,
  STATE_HOMED
};


void pin_ISRZ() {Serial.print("z"); moving=false;}

void pin_ISRY() {Serial.print("y");moving=false;}

void pin_ISRX() {

  Serial.print("x");
  moving=false;
  
  /*
  for(int i=0;i<4;i++)
  {
    controller.getMotor(i).stop();  
 }

  moving = false;
  */
  
  //buttonState = digitalRead(buttonPin);
  //digitalWrite(ledPin, buttonState);
}

int incomingByte = 0;

void setAxisParameters()
{
  for(int i=0; i<4;i++)
  {
    
    axisController.pGetAxis(i)->_basicStepperDriver.begin(RPM, MICROSTEPS);
    axisController.pGetAxis(i)->_basicStepperDriver.setEnableActiveState(LOW);
    axisController.pGetAxis(i)->_basicStepperDriver.enable();
    axisController.pGetAxis(i)->_basicStepperDriver.setSpeedProfile(axisController.pGetAxis(i)->_basicStepperDriver.LINEAR_SPEED,ACCL,DECL);
  
  }
}

void initAxis()
{
  //create axis
  axisController.AddAxis(0, new Axis(STEPS, A_DIR, A_STEP,ENABLE_PIN) );
  axisController.AddAxis(1, new Axis(STEPS, B_DIR, B_STEP,ENABLE_PIN) );
  axisController.AddAxis(2, new Axis(STEPS, C_DIR, C_STEP,ENABLE_PIN) );
  axisController.AddAxis(3, new Axis(STEPS, D_DIR, D_STEP,ENABLE_PIN) );
  
  //add axis to sync controller
  axisController.CreateSyncDriveController();

  //set axis parameters
  setAxisParameters();

}

void setup() {

  Serial.begin(115200);

  initAxis();
}

// the loop function runs over and over again forever
void loop() {


  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte == 's')
    {
      Serial.print("limit status ");
    }

    
  }


if(moving)
{


delay(1000);
axisController.pGetSyncDriver()->rotate(Z_HEIGHT, A_ANGLE,B_ANGLE,C_ANGLE);
delay(1000);
axisController.pGetSyncDriver()->rotate(-Z_HEIGHT, -A_ANGLE,-B_ANGLE,-C_ANGLE);


}
 


}
