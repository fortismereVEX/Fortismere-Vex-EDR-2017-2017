#pragma once

#include "main.hpp"

#include "lcd.hpp"

//#define DRIVE_DEBUG

#include "demo.hpp"

// TODO: recorder + replay should be the same class

class drive {
    // are we replaying joystick / button commmands
    static bool in_demo;
    static bool in_recording;

    static recorder *rec;
    static replay *  rep;

    static int last_time;

    static bool pid_enabled;

    static pid_helper<ime> pid_drive_left;
    static pid_helper<ime> pid_drive_right;

    static char  power_left;
    static char  power_right;
    static float requested_left;
    static float requested_right;

    static int get_delta_time() {
        int current_time = millis();
        int dt           = current_time - last_time;
        last_time        = current_time;
        return dt;
    }

public:
    static void initialize() {
        auto inited = imeInitializeAll();
        lcd::printf("%d imes", inited);
    }

    static int get_joystick_analog(int axis) {
        if (in_demo == true) {
            // return next byte
            return 0;
        }

        char value = joystickGetAnalog(1, axis);

        // account for dead space
        // TODO: we shouldnt need to deal with this
        if (abs(value) < 20) value = 0;

        if (in_recording == true) {
            // write to buffer
        }

        return value;
    }
    static bool get_joystick_digital(int group, int direction) {
        if (in_demo == true) {
            // return next byte
            return 0;
        } else if (in_recording == true) {
            char value = joystickGetDigital(1, group, direction);
            // store value in buffer
            return value;
        }
        return joystickGetDigital(1, group, direction);
    }

    static void run_drive() {
        int fwd = get_joystick_analog(1);
        int lr  = get_joystick_analog(3);

        power_right = (fwd - lr);
        power_left  = (fwd + lr);
    }

    static void run_intake() {
        // TODO: split into setting values here and
        // applying them in finalise to be consistent with other code
        if (get_joystick_digital(7, JOY_UP)) {
            motorSet(6, 127);
            motorSet(7, -127);
        } else if (get_joystick_digital(7, JOY_DOWN)) {
            motorSet(6, -127);
            motorSet(7, 127);
        } else {
            motorSet(6, 0);
            motorSet(7, 0);
        }
    }

    static void run_lift() {
    }

    static void pid_frame() {
        if (get_joystick_digital(8, JOY_UP)) {
            pid_enabled = true;
        } else if (get_joystick_digital(8, JOY_DOWN)) {
            pid_enabled = false;
        }

        if (pid_enabled) {
            int dt = get_delta_time();

            pid_drive_left.set_dt(dt);
            pid_drive_left.set_requested(power_left);

            pid_drive_right.set_dt(dt);
            pid_drive_right.set_requested(power_right);

            pid_drive_left.step();
            pid_drive_right.step();

            power_left  = pid_drive_left.get_new_power();
            power_right = pid_drive_right.get_new_power();
        }
    }

    static void finalise() {
#if defined(DRIVE_DEBUG)
        lcdPrint(uart1, 1, "l: %d r: %d", power_left, power_right);
//printf("w: %.1f a: %.1f\n", right_rpm, right_real_rpm);
#endif

        // front left
        motorSet(2, power_left);
        // back left
        motorSet(3, power_left);

        motorSet(8, power_left);

        // front right
        motorSet(4, power_right);
        // back right
        motorSet(5, power_right);

        motorSet(9, power_right);
    }

    static void run_frame() {
        // build motor values
        run_drive();
        run_intake();
        run_lift();

        // run pid where needed
        pid_frame();

        // set final motor values
        finalise();
    }
};
