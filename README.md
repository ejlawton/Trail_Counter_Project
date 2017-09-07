# Progress Report

## Milestone 1:

#### Requirements: 
  1. [✓] Trigger the counter based on a fixed accelerometer threshold
  2. [✓] Time stamp the number of counts each minute and store in processor RAM
  3. [✓] Create a ChibiOS command, display_counts that will print the time-stamps and counts

#### Files added/modified & used: 
  1. main.c 
  2. LSM303AGR_ACC_driver.c/.h
  3. SensorTile.c/.h
  4. Makefile
  
  A majority of the changes for this milestone occured in SensorTile.c, where we approached an implementation where we unwrapped IO_SPI_WRITE/READ from the IO_WRITE/READ functions, and converted them to be compatible with ChibiOS. 

  Further configuration had to occur in various low-level driver files where we had to enable 3-wire mode, and other various variables that were necessary for the class-chosen SPI communication format. 

  We had to add the necessary files for this implementation (SensorTile / Acc driver) to the Makefile dependencies, as well as created a local directory (drivers), where we are compiling every necessary driver to one location. This also had to be added onto the directory dependencies. This was done under CSRC and INCDIR respectively. 
  
  Finally, the acc/mag peripheral had to be configured so that we were getting accurate data that could be parsed quickly. With that, in addition to regular intialization configuration provided by LSM303AGR_ACC_Init(), we enabled high-precision mode and set the data retrieval rate to 100Hz. We activated several threads which had differing responsibilities: reading data from accelerometer, triggering count conditionals / suspending thread activity when above thresholds, waking up every 60 seconds to record count and store in array located in processor RAM. To elaborate a bit more on the threshold for the accelerometer, I went through one axis at a time and logged the recordings for around 60 seconds with intermittent "bangs" on the table to get a good deviation in the readings. After that, I took the values (from a logged screen) and put them into a scatter plot to find an average threshold we could use. After repeating that for each axis, we then had some relatively decent thresholds for our accelerometer to work over. 
  
## Milestone 2:

### Requirements:

  1. [✓] Store the time-stamps/people counts in RAM.
  2. [✓] Determine the accuracy of your counter by mounting it to a campus footbridge for 30 minutes
  3. [✓] Enable and configure the real time clock (RTC)  

### Files added/modified & used:
  1. main.c 
  2. rtc.c/.h
  3. rtc_lld.h  
  
  A majority of the progress for this milestone was performed in only the main.c file. We kept with the original structure for storing counts at time intervals as we had for milestone 1. Just as a reminder, that was an array of type uint8_t, and every index represented a time interval of 1 minute. In order to adapt that to the use of the real-time clock (RTC), we grab the starting time upon RTC configuration, and then count off of that when reading / manipulating the array values later one. So for example, we would grab the initial time (e.g. Minute 3, Second 53), and then when displaying the counts or sending them off to a receiver, we would just increment on that initial time based on an interval that we will choose at a later date. For now, that'll be every minute. So for this milestone since we are only reading for 30 minutes, the beginning and end time will be a 30 minute gap, supposedly. This removes the need to tack on the entire RTCDateTime structure to a count value. It's times like these where I wish we had the ability to work with solid data structures and not "amalgamation" data structures.

  In addition to that, when changing the sensortile to its more "permanent residence", we had to adjust our counting algorithm to adjust for accelerometer orientation. Beforehand, we had the SensorTile placed on a level board, where the orientation was a non-problem. Now, the little board and SensorTile hang freely and can face any-which way. We then went on to record 30 minutes of count from the foot bridge between Ballantine and Woodburn Hall. We recorded a count of 320 people, and our device recorded roughly 400 people. Based on that, we are going to go back and adjust our implementation in order to decrease it's sensitivity. As well as, the ability to differentiate between a lot of people crossing the bridge at once, which was a large source of the device's innacuracy. 
  
## Milestone 3:
  
### Requirements:
  1. [-] Write to flash memory  
  2. [-] Begin implementing BlueTooth infrastructure  
  3. [-] Work on count precision  

### Files added/modified & used:  
  1. main.c  
  
  So far, we have just added the ability to throttle back the count we're getting from the counting algorithm. We've enabled another thread to wakeup every few seconds and check if the trigger_count is too far ahead. Moreso, we're checking to see if the count is more than 3 greater than the count previous to the thread waking up. If so, we throttle it back and divide the current count by 3 and add that to the people count. This is under the assumption that each person is read as 3 steps by the accelerometer, and so each chunk of 3 from the current count is 1 person. From there, we just reset the trigger_count back to 0 and start over again.  

  Progress was made on the SensorTile_BlueNRG porting from HAL to ChibiOS. We have implemented the read/write functions for BlueNRG, as well as some of the functions that involving modifying ports and pins. We took that from HAL and translated that for ChibiOS used the PAL. From here, we need to modifiy HAL_Write_Serial in order for ChibiOS and all other files to use it. We need to also find what other files are using this SensorTile_BlueNRG, so we can begin porting them over as well if necessary. 
  
  
## Update log: 
  [3/06/17] : Milestone 1 finalized  
  [3/09/17] : Progress report created  
  [3/24/17] : RTC enabled / configured  
  [3/29/17] : Accelerometer readings adjusted for orientation  
  [3/31/17] : First set of recordings taken on the bridge
  [4/17/17] : Write to flash finalized
  [4/28/17] : Poster finalized
  