#pragma config(Motor,  port2,           armL1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           armL2,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           armR1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           armR2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           wheelLF,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           wheelLB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           wheelRF,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           wheelRB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          claw,          tmotorNone, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(true)
	{
	// Stuff for the arms

		if (vexRT[Btn8U])
		{
			motor[armL1] = 127;
			motor[armL2] = 127;
			motor[armR1] = 127;
			motor[armR2] = 127;

		}
		else if (vexRT[Btn8D])
		{
			motor[armL1] = -127;
			motor[armL2] = -127;
			motor[armR1] = -127;
			motor[armR2] = -127;
		}
		else
		{
			motor[armL1] = 0;
			motor[armL2] = 0;
			motor[armR1] = 0;
			motor[armR2] = 0;
		}

		// fwd + strafe

		if ((unsigned short)vexRT[Ch4] > (unsigned short)vexRT[Ch3])
		{
			motor[wheelLF] = vexRT[Ch4];
			motor[wheelLB] = -vexRT[Ch4];

			motor[wheelRF] = vexRT[Ch4];
			motor[wheelRB] = -vexRT[Ch4];
		}
		else
		{
			motor[wheelLF] = vexRT[Ch3];
			motor[wheelLB] = vexRT[Ch3];

			motor[wheelRF] = -vexRT[Ch3];
			motor[wheelRB] = -vexRT[Ch3];
		}

		// turn

		motor[wheelLF] = vexRT[Ch1];
		motor[wheelLB] = vexRT[Ch1];
		motor[wheelRF] = vexRT[Ch1];
		motor[wheelRB] = vexRT[Ch1];
	}
}

// LEFT STICK FOR FORWARD / STRAFE
// RIGHT STICK FOR TURN

// if left > forward:
//			left
// if -left > forward:
//			right
