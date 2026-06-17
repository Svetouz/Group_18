# Group_18 Pick and Place Robot

## User guide 
- main.cpp contains the final and most recent version of the code, intended to
be what's run the day of the demo. 
- Reach_and_grab.cpp contains the useful functions pertaining to the pick and place
operations, including entering the desired sectors. There is also the header file
Reach_and_grab.h within the "include" folder, which allows for the functions in
Reach_and_Grab.cpp to get resued without issue in main.cpp
- testServos, testStepper, and testServoAndStepper are all designed for testing 
with the arduino plugged in to a laptop. They have not been updated since the
repair for SIM2.

### main

**Calibration**
The calibration phase takes place almost immediately after the code is uploaded 
to the arduino, which is why (as well as because the code is not designed to 
reset after the track is complete) resetting the arduino with its inbuilt button
before powering on the robot and calibrating is a good practice.

**User Input**
Once calibration is finished, the servos will go to their default positions, and 
the robot will wait until the sectors are entered before proceeding. To enter the
sectors, first press the * character on the keypad. Then, enter the numbers of the
sectors in the order of:
starting sector of the object -> first target -> final target
To input a sector number less than 10, add a 0 in front of the significant digit
e.g. sector 9 input -> 09
After inputing a sector, simply move on to the next sector value. 
*needs description of how errors are handled*

**Line Following**
Once the sectors are entered, the robot immediately begins following the line.
*needs complete description*



