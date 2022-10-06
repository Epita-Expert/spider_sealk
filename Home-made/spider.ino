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
double new_position[4][3];
const int servo_pin[4][3] = {
    {4, 2, 3},    // Leg 1 - Front right (R2) - {H, F, T}
    {7, 5, 6},    // Leg 2 - Back right (R1) - {H, F, T}
    {16, 14, 15}, // Leg 3 - Front left (L1) - {H, F, T}
    {19, 17, 18}  // Leg 4 - Back left (L2) - {H, F, T}
};

const double pls_position[4][3] = {
    {90, 90, 160}, // Leg 1 - Front right (R2) - {H, F, T}
    {90, 90, 20}, // Leg 2 - Back right (R1) - {H, F, T}
    {90, 90, 20}, // Leg 3 - Front left (L1) - {H, F, T}
    {90, 90, 160}  // Leg 4 - Back left (L2) - {H, F, T}
};

float getHanche(float x, float y) {
    return (atan(y/x)*4068)/71;
}

float getFemur(float x, float y, float z) {
    return ((atan(z/(sqrt(sq(x)+sq(y)) - 30.5))*4068)/71) + (acos((sq(x)+sq(y) + sq(z) - 61*sqrt(sq(x)+sq(y)) - 2581) / (106 * sqrt(sq(x) + sq(y) + sq(z)-61*(sqrt(sq(x)+sq(y))) +930.25)))*4068)/71;
}

float getTibia(float x, float y, float z) {
    
    return (acos((8199 - sq(z) -sq(x)-sq(y)+61*sqrt(sq(x) + sq(y))) / 8427)*4068)/71;
}

bool r2(int servo) {
    return 4 == servo || 2 == servo || 3 == servo; // Leg 1 - Front right (R2) - {H, F, T}
}

bool r1(int servo) {
    return 7 == servo || 5 == servo || 6 == servo; // Leg 2 - Back right (R1) - {H, F, T}
}

bool l1(int servo) {
    return 16 == servo || 14 == servo || 15 == servo; // Leg 3 - Front left (L1) - {H, F, T}
}

bool l2(int servo) {
    return 19 == servo || 17 == servo || 18 == servo; // Leg 4 - Back left (L2) - {H, F, T}
}

bool checkConsigne(int servo, int position) {
    if (r2(servo) || l2(servo)) {
        if (servo == 4 || servo == 19) { // HANCHE 
            if (position >= 60 && position <= 175) {
                return true;
            }
        } else if (servo == 2 || servo == 17) { // FEMUR
            if (position >= 5 && position <= 120) {
                return true;
            }
        } else if (servo == 3 || servo == 18) { // TIBIA
            if (position >= 40 && position <= 160) {
                return true;
            }
        }
    } else if (r1(servo) || l1(servo)) {
        if (servo == 7 || servo == 16 ) { // HANCHE
            if (position >= 5 && position <= 120) {
                return true;
            }
        } else if (servo == 5 || servo == 14) { // FEMUR
            if (position >= 60 && position <= 175) {
                return true;
            }
        } else if (servo == 6 || servo == 15) { // TIBIA
            if (position >= 20 && position <= 140) {
                return true;
            }
        }
    }
    return false;
}

void activeTibia() {
    for (int i = 0; i <= 3; i += 1) {
        servo[i][2].attach(servo_pin[i][2]);
        servo[i][2].write(pls_position[i][2]);
    }
}

void activeOthersLimbs() {
    for (int i = 0; i <= 3; i += 1) {
        for (int j = 0; j <= 1; j += 1) {
            servo[i][j].attach(servo_pin[i][j]);
            servo[i][j].write(pls_position[i][j]);
        }
    }
}

void plsMode() {
    activeTibia();
    delay(200);
    activeOthersLimbs();
    delay(200);
    exit(0);
}

/* Set the angles position of the servos */
void set_pos(const double angles[4][3]) {
  for (int i = 0; i <= 3; i += 1) {
    for (int j = 0; j <= 2; j += 1) {
      servo[i][j].attach(servo_pin[i][j]);
      servo[i][j].write(angles[i][j]);
    }
  }
}

void check_consigne(const double angles[4][3]) {
  for (int i = 0; i <= 3; i += 1) {
    for (int j = 0; j <= 2; j += 1) {
      bool consigneHancheL2 = checkConsigne(servo_pin[i][j], angles[i][j]);
      if (!consigneHancheL2) {
        Serial.println("PLS MODE ATIVATED");
        Serial.print("Servo: ");
        Serial.print(servo_pin[i][j]);
        Serial.print(" - ");
        Serial.print("Position: ");
        Serial.println(angles[i][j]);
        plsMode();
      }
      Serial.print("OK: ");
      Serial.println(angles[i][j]);
    }
  }
}

void move_all_legs_in_same_direction(double x, double y, double z) {
  float hancheDeg = getHanche(x, y);
  float femurDeg = getFemur(x, y, z);
  float tibiaDeg = getTibia(x, y, z);
  
  // L1 & R1
  float hancheDegR1L1 = abs(hancheDeg - 90);
  float femurDegR1L1 = abs(femurDeg + 90);
  float tibiaDegR1L1 = abs(tibiaDeg - 180);

  // L2 & R2
  float hancheDegR2L2 = abs(hancheDeg + 90);
  float femurDegR2L2 = abs(femurDeg - 90);
  float tibiaDegR2L2 = abs(tibiaDeg);

  double new_position1[4][3] = {
    {hancheDegR2L2, femurDegR2L2, tibiaDegR2L2}, // Leg 1 - Front right (R2) - {H, F, T}
    {hancheDegR1L1 - 9.53, femurDegR1L1 + 1.87, tibiaDegR1L1 - 8.13},// Leg 2 - Back right (R1) - {H, F, T}
    {hancheDegR1L1, femurDegR1L1, tibiaDegR1L1}, // Leg 3 - Front left (L1) - {H, F, T}
    {hancheDegR2L2, femurDegR2L2, tibiaDegR2L2}  // Leg 4 - Back left (L2) - {H, F, T}
  };
  
  for (int i = 0; i <= 3; i += 1) {
    for (int j = 0; j <= 2; j += 1) {
      new_position[i][j] = new_position1[i][j];
    }
  }
}

void check_consigne_and_set_pos() {
  check_consigne(new_position);
  set_pos(new_position);
}

void make_triangle() {
  // triangle first position
  move_all_legs_in_same_direction(90, 10, -27);
  check_consigne_and_set_pos();
  delay(1000);
  // triangle second position
  move_all_legs_in_same_direction(90, 80, -27);
  check_consigne_and_set_pos();
  delay(1000);
  // triangle third position
  move_all_legs_in_same_direction(140, 10, -27);
  check_consigne_and_set_pos();
}

void sit_down() {
  move_all_legs_in_same_direction(70, 50, -27);
  check_consigne_and_set_pos();
}

void stand_up() {
  move_all_legs_in_same_direction(70, 50, -40);
  check_consigne_and_set_pos();
}

void walk_R2() {
  move_one_leg(70, 100, -20, "R2");
  check_consigne_and_set_pos();
  move_one_leg(70, 100, -40, "R2");
  check_consigne_and_set_pos();
}

void walk_L1() {
  move_one_leg(70, 50, -20, "L1");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "L1");
  check_consigne_and_set_pos();
}

void walk_R1() {
  move_one_leg(70, 50, -20, "R1");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "R1");
  check_consigne_and_set_pos();
}

void walk_L2() {
  move_one_leg(70, 50, -20, "L2");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "L2");
  check_consigne_and_set_pos();
}

void turn_left_L1() {
  move_one_leg(73, 26, -20, "L1");
  check_consigne_and_set_pos();
  move_one_leg(70, 26, -40, "L1");
  check_consigne_and_set_pos();
}

void turn_left_L2() {
  move_one_leg(33, 71, -20, "L2");
  check_consigne_and_set_pos();
  move_one_leg(33, 71, -40, "L2");
  check_consigne_and_set_pos();
}

void turn_left_R1() {
  move_one_leg(75, 26, -20, "R1");
  check_consigne_and_set_pos();
  move_one_leg(75, 26, -40, "R1");
  check_consigne_and_set_pos();
}

void turn_left_R2() {
  move_one_leg(33, 72, -20, "R2");
  check_consigne_and_set_pos();
  move_one_leg(33, 72, -40, "R2");
  check_consigne_and_set_pos();
}

void move_one_leg(double x, double y, double z, String leg) {
    float hancheDeg = getHanche(x, y);
    float femurDeg = getFemur(x, y, z);
    float tibiaDeg = getTibia(x, y, z);

    float hancheDegCustom;
    float femurDegCustom;
    float tibiaDegCustom;
    
    if (leg == "R1" || leg == "L1") {
      hancheDegCustom = abs(hancheDeg - 90);
      femurDegCustom = abs(femurDeg + 90);
      tibiaDegCustom = abs(tibiaDeg - 180);
    } else {
       hancheDegCustom = abs(hancheDeg + 90);
       femurDegCustom = abs(femurDeg - 90);
       tibiaDegCustom = abs(tibiaDeg);
    }

    const double leg_position[3] = {hancheDegCustom, femurDegCustom, tibiaDegCustom};
    
    for (int i = 0; i <= 2; i += 1) {
        if (leg == "R1") {
            new_position[1][i] = leg_position[i];
        } else if (leg == "L1") {
            new_position[2][i] = leg_position[i];
        } else if (leg == "R2") {
            new_position[0][i] = leg_position[i];
        } else if (leg == "L2") {
            new_position[3][i] = leg_position[i];
        }
    }
}

void turn_all_legs_to_left_position() {
  // L1
  move_one_leg(33, 70, -40, "L1");
  // R2
  move_one_leg(73, 25, -40, "R2");
  // L2
  move_one_leg(73, 25, -40, "L2");
  // R1
  move_one_leg(33, 71, -40, "R1");
  check_consigne_and_set_pos();
}

void put_in_initial_position() {
  // L1
  move_one_leg(70, 50, -20, "L1");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "L1");
  check_consigne_and_set_pos();

  // L2
  move_one_leg(70, 50, -20, "L2");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "L2");
  check_consigne_and_set_pos();
  
  // R1
  move_one_leg(70, 50, -20, "R1");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -20, "R1");
  check_consigne_and_set_pos();

  // L2
  move_one_leg(70, 50, -20, "L2");
  check_consigne_and_set_pos();
  move_one_leg(70, 50, -40, "L2");
  check_consigne_and_set_pos();
}

void init_position_forward() {
  // L1
  move_one_leg(70, 0, -40, "L1");
  // R2
  move_one_leg(70, 50, -40, "R2");
  // L2
  move_one_leg(70, 100, -40, "L2");
  // R1
  move_one_leg(70, 100, -40, "R1");
  check_consigne_and_set_pos();
}

void go_forward() {
  for (int i = 0; i <= 4; i += 1) {
    if (i == 0) {
      walk_R1();
    } else if (i == 1) {
      walk_L2();
    } else if (i == 2) {
      walk_L1();
    } else if (i == 3) {
      walk_R2();
    } else if (i == 4) {
      init_position_forward();
    }
  }
}

void go_turn_left() {
  turn_left_L1();
  turn_left_L2();
  turn_left_R1();
  turn_left_R2();
  turn_all_legs_to_left_position();
}

void initial_position() {
  move_all_legs_in_same_direction(100, 70, 15);
  check_consigne(new_position);
  set_pos(new_position);
}

void setup() {  
  Serial.begin(9600);
  // initial position
  initial_position();
  //make_triangle();
  stand_up();
  // delay(1000);
  //Serial.println("Stand up done");
  // walk_R2();
  // init_position_forward();
  // go_forward();
  // go_turn_left();
  Serial.println("DONE !");
}

void loop() {
  // put your main code here, to run repeatedly:
  go_turn_left();
}