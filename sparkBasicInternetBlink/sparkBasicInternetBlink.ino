int led=D7;
int led2=D6;

void setup() {
  pinMode(led, OUTPUT); // set the D7 LED as output
  pinMode(led2, OUTPUT); // set the D6 LED as output
  Spark.function("control",control);    // a POST request for "blink" will reference blinkfunc, defined below
}

// call the below function when the POST request matches it
int control(String command) {
    if (command == "1") {
        digitalWrite(led, HIGH);    
    }
    else if (command == "2") {
           digitalWrite(led, LOW);    
    }
     else if (command == "3") {
           digitalWrite(led2, HIGH);    
    }
     else if (command == "4") {
           digitalWrite(led2, LOW);    
    }
}

void loop() {
//not doing anything here 
}
