/* Sweep tamere
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo servo[4][3]; // 4 legs of 3 segments each
/* Array of pins connected to the servo */
const int servo_pin[4][3] = {
    {4, 2, 3},    // Leg 1 - Front right (R2) - {H, F, T}
    {7, 5, 6},    // Leg 2 - Back right (R1) - {H, F, T}
    {16, 14, 15}, // Leg 3 - Front left (L1) - {H, F, T}
    {19, 17, 18}  // Leg 4 - Back left (L2) - {H, F, T}
};

const double init_angles[4][3] = {
    {124.99, 21.92, 86.46}, // Leg 1 - Front right (R2) - {H, F, T}
    {55.01, 158.08, 93.54}, // Leg 2 - Back right (R1) - {H, F, T}
    {55.01, 158.08, 93.54}, // Leg 3 - Front left (L1) - {H, F, T}
    {124.99, 21.92, 86.46}  // Leg 4 - Back left (L2) - {H, F, T}
};

const double stand_angles[4][3] = {
    {124.99, 80, 86.46}, // Leg 1 - Front right (R2) - {H, F, T}
    {55.01, 100, 93.54}, // Leg 2 - Back right (R1) - {H, F, T}
    {55.01, 100, 93.54}, // Leg 3 - Front left (L1) - {H, F, T}
    {124.99, 80, 86.46}  // Leg 4 - Back left (L2) - {H, F, T}
};

const int servo_fl_t = servo_pin[2][2];
const int servo_fl_f = servo_pin[2][3];

/* Set the angles position of the servos */
void set_pos(const double angles[4][3])
{
    for (int i = 0; i <= 3; i += 1)
    {
        for (int j = 0; j <= 2; j += 1)
        {
            servo[i][j].attach(servo_pin[i][j]);
            servo[i][j].write(angles[i][j]);
        }
    }
}

/* Set the position to down */
void reset_pos()
{
    delay(1000);
    set_pos(init_angles);
}

/* Set the position to stand */
void stand_pos()
{
    delay(1000);
    set_pos(stand_angles);
}

void attack_left()
{
    delay(1000);

    servo[2][2].write(20);  // servo_fl_t
    servo[2][1].write(100); // servo_fl_f
}

void setup()
{
    reset_pos();
    stand_pos();
}

void loop()
{
    attack_left();
    stand_pos();
}
