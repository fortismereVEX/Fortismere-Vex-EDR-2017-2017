#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           middleMotor,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftArmMotor,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           rightArmMotor, tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while (true)
	{
		//motor[port1] = vexRT[Ch1];
		//motor[port2] = vexRT[Ch1];

		motor[leftMotor] = vexRT[Ch3];
		motor[rightMotor] = vexRT[Ch2];
		motor[middleMotor] = vexRT[Ch1];

		if(vexRT[Btn5U] == 1)
		{
			motor[leftArmMotor] = -32;
			motor[rightArmMotor] = -32;
		}
		else if(vexRT[Btn5D] == 1)
		{
			motor[leftArmMotor] = 32;
			motor[rightArmMotor] = 32;
		}
		else
		{
			motor[leftArmMotor] = 0;
			motor[rightArmMotor] = 0;
		}
	}
}
