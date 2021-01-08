// This #include statement was automatically added by the Particle IDE.
#include <blynk.h>
char auth[] = "_1p7LfGHnLvFOo9HmXhl9POWo_iVmJMt";
BlynkTimer timer;
WidgetTerminal terminal(V10);
#define doorClose D4 //I would change the names of these to something like doorCloseRelay
#define doorOpen  D7 //set to use blue LED on Argon
bool doorState;
bool doorMoving;//if true or 1 door is moving

int openDoorBlynk;
int closeDoorBlynk;

BLYNK_WRITE(V1){//a button set as push in blynk
    openDoorBlynk=param.asInt();
    testLoop();//does this run without variables passed into it
    if(openDoorBlynk ==1) {
        openDoor();
    }
    //moveDoor(0,1);
}
BLYNK_WRITE(V2){
    closeDoorBlynk=param.asInt();
    //moveDoor(0,0);
}


void setup() {
    Time.zone(-6);
    Blynk.begin(auth);
    pinMode(doorClose, OUTPUT);
    pinMode(doorOpen, OUTPUT);

}

void loop() {
    Blynk.run();
    timer.run();
    /*
    moveDoor(0, 0);  //replace every place in your code that says closeDoor() with this
    moveDoor(0, 1);  //replace every place in your code that says openDoor() with this
    moveDoor(5000L, 0);  //this will close the door 5 secounds after being called
    */
}

void testLoop() {//using this to test out blynk button
    terminal.print(Time.format("%D %r - "));
    terminal.println("testLoop");
    terminal.flush();
}

void moveDoor(int whenToStart, bool openIt) {//whenToStart sets the delay time before door begins to open or close -- openIt?? open door=1 close door=0
  if(whenToStart == 0  && doorMoving == 0){//no delay>so beging moving door right now if its not moving right now
        if(openIt){//
            openDoor();
        }
        else{
            closeDoor();
        }
    }
    else if(!doorMoving && openIt){//make sure door still isn't moving and if we should open the door
        int delayDoorMove = timer.setTimeout(whenToStart, [] () {//assign the delay from moveDoor function
            terminal.print(Time.format("%D %r - "));
            //terminal.println(whenToStart);
            terminal.flush();
            openDoor();
        });
    }
    else if(!doorMoving && !openIt){//make sure door still isn't moving and if we should close the door
        int delayDoorMover = timer.setTimeout(whenToStart, [] () {
            closeDoor();
        });
    }
    else{
        terminal.print(Time.format("%D %r - "));
        terminal.println("moveDoor function did not run");
        terminal.flush();
    }
}
  


void openDoor() {
    doorMoving = 1;//door will begin moving in next line of code
    digitalWrite(doorOpen,LOW); //door is opening
    terminal.print(Time.format("%D %r - "));
    terminal.println("In openDoor function");
    int stopMovement = timer.setTimeout(2000L, [] () { //give to door time to open completely
        terminal.println("turning off openDoor");
        terminal.flush();
        digitalWrite(doorOpen, HIGH); //once all the way open, shut off the relay
        doorMoving = 0; //door is no longer moving
        doorState = 1;  //door is now open
    });
}

void closeDoor() {
    doorMoving = 1;//door will be moving in next line of code
    digitalWrite(doorClose,LOW); //door is closing
    int stopMovement = timer.setTimeout(2000L, [] () {
       digitalWrite(doorClose, HIGH);
       doorMoving = 0; //door is no longer moving
       doorState = 0;  //door is now closed
    });
}
