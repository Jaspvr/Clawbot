//configuring sensors and motors
#pragma config(Sensor, in1,    irsignal,       sensorReflection)
#pragma config(Sensor, dgtl1,  button,         sensorTouch)
#pragma config(Sensor, dgtl3,  input,          sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  redlight,       sensorLEDtoVCC)
#pragma config(Sensor, dgtl8,  leftswitch,     sensorTouch)
#pragma config(Sensor, dgtl9,  rightswitch,    sensorTouch)
#pragma config(Motor,  port1,           rightmotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           leftmotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           arm,           tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//There are several functions that perform specific actions, and they are all called in main to perform the task


//TARGET DETECTION:
//function to determine if the IR sensor is detecting the target, returns true if we are, false if not
	int IRDetection(){
		//initialize 2 variables for the values recorded by IR sensor
		int min = SensorValue[irsignal];
		int max = SensorValue[irsignal];
		/*since the value read by the IR sensor varies even when it's not detecting the target,
			and it varies more when we sense the target, we need to find when the there is more variance.
		To do this, we go through a quick loop (0.05 * 5 seconds) and retrieve the maximum and minimum read values during the 5 increments of the loop.
		We return the value of the variance, if it is past a certain threshold number (changes based on distance away from ir signal), then it means
			we are detecting the signal and should take appropriate action in main.
		*/
		int i = 0;
		while( i<5 ){
			if (SensorValue[irsignal] < min) {
				min = SensorValue[irsignal];
			}else if (SensorValue[irsignal] > max) {
				max = SensorValue[irsignal];
			}
			wait1Msec(50);
			i++;
		}

		//difference between max and min is the variance
		return (max-min);
}

    //IN THE RIGHT PLACE TO DROP
    bool correctPositioning(){
        	//if the target is straight ahead and the appropriate distance away, return true
			if( SensorValue(input)<=6 ){
                return true;
			//For too far, return false
			}else if( SensorValue(input)>6){
                return false;
			}
	}

        //TOO CLOSE TO WALL
        //returns true if too close to a wall, false if not
        int wallDetection(){
			if(SensorValue(input)<20 || SensorValue(leftswitch)!=0 || SensorValue(rightswitch)!=0 ){
				return true;
			}else{
				return false;
			}
        }

        //DROP BALL
		//spins arm motor to dispense the ball onto the target, and resets the arm
        void dropBall(){
		    motor[arm]= -15;
		    wait1Msec(2000);
		    motor[arm]= 0;
			wait1Msec(500);
			motor[arm]= 15;
		    wait1Msec(2000);
		    motor[arm]= 0;
        }

//Below are 4 seperate drive methods. There could have been just one and a stop method, but this provides very readable code in main
        //3 second drive
        void drive3(){
            motor[leftmotor] = -40;
		    motor[rightmotor] = -40;
            wait1Msec(3000);
            motor[leftmotor] = 0;
		    motor[rightmotor] = 0;
		    wait1Msec(1000);
        }

        //1.5 second drive
        void drive1(){
            motor[leftmotor] = -40;
		    motor[rightmotor] = -40;
            wait1Msec(1500);
            motor[leftmotor] = 0;
		    motor[rightmotor] = 0;
		    wait1Msec(1000);
        }

        //0.5 second drive
        void drive0(){
            motor[leftmotor] = -40;
		    motor[rightmotor] = -40;
            wait1Msec(500);
            motor[leftmotor] = 0;
		    motor[rightmotor] = 0;
		    wait1Msec(500);
        }

		//0.25 second drive
        void drive00(){
            motor[leftmotor] = -40;
		    motor[rightmotor] = -40;
            wait1Msec(250);
            motor[leftmotor] = 0;
		    motor[rightmotor] = 0;
		    wait1Msec(500);
        }


        //SPIN
		//spins the robot to face the other way, we use this after dropping the ball
        void spin180(){
            motor[leftmotor] = -40;
		    motor[rightmotor] = 40;
            wait1Msec(3500);
            motor[leftmotor] = 0;
		    motor[rightmotor] = 0;
            wait1Msec(1000);
        }
        //JOB COMPLETION
		//Move back, turn around, and drive until in close proximity of a wall
        void retreat(){
            wait1Msec(1000);
            motor[leftmotor] = 40;
		    motor[rightmotor] = 40;
            wait1Msec(2000);
            spin180();
            while(!wallDetection()){
                motor[leftmotor] = -40;
		    	motor[rightmotor] = -40;
            }
        }



    task main(){
        //To stop process during testing
        while(SensorValue(leftswitch)==0){

			//initialize the process by clicking the button
	        while( SensorValue[button] == 0 ){
			    motor[leftmotor] = 0;
			    motor[rightmotor] = 0;
		    }

			//spin until facing open space
			while(SensorValue[input]<200){
				motor[rightmotor] = 35;
		        motor[leftmotor] = -35;
			}

			//move into arena space towards the target
			drive3();

			//detect the target, using a threshold of 40
	        while(IRDetection()<40){
		        //robot spins
		        motor[rightmotor] = 35;
		        motor[leftmotor] = -35;
	        }

	        //IR signal faces the target, stop spinning
	        motor[rightmotor] = 0;
	        motor[leftmotor] = 0;

            //now we need to enter a loop until we are at the appropriate distance from the device
			//
            //Until less than 100cm away from the target, move forward, detect and straigten, repeat
            while(SensorValue(input) > 100){
                drive1();
                while(IRDetection()>50){
                    motor[rightmotor] = 40;
		            motor[leftmotor] = -40;
                }
                motor[rightmotor] = 0;
	            	motor[leftmotor] = 0;
            }
            //now we are closer to the target, use a larger threshold for the IRDetection
            while(SensorValue(input) > 50){
                drive0();
                while(IRDetection()>100){
                    motor[rightmotor] = 40;
		            motor[leftmotor] = -40;
                }
                motor[rightmotor] = 0;
	            motor[leftmotor] = 0;
            }
            //even closer, use even larger threshold, and do not do a full spin, in case the sides would hit the target
            while(SensorValue(input) > 25){
                drive00();
                while(IRDetection()>200){
                    motor[rightmotor] = 40;
		            motor[leftmotor] = -40;
					wait1Msec(2000);
					motor[rightmotor] = -40;
		            motor[leftmotor] = 40;
					wait1Msec(2000);
                }
                motor[rightmotor] = 0;
	            motor[leftmotor] = 0;
            }
			//super close to the target now; while we still are not the right distance away to drop, drive for 0.25 seconds 
				//then make sure we are still in front of the target. Use a higher detection threshold given how close we are
				
			while(!correctPositioning()){
                drive00();
                while(IRDetection()>500){
                    motor[rightmotor] = 40;
		            motor[leftmotor] = -40;
					wait1Msec(2000);
					motor[rightmotor] = -40;
		            motor[leftmotor] = 40;
					wait1Msec(2000);
                }
                motor[rightmotor] = 0;
	            motor[leftmotor] = 0;
            }

            //Now that we have exited the loop, we must be at perfect positioning, drop the ball, retreat, and signal job completion
            dropBall();
            retreat();
			SensorValue(redlight) = 1;
        }
		//for if the left switch is clicked (in testing)
        motor[leftmotor] = 0;
		motor[rightmotor] = 0;
	}
