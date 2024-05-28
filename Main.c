// Channel A Pinout
int A_directionPin = 1;
int A_pwmPin = 2;
int A_brakePin = 3;

// Channel B Pinout
int B_directionPin = 4;
int B_pwmPin = 5;
int B_brakePin = 6;

// Sensor Pinout
int trigPin1 = 7;
int echoPin1 = 8;
int trigPin2 = 9;
int echoPin2 = 10;

// Sensor variables
long duration1;
int distance1;
long duration2;
int distance2;

// boolean to switch direction
bool directionState;

// boolean to know if the robot's askew
bool isAskew;

void setup() {
  
    // define motor pins
    pinMode(A_directionPin, OUTPUT);
    pinMode(A_pwmPin, OUTPUT);
    pinMode(A_brakePin, OUTPUT);

    pinMode(B_directionPin, OUTPUT);
    pinMode(B_pwmPin, OUTPUT);
    pinMode(B_brakePin, OUTPUT);

    // define sensor pins
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);

    // Start display
    Serial.begin(9600);

}

void loop() {

    // Taking in sensor measurements
    digitalWrite(trigPin1, LOW);
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin1, HIGH);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin1, LOW);
    digitalWrite(trigPin2, LOW);

    duration1 = pulseIn(echoPin1, HIGH);
    duration2 = pulseIn(echoPin2, HIGH);

    // Converting sensor input to mm
    distance1 = duration1 * 0.034 / 2;
    distance2 = duration2 * 0.034 / 2;

    // Displaying sensor input
    Serial.print("Distance 1: ");
    Serial.print(distance1);

    Serial.print("            Distance 2: ");
    Serial.println(distance2);

    /* TODO: some arbitrary way to determine direction */
    /*
        if(changeDirection == true) {
            A_directionState = !A_directionState;
            B_directionState = !B_directionState;
        }
    */

    // Setting direction of motors
    if(directionState == false) {
        digitalWrite(A_directionPin, LOW);
        digitalWrite(B_directionPin, LOW);
    } else {
        digitalWrite(A_directionPin, HIGH);
        digitalWrite(B_directionPin, HIGH);
    }

    // Determining course of action based on sensor reading
    isAskew = isAskew(distance1, distance2);
    
    // Normal case when all motors move the same way
    if (isAskew == false) {

        //release breaks
        digitalWrite(A_brakePin, LOW);
        digitalWrite(B_brakePin, LOW);

        //set work duty for the motor
        analogWrite(A_pwmPin, 40);
        analogWrite(B_pwmPin, 40);

    // Askew case if one of the motors are stuck
    } else {

        // Determine which side is stuck
        if(distance1 > distance2) {

            digitalWrite(A_brakePin, LOW); // Releasing the stuck side's breaks
            digitalWrite(B_brakePin, HIGH); // Engaging the free side's breaks

            analogWrite(A_pwmPin, 60); // Setting the stuck motor to a strong output
            analogWrite(B_pwmPin, 10); // Setting the free motor to a weak output

        } else {
            digitalWrite(A_brakePin, HIGH); // Engaging the free side's breaks
            digitalWrite(B_brakePin, LOW); // Releasing the stuck side's breaks

            analogWrite(A_pwmPin, 10); // Setting the free motor to a weak output
            analogWrite(B_pwmPin, 60); // Setting the stuck motor to a strong output
        }

    }
    
    /* TODO: Some arbitrary way to determine when the robot's done */
    /*
        if(done == true) {
            digitalWrite(A_brakePin, HIGH);
            digitalWrite(B_brakePin, HIGH);
            
            analogWrite(A_pwmPin, 0);
            analogWrite(B_pwmPin, 0);
        }
    */

    delay(2000);
}

// Function to determine if the robot's askew (one of the motors is stuck)
// based on sensor input
bool isAskew(distance1, distance2) {

    int diff = abs(distance1-distance2);

    // TODO: Fix arbitrary threshold
    if(diff > 10) {
        return true;
    }
    return false;

}