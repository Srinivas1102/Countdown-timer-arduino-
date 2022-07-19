// Game Countdown Timer
// After turning on, use the UP and DOWN buttons to
//    set the time in minutes and seconds for the time allowed,
//    hold button for repeated time changes
// The set time will remain showing on the display
// Press start a short tone will sound, "go" will be displayed 
//    and the time will start counting down to 0
// When 0 is reached a longer tone will sound and "End" displayed 
//    for 5 seconds before resetting to the allocated time 
//    for the next players go.

// Include the library driver for display:
#include <TM1637Display.h>

// Define the connections pins for display
#define CLK 7
#define DIO 6

// Define other pin connections
#define UP_BUTTON 4
#define DOWN_BUTTON 2
#define START_BUTTON 3
#define BUZZER 9

int duration;      // Duration in seconds

// Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);

// Set the individual segments for the word displays:
const uint8_t seg_end[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,         // E
  SEG_C | SEG_E | SEG_G,                         // n 
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  0x00                                           // All off
};

const uint8_t seg_go[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,// g
  SEG_C | SEG_D | SEG_E | SEG_G,                  // o
  0x00,                                           // All off
  0x00                                           // All off
};

void setup() {
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  duration = 10;              // Default to 30 seconds
  display.setBrightness(3);   // 0 to 7 change if required
  ShowTime(duration);
}

void loop() {
  // Function will checks for time change buttons and only return 
  // when start button pressed
  WaitForStart();
  // Start the duration timer
  TimeDuration();
}

void WaitForStart(){
  // Check for button presses every 0.15 seconds
  while (digitalRead(START_BUTTON) == HIGH){
    // Check if up or down has been pressed
    //if time > 60 then increment by 10 seconds
    if (digitalRead(UP_BUTTON) == LOW){
      if (duration < 60){
        duration++;
       }
       else{
        duration += 10;
       }
      ShowTime(duration);
    }
    if (digitalRead(DOWN_BUTTON) == LOW){
      if (duration > 60){
        duration -= 10;
       }
       else{
        duration--;
       }
      ShowTime(duration);
    }
    delay(150);
  }
  // Start button has been pressed
  tone(BUZZER, 2000, 200);
  display.clear();
  display.setSegments(seg_go);
}

void TimeDuration(){
  // While loop will continue until time up
  unsigned long startTime = millis();
  unsigned long timer = 1000ul * duration;
  // Repeatedly check if time is up
  while ((millis() - startTime) <= timer){
    // Calculate time elapsed in seconds
    int elapsed = int((millis() - startTime)/1000);
    // Only start to display countdown after 3 seconds
    if ((millis() - startTime) > 10){
      ShowTime(duration - elapsed);
    }
  }
  // Time up
  tone(BUZZER, 3000, 240000);
  display.clear();
  display.setSegments(seg_end);
  // Wait 5 seconds and reset display
  delay(1000);
  // Show duration for next player
  ShowTime(duration);
}

void ShowTime(int value){
  static int lastTime;
  // Update the display if time has changed
  if (lastTime != value) {
    lastTime = value;
    int iMinutes = value / 60;
    int iSeconds = value - (iMinutes * 60);
    // Show on 4 digit display
    uint16_t number = iMinutes * 100 + iSeconds;
    display.showNumberDecEx(number, 0b01000000, true, 4, 0);    
  }
}
