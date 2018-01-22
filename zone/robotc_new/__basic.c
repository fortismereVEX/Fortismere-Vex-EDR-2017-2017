#pragma config(Sensor, in1,    left_line_sensor, sensorLineFollower)
#pragma config(Sensor, in2,    right_line_sensor, sensorLineFollower)
#pragma config(Sensor, dgtl1,  left_encoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  right_encoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, sonar,          sensorSONAR_cm)
#pragma config(Motor,  port1,           right_back,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           right_front,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           right_middle,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           left_middle,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           left_front,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          left_back,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "robotc_base_include.c"

#define max(x, a) (x > a ? x : a)
#define min(x, a) (x < a ? x : a)
#define clamp(x, a, b) (min(b, max(a, x)))

void get_requested_delta(float *left, float *right) {
    int r = (vexRT[Ch1] + vexRT[Ch3]);
    int l = (vexRT[Ch1] - vexRT[Ch3]);

    *left  = abs(l) > 20 ? l : 0;
    *right = abs(r) > 20 ? r : 0;
}

float step_pid(float constant_p, float constant_i, float constant_d, float requested, float current_value, float *last_error, float *integral) {
    float error = current_value - requested;

    float derivitive = error - *last_error;
    *last_error      = error;

    if (constant_i != 0 && abs(error) < 50) {
        *integral = *integral + error;
    } else {
        *integral = 0;
    }

    return constant_p * error + constant_i * *integral + constant_d * derivitive;
}

task pid_loop() {
    // clear out the encoder
    SensorValue[left_encoder]  = 0;
    SensorValue[right_encoder] = 0;

    float current_left  = 0;
    float current_right = 0;

    float requested_left  = 0;
    float requested_right = 0;

    float last_error_left  = 0;
    float last_error_right = 0;

    float integral_left  = 0;
    float integral_right = 0;

    bool reset = true;

    while (true) {
        current_left  = SensorValue[left_encoder];
        current_right = SensorValue[right_encoder];

        float delta_requested_left  = 0;
        float delta_requested_right = 0;

        get_requested_delta(&delta_requested_left, &delta_requested_right);

        if (delta_requested_left == 0 && delta_requested_right == 0) {
            if (reset) {
                requested_left             = 0;
                requested_right            = 0;
                SensorValue[left_encoder]  = 0;
                SensorValue[right_encoder] = 0;

                last_error_left  = 0;
                last_error_right = 0;

                reset = false;
            }
        } else {
            requested_left += delta_requested_left / 20;
            requested_right += delta_requested_right / 20;

            reset = true;
        }

        float new_left  = step_pid(1, 0.0, 0.1, requested_left, current_left, &last_error_left, &integral_left);
        float new_right = step_pid(1, 0.0, 0.1, requested_right, current_right, &last_error_right, &integral_right);

        new_left  = clamp(new_left, -127, 127);
        new_right = clamp(new_right, -127, 127);

        if (abs(new_left) < 20) new_left = 0;
        if (abs(new_right) < 20) new_right = 0;

        writeDebugStreamLine("r: %.2f %.2f c: %.2f %.2f n: %.2f %.2f", requested_left, requested_right, current_left, current_right, new_left, new_right);

        motor[right_front] = -new_right;
        motor[right_middle] = -new_right;
        motor[right_back]  = -new_right;

        motor[left_front] = -new_left;
        motor[left_middle] = -new_left;
        motor[left_back]  = -new_left;
    }
}

void init() {
	stop_tasks_between_mode = true;
}

task auton() {}

task user_control() {
    //startTask(pid_loop);

	SensorValue[left_line_sensor] = 0;
    SensorValue[right_line_sensor] = 0;

    while (true) {

        int r = (vexRT[Ch1] + vexRT[Ch3]);
        int l = (vexRT[Ch1] - vexRT[Ch3]);

        if (abs(l) < 20) l = 0;
        if (abs(r) < 20) r = 0;

        motor[right_front] = r;
        motor[right_middle] = r;
        motor[right_back]  = r;

        motor[left_front] = l;
        motor[left_middle] = l;
        motor[left_back]  = l;

  		//clearLCDLine(0);
  		//clearLCDLine(1);

  		//displayLCDString(0, 0, " left: ");
  		//displayNextLCDNumber(SensorValue[left_line_sensor], 3);

  		//displayLCDString(1, 0, "right: ");
  		//displayNextLCDNumber(SensorValue[right_line_sensor], 3);

  		if(SensorValue[left_line_sensor] < 2700 && SensorValue[right_line_sensor] < 2700) {
  			bLCDBacklight = true;
  		} else {
  			bLCDBacklight = false;
  		}

  		clearLCDLine(0);
  		displayLCDString(0, 0, "s: ");
  		displayNextLCDNumber(SensorValue[sonar], 3);

    }

}
