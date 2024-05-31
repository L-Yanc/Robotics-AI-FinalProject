// Channel A Pinout
int A_directionPin = 4;
int A_pwmPin = 5;

// Channel B Pinout
int B_directionPin = 7;
int B_pwmPin = 6;

// Sensor Pinout
int trigPin1 = 2;
int echoPin1 = 3;
int trigPin2 = 11;
int echoPin2 = 10;

// Sensor variables
long duration1;
float distance1;
long duration2;
float distance2;

// Robot states
bool direction;
bool changeDirection;
bool isFinished;

// Variables for storing initial distance reading and range
long distance2_ini;
long distance2_lb;
long distance2_ub;

// Counter variable for loop
int ctr;

void setup() {
  
    // Defining motor pins
    pinMode(A_directionPin, OUTPUT);
    pinMode(A_pwmPin, OUTPUT);

    pinMode(B_directionPin, OUTPUT);
    pinMode(B_pwmPin, OUTPUT);

    // Defining sensor pins
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);

    // Starting display
    Serial.begin(9600);

    // Setting initial direction & states
    changeDirection = false;
    direction = false;
    isFinished = false;

    // Starting counter
    ctr = 1;

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

    /* Decision: If robot is at the canopy */
    // Determining if the sensor has been deployed
    if(changeDirection==false && distance1>15 && distance1<1000) {

        // Changing the changeDirection variable to avoid changing directions at every iteration
        changeDirection = true;

        // Changing motor direction
        direction = true;
    }
    
    /* Decision: End Case */
    // Storing initial reading from sensor 2 to know when it has been achieved again
    if(ctr==1) {

        // Getting sensor reading
        distance2_ini = distance2;

        // Creating a range of 10 mm for the margin of error
        distance2_lb = distance2 - 5;
        distance2_ub = distance2 + 5;
    }

    // Stopping motors if the robot is within the initial range again after 50 iterations of the loop
    if(ctr>50 && distance2>distance2_lb && distance2<distance2_ub) {

        // changing the state of the robot
        isFinished == true;

    }

    /* Continuous running command */
    // Setting direction of motors
    if(direction == false) {

        digitalWrite(A_directionPin, LOW);
        digitalWrite(B_directionPin, LOW);

    } else {

        digitalWrite(A_directionPin, HIGH);
        digitalWrite(B_directionPin, HIGH);

    }

    // Running motors at 80% capacity when not in the end case, stopping otherwise
    if(isFinished == false) {

        analogWrite(A_pwmPin, 80);
        analogWrite(B_pwmPin, 80);

    } else {

        analogWrite(A_pwmPin, 0);
        analogWrite(B_pwmPin, 0);
        
    }

    ctr++;
    delay(2000);

}
