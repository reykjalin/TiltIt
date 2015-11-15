/**** Controller button pins ****/
int L2 = 2, L1 = 3;
int R2 = 4, R1 = 6;
int leftbutton = A4, rightbutton = A5;
int ly = A0, lx = A1;
int rx = A2, ry = A3;
/**** Controller button pins ****/

/**** Variables for button states ****/
int buttonStateL1, buttonStateL2;
int buttonStateR1, buttonStateR2;
int buttonStateLsw, buttonStateRsw;
int buttonStately, buttonStatelx;
int buttonStatery, buttonStaterx;          
int lastButtonStateLsw = LOW, lastButtonStateRsw = LOW;
/**** Variables for button states ****/

/**** Variables for debounce times ****/
long lastDebounceTimeL1 = 0, lastDebounceTimeL2 = 0;
long lastDebounceTimeR1 = 0, lastDebounceTimeR2 = 0;
long lastDebounceTimeLsw = 0, lastDebounceTimeRsw = 0;
long lastDebounceTimely = 0, lastDebounceTimelx = 0;
long lastDebounceTimery = 0, lastDebounceTimerx = 0;
long lastDebounceTimePrint = 0;
/**** Variables for debounce times ****/

/**** Debounce durations ****/
long incrDebounceDelay = 1;
long sendDebounceDelay = 50000;
long killDeb = 200;
/**** Debounce durations ****/

/**** Strings for G-code commands ****/
String gstring, lastgstring, killCommand = "M18;";
/**** Strings for G-code commands ****/

/**** Variables for coordinates ****/
double X = 0, Y = 0, Z = 0;
double U = 0, V = 0;
/**** Variables for coordinates ****/

/**** Number of messages sent this loop ****/
int msgSent = 0;
/**** Number of messages sent this loop ****/

void setup() {
	Serial.begin(57600);
	pinMode(L2,INPUT);
	pinMode(L1,INPUT);
	pinMode(R2,INPUT);
	pinMode(leftbutton,INPUT);
	pinMode(R1,INPUT);
	pinMode(rightbutton,INPUT);
	pinMode(ly,INPUT);
	pinMode(lx,INPUT);
	pinMode(rx,INPUT);
	pinMode(ry,INPUT);
}

void loop() {
    /**** Read button states ****/
    int buttonStateL1 = digitalRead(L1);
    int buttonStateL2 = digitalRead(L2);
    int buttonStateR1 = digitalRead(R1);
    int buttonStateR2 = digitalRead(R2); 
    int buttonStateLsw = analogRead(leftbutton);
    int buttonStateRsw = analogRead(rightbutton);
    int buttonStately = analogRead(ly);
    int buttonStatelx = analogRead(lx);
    int buttonStatery = analogRead(ry);
    int buttonStaterx = analogRead(rx);
	/**** Read button states ****/

    /**** First part of command string ****/
    gstring = "G01";
    /**** First part of command string ****/

    /**** Change 'X' every debounce interval ****/
    if((micros() - lastDebounceTimerx) > incrDebounceDelay){
        if(buttonStaterx < 450){
            if(X < 5){
                X+=0.015;
                gstring += " X" + String(X);
            }
        }
        else if(buttonStaterx > 550){
            if(X > -5){
                X-=0.015;
                gstring += " X" + String(X);
            } 
        }
        lastDebounceTimerx = micros();
    }
    /**** Change 'X' every debounce interval ****/

    /**** Change 'Y' every debounce interval ****/
    if((micros() - lastDebounceTimery) > incrDebounceDelay){
        if(buttonStatery > 550){
            if(Y < 5){
                Y+=0.015;
                gstring += " Y" + String(Y);  
            }  
        } 
        else if(buttonStatery < 450){
            if(Y > -5){
                Y-=0.015;
                gstring += " Y" + String(Y);
            }
        } 
        lastDebounceTimery = micros();
    }
    /**** Change 'Y' every debounce interval ****/

    /**** Change 'Z' every debounce interval ****/
    if((micros() - lastDebounceTimeL1) > incrDebounceDelay){ 
        if(buttonStateL1 == HIGH){
            if(Z > -3.6){
                Z-=0.01;
                gstring += " Z" + String(Z);
            }        
        } 
        lastDebounceTimeL1 = micros();
    }
    if((micros() - lastDebounceTimeR1) > incrDebounceDelay){ 
        if(buttonStateR1 == HIGH){
            if(Z < 6.26){
                Z+=0.01;
                gstring += " Z" + String(Z);
            }
        } 
        lastDebounceTimeR1 = micros();
    }
    /**** Change 'Z' every debounce interval ****/

    /**** Change 'U' every debounce interval ****/
    if((micros() - lastDebounceTimely) > incrDebounceDelay){
        if(buttonStately > 550){
            if(U > -20){
                U-=0.02;
                gstring += " U" + String(U);
            } 
        } 
        else if(buttonStately < 450){
            if(U < 20){
                U+=0.02;
                gstring += " U" + String(U);
            }  
        } 
        lastDebounceTimely = micros();
    }
    /**** Change 'U' every debounce interval ****/

    /**** Change 'V' every debounce interval ****/
    if((micros() - lastDebounceTimelx) > incrDebounceDelay){
        if(buttonStatelx < 450){
            if(V < 20){
                V+=0.02;
                gstring += " V" + String(V);
            } 
        } 
        else if(buttonStatelx > 550){
            if(V > -20){
                V-=0.02;
                gstring += " V" + String(V);
            }
        } 
        lastDebounceTimelx = micros();
    }
    /**** Change 'V' every debounce interval ****/

    /**** If 'L2' is pressed, go to home position ****/
    if((millis() - lastDebounceTimeL2) > killDeb){ 
        if(buttonStateL2 == HIGH){
            Serial.println("G28;");
            X = 0; Y = 0; Z = 0;
            U = 0; V = 0;
            get_string();
            msgSent++;
        }
        lastDebounceTimeL2 = millis();
    }
    /**** If 'L2' is pressed, go to home position ****/

    /**** If 'R2' is pressed, turn motors off ****/
    if((micros() - lastDebounceTimeR2) > killDeb){ 
        if(buttonStateR2 == HIGH){
            Serial.println(killCommand);
        } 
        lastDebounceTimeR2 = micros();
    }
    /**** If 'R2' is pressed, turn motors off ****/

    /**** Shutting motors off was planned for right switch, switch broken ****/
    if((millis() - lastDebounceTimeRsw) > killDeb){
        if(lastButtonStateRsw != buttonStateRsw && buttonStateRsw == LOW && msgSent == 0){
            Serial.println("M18;");
            msgSent++;
            lastButtonStateRsw = buttonStateRsw;
        }
        lastDebounceTimeRsw = millis();
    }
    /**** Shutting motors off was planned for right switch, switch broken ****/

    /**** Home position was planned for left switch, switch broken ****/
    if((micros() - lastDebounceTimeLsw) > incrDebounceDelay){
        if(lastButtonStateLsw != buttonStateLsw && buttonStateLsw == LOW && msgSent == 0) {
            Serial.println("G28;");
            lastButtonStateLsw = buttonStateLsw;
            get_string();
            msgSent++;
        }
        lastDebounceTimeLsw = micros();
    }
    /**** Home position was planned for left switch, switch broken ****/
    
    /**** End of command string ****/
    gstring += ";";
    /**** End of command string ****/

    /**** Send command through serial communications after change every
    		debounce interval ****/
    if(lastgstring != gstring && micros() - lastDebounceTimePrint > sendDebounceDelay && msgSent == 0) {
        Serial.println(gstring);
        lastDebounceTimePrint = micros();
        lastgstring = gstring;
    }
    /**** Send command through serial communications after change every
    		debounce interval ****/

    /**** Re-initialize number of messages sent ****/
    msgSent = 0;
    /**** Re-initialize number of messages sent ****/
}

/**
 * Create string from new values (only used for home position)
 */
void get_string(){
    gstring = "G01 X" + String(X) + " Y" + String(Y) + " Z" + String(Z) + " U" + String(U) + " V" + String(V) + ";";
}
