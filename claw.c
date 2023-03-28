/*
 This program drops the ball when positioned over the target.
 The mechanical system in place allows us to simply spin one motor for 2 seconds to complete the task.
 */
//Configuring the button and motor
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Motor,  port3,           blackMotor1,   tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(1){
        
    //initialize button value as a variable (if == 1 then button is pressed,
    //if == 0  thenbutton is not pressed)
	int sensvalue1=SensorValue(button1);

    //if button is pressed, turn on the motor for 2 seconds
	if(sensvalue1==1){
		motor[blackMotor1]= -15;
		wait1Msec(2000);
		motor[blackMotor1]= 0;
	}

}



}