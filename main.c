
 /*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "../ChibiOS/os/hal/include/rtc.h"
#include <ctype.h>
#include "chprintf.h"
#include "shell.h"

#include "./driver/LSM303AGR_ACC_driver.h"
//#include "timestamp.h"
//#include "accelerometer.h"


#define UNUSED(x) (void)(x)
#define BOARD_LED LINE_SAI_SD

#define Z_Axis_Offset 250 // 200
#define X_Axis_Offset 150 // 100 
#define Y_Axis_Offset 150 // 100
#define XYZ_THRESHOLD 300

int trigger_count;
#define MAX_DAYS 1440 //array set value

static int a_data[3] = {0,0,0}; // accel data array 

static RTCDateTime starting_time; 
static RTCDateTime remember_time; 
//initialize the time count array in flash -- set to F

//uint16_t time_stamp[1440];
volatile uint16_t time_stamp[MAX_DAYS] __attribute__((section (".rodata"))) = { [0 ... MAX_DAYS-1] = 0xFFFF};

// time stamps stored in processor RAM
 
int timer_index = 0; 
int read_state;

int people; 
int check_count = 0; 


static void convert_axes(void);

/* Thread that retrieves accelerometer data*/

static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {

  UNUSED(arg);
  chRegSetThreadName("blinker");
  int junk;

  while (TRUE) {
    junk = LSM303AGR_ACC_Get_Acceleration(NULL, a_data);
    
    convert_axes();
    
    UNUSED(junk);

    if(read_state){
      chprintf((BaseSequentialStream *) &SD5, "X: %d\n\r", a_data[0]) ;
      chprintf((BaseSequentialStream *) &SD5, "Y: %d\n\r", a_data[1]) ;
      chprintf((BaseSequentialStream *) &SD5, "Z: %d\n\r", a_data[2]) ; 
    }

    chThdSleepMilliseconds(100);

  }
}

//function to write the data to the array 

/*void writeHourlyData(int hourly_array_index, uint16_t data) {
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  FLASH_ProgramHalfWord((uint32_t)(&time_stamp[hourly_array_index]),data);
  FLASH_Lock();
}*/


/* Thread that retrieves the number of counts every minute */

static THD_WORKING_AREA(timeStampThread,128);
static THD_FUNCTION(timeStamp,arg) {

  UNUSED(arg);
  chRegSetThreadName("timer");

  while (TRUE) {
  //old array write call 
  //time_stamp[timer_index++] = people; // trigger_count (was this)
  // new array write to flash 
  //writeHourlyData(timer_index++, people);
  trigger_count = 0;
  people = 0;

   chThdSleepMilliseconds(60000);

  }
}


 /* Thread that throttles the count based on interval reasoning 
  * 
  * If the count difference betweeen the start of the interval, and the end 
  * then the count is throttled back. This is supposed to account for the accel
  * picking up steps and counting that as multiple people. 
  *
  */


static THD_WORKING_AREA(throttleThread,256);
static THD_FUNCTION(throttleCount,arg) {

  UNUSED(arg);
  chRegSetThreadName("throttle");

  while (TRUE) {
    
    if((trigger_count - check_count) > 3){
      people += trigger_count / 3; 
      trigger_count = check_count = 0;
    } else{
      people++;
    }
    chThdSleepMilliseconds(3000);     // wakes up roughly every 3 seconds 

  }
}

/* Thread that increments the count based on trigger parameters */

static THD_WORKING_AREA(accelCounterThread,256);
static THD_FUNCTION(theCounterThread,arg) {

  UNUSED(arg);
  chRegSetThreadName("counter");
  int trash; 

  int32_t x_tuning_offset = 0;
  int32_t y_tuning_offset = 0;
  int32_t z_tuning_offset = 0;
  int32_t sleep_counts = 0;

  uint8_t thresholds; 
  uint8_t tmp_thresholds; 

  while (TRUE) {


    // this conditional determines which of the 3 axes are above the threshold
/*    if(a_data[2] >= Z_Axis_Offset) thresholds |= 0x4;
    if(a_data[1] >= Y_Axis_Offset) thresholds |= 0x2;
    if(a_data[0] >= X_Axis_Offset) thresholds |= 0x1; 
*/

    if(a_data[2] - z_tuning_offset < 0) z_tuning_offset += (a_data[2] - z_tuning_offset);
    if(a_data[1] - y_tuning_offset < 0) y_tuning_offset += (a_data[1] - y_tuning_offset);
    if(a_data[0] - x_tuning_offset < 0) x_tuning_offset += (a_data[0] - x_tuning_offset);


    if(abs(a_data[2] - z_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x4;
    if(abs(a_data[0] - x_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x1; 
    if(abs(a_data[1] - y_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x2;

/*
    if((a_data[2] - z_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x4;
    if((a_data[1] - y_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x2;
    if((a_data[0] - x_tuning_offset) > XYZ_THRESHOLD) thresholds |= 0x1; 
*/

    tmp_thresholds = thresholds;



    if(thresholds != 0){      // axes above the threshold triggering a count
      
      
      
      sleep_counts = 0;
      while(TRUE){

        chprintf((BaseSequentialStream *) &SD5, "In here\n\r");

        thresholds = 0;
        trash = LSM303AGR_ACC_Get_Acceleration(NULL, a_data);   // updates thresholds value

        convert_axes();                 // convert them back wrt to the thresholds 


          // same conditional as before
        if(a_data[2] - z_tuning_offset > XYZ_THRESHOLD) thresholds |= 0x4;
        if(a_data[1] - y_tuning_offset > XYZ_THRESHOLD) thresholds |= 0x2;
        if(a_data[0] - x_tuning_offset > XYZ_THRESHOLD) thresholds |= 0x1; 


        tmp_thresholds = thresholds;

        if(tmp_thresholds != 0){
          if (sleep_counts == 100) {
/*            z_tuning_offset = (a_data[2] + z_tuning_offset)/2;
            x_tuning_offset = (a_data[1] + y_tuning_offset)/2;
            y_tuning_offset = (a_data[0] + x_tuning_offset)/2;
*/
            z_tuning_offset = a_data[2];
            y_tuning_offset = a_data[1];
            x_tuning_offset = a_data[0];
            sleep_counts = 0;
          }
        } else{
          trigger_count++;
          break;
        }
        sleep_counts += 1;
        chThdSleepMilliseconds(1);
      }
      chprintf((BaseSequentialStream *) &SD5, "Incremented: %d\n\r", trigger_count);    
    } 
    chThdSleepMilliseconds(100);

    }
  UNUSED(trash);
}

// brings Z axis down an order of magnitude for conditional purposes
static void convert_axes(void){

  a_data[2] -= 1000;
  a_data[2] = abs(a_data[2]);

}

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;


  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}


// function that controls a read_state variable, that controls whether or not the accel values are being printed or not
static void cmd_accel_test(BaseSequentialStream *chp, int argc, char *argv[]){
  UNUSED(argc);
  UNUSED(argv);
  UNUSED(chp);  

  read_state = !read_state;

}


// displays all of the counts stored from the beginning to where it is now
static void cmd_display_counts(BaseSequentialStream *chp, int argc, char *argv[]){

  UNUSED(argc);
  UNUSED(argv);
  int i;


  
  if(timer_index == 0) chprintf(chp, "1 minute has yet to pass.\n\r");
  else{
    for(i = 0; i < timer_index; i++){
      chprintf(chp, "Minute: %d | Count: %d\n\r", i+1, time_stamp[i]); // timestamp[i] = time_stamp.count || timestamp[i] = timestamp[i - 1].count; time_stamp[i].count, time_stamp[i].time
    }
  }
  

}


// This function converts the arguments into a character array for strtok 
static void parse_args(int argc, char *argv[], char destination[]){
  UNUSED(argc);

  //chprintf((BaseSequentialStream *) &SD5, "%s\n\r", argv[0]);

  int i, j;

  for(j = 0; j < argc; j++){
    for(i = 0; i < (int)strlen(argv[j]); i++){
      //chprintf((BaseSequentialStream *) &SD5, "%c\n\r", argv[j][i]);
      destination[j+i] = argv[j][i]; 
    }
  }  
}


// calibrates the RTC with user input 
static void cmd_set_time(BaseSequentialStream *chp, int argc, char *argv[]){
  UNUSED(argc);
  UNUSED(chp);

  char destination[36];
 
  parse_args(argc, argv, destination);

  int x;
  for(x = 0; x < (int)sizeof(destination); x++){
   chprintf(chp, "%c", destination[x]);
  }

  chprintf(chp, "\n\r");


  char delim[2] = "/";
  char *token;
  UNUSED(token);

  // begins converting key 
  starting_time.year = atoi(strtok(destination,delim)); // str instead of argv
  chprintf(chp, "year: %u ", starting_time.year);
  starting_time.month = atoi(strtok(NULL,delim));
  chprintf(chp, "mon: %u ", starting_time.month);
  starting_time.day = atoi(strtok(NULL,delim));
  chprintf(chp, "day: %u ", starting_time.day);
  starting_time.dayofweek = atoi(strtok(NULL,delim));
  chprintf(chp, "dow: %u ", starting_time.dayofweek);
  starting_time.millisecond = atoi(strtok(NULL,delim));
  chprintf(chp, "ms: %u\n\r", starting_time.millisecond);

  rtcSetTime(&RTCD1, &starting_time);
  remember_time = starting_time;

}

// retrieves the RTC values from the data structure 

static void cmd_get_time(BaseSequentialStream *chp, int argc, char *argv[]){

  UNUSED(argc);
  UNUSED(argv);


  rtcGetTime(&RTCD1, &starting_time);
  chprintf(chp, "Year: %d | Month: %d | Day: %d | Day of Week: %d | Seconds: %d\n\r", 
    starting_time.year, starting_time.month, starting_time.day, starting_time.dayofweek,
    starting_time.millisecond/1000);

}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"accel", cmd_accel_test},
  {"display_counts", cmd_display_counts},
  {"get_time", cmd_get_time},
  {"set_time", cmd_set_time},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD5,
  commands
};

/*
 * Application entry point.
 */

int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  static thread_t *shelltp = NULL;

  halInit();
  chSysInit();

  LSM303AGR_ACC_Init(); // Initialize the Accelerometer 
  LSM303AGR_ACC_W_HiRes(NULL, LSM303AGR_ACC_HR_ENABLED);  // enables high-resolution mode
  LSM303AGR_ACC_W_ODR(NULL, LSM303AGR_ACC_ODR_DO_100Hz);  // sets the data rate to 100Hz

  /*
   * Activates the serial driver 5 using the driver default configuration.
   * PC12(TX) and PD2(RX). The default baud rate is 38400.
   */
  sdStart(&SD5, NULL);
  chprintf((BaseSequentialStream*)&SD5, "\n\rUp and Running\n\r");

  /* Initialize the command shell */ 
  shellInit();
  chThdCreateStatic(throttleThread, sizeof(throttleThread), NORMALPRIO+1, throttleCount, NULL);
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  chThdCreateStatic(accelCounterThread, sizeof(accelCounterThread), NORMALPRIO+1, theCounterThread, NULL);
  chThdCreateStatic(timeStampThread, sizeof(timeStampThread), NORMALPRIO+1, timeStamp, NULL);

  while (TRUE) {
    if (!shelltp) {
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    }
    else if (chThdTerminatedX(shelltp)) {
     chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
     shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(1000);
  }
}



/* /\*===========================================================================*\/ */
/* /\* Card insertion monitor.                                                   *\/ */
/* /\*===========================================================================*\/ */

/* #define POLLING_INTERVAL                10 */
/* #define POLLING_DELAY                   10 */

/* #define SENSORTILE_LED PAL_LINE(GPIOG,12U) */


/* /\** */
/*  * @brief   Card monitor timer. */
/*  *\/ */
/* static virtual_timer_t tmr; */

/* /\** */
/*  * @brief   Debounce counter. */
/*  *\/ */
/* static unsigned cnt; */

/* /\** */
/*  * @brief   Card event sources. */
/*  *\/ */
/* static event_source_t inserted_event, removed_event; */

/* /\** */
/*  * @brief   Insertion monitor timer callback function. */
/*  * */
/*  * @param[in] p         pointer to the @p BaseBlockDevice object */
/*  * */
/*  * @notapi */
/*  *\/ */
/* static void tmrfunc(void *p) { */
/*   BaseBlockDevice *bbdp = p; */

/*   chSysLockFromISR(); */
/*   if (cnt > 0) { */
/*     if (blkIsInserted(bbdp)) { */
/*       if (--cnt == 0) { */
/*         chEvtBroadcastI(&inserted_event); */
/*       } */
/*     } */
/*     else */
/*       cnt = POLLING_INTERVAL; */
/*   } */
/*   else { */
/*     if (!blkIsInserted(bbdp)) { */
/*       cnt = POLLING_INTERVAL; */
/*       chEvtBroadcastI(&removed_event); */
/*     } */
/*   } */
/*   chVTSetI(&tmr, MS2ST(POLLING_DELAY), tmrfunc, bbdp); */
/*   chSysUnlockFromISR(); */
/* } */

/* /\** */
/*  * @brief   Polling monitor start. */
/*  * */
/*  * @param[in] p         pointer to an object implementing @p BaseBlockDevice */
/*  * */
/*  * @notapi */
/*  *\/ */
/* static void tmr_init(void *p) { */

/*   chEvtObjectInit(&inserted_event); */
/*   chEvtObjectInit(&removed_event); */
/*   chSysLock(); */
/*   cnt = POLLING_INTERVAL; */
/*   chVTSetI(&tmr, MS2ST(POLLING_DELAY), tmrfunc, p); */
/*   chSysUnlock(); */
/* } */

/* /\*===========================================================================*\/ */
/* /\* FatFs related.                                                            *\/ */
/* /\*===========================================================================*\/ */

/* /\** */
/*  * @brief FS object. */
/*  *\/ */
/* static FATFS SDC_FS; */

/* /\* FS mounted and ready.*\/ */
/* static bool fs_ready = FALSE; */

/* /\* Generic large buffer.*\/ */
/* static uint8_t fbuff[1024]; */

/* static FRESULT scan_files(BaseSequentialStream *chp, char *path) { */
/*   FRESULT res; */
/*   FILINFO fno; */
/*   DIR dir; */
/*   int i; */
/*   char *fn; */

/* #if _USE_LFN */
/*   fno.lfname = 0; */
/*   fno.lfsize = 0; */
/* #endif */
/*   res = f_opendir(&dir, path); */
/*   if (res == FR_OK) { */
/*     i = strlen(path); */
/*     for (;;) { */
/*       res = f_readdir(&dir, &fno); */
/*       if (res != FR_OK || fno.fname[0] == 0) */
/*         break; */
/*       if (fno.fname[0] == '.') */
/*         continue; */
/*       fn = fno.fname; */
/*       if (fno.fattrib & AM_DIR) { */
/*         path[i++] = '/'; */
/*         strcpy(&path[i], fn); */
/*         res = scan_files(chp, path); */
/*         if (res != FR_OK) */
/*           break; */
/*         path[--i] = 0; */
/*       } */
/*       else { */
/*         chprintf(chp, "%s/%s\r\n", path, fn); */
/*       } */
/*     } */
/*   } */
/*   return res; */
/* } */

/* /\*===========================================================================*\/ */
/* /\* Command line related.                                                     *\/ */
/* /\*===========================================================================*\/ */

/* #define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048) */
/* #define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256) */

/* static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) { */
/*   size_t n, size; */

/*   (void)argv; */
/*   if (argc > 0) { */
/*     chprintf(chp, "Usage: mem\r\n"); */
/*     return; */
/*   } */
/*   n = chHeapStatus(NULL, &size); */
/*   chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX()); */
/*   chprintf(chp, "heap fragments   : %u\r\n", n); */
/*   chprintf(chp, "heap free total  : %u bytes\r\n", size); */
/* } */

/* static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) { */
/*   static const char *states[] = {CH_STATE_NAMES}; */
/*   thread_t *tp; */

/*   (void)argv; */
/*   if (argc > 0) { */
/*     chprintf(chp, "Usage: threads\r\n"); */
/*     return; */
/*   } */
/*   chprintf(chp, "    addr    stack prio refs     state time\r\n"); */
/*   tp = chRegFirstThread(); */
/*   do { */
/*     chprintf(chp, "%08lx %08lx %4lu %4lu %9s\r\n", */
/*             (uint32_t)tp, (uint32_t)tp.p_ctx.r13, */
/*             (uint32_t)tp.p_prio, (uint32_t)(tp.p_refs - 1), */
/*             states[tp.p_state]); */
/*     tp = chRegNextThread(tp); */
/*   } while (tp != NULL); */
/* } */

/* static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) { */
/*   thread_t *tp; */

/*   (void)argv; */
/*   if (argc > 0) { */
/*     chprintf(chp, "Usage: test\r\n"); */
/*     return; */
/*   } */
/*   tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(), */
/*                            TestThread, chp); */
/*   if (tp == NULL) { */
/*     chprintf(chp, "out of memory\r\n"); */
/*     return; */
/*   } */
/*   chThdWait(tp); */
/* } */

/* static void cmd_tree(BaseSequentialStream *chp, int argc, char *argv[]) { */
/*   FRESULT err; */
/*   uint32_t clusters; */
/*   FATFS *fsp; */

/*   (void)argv; */
/*   if (argc > 0) { */
/*     chprintf(chp, "Usage: tree\r\n"); */
/*     return; */
/*   } */
/*   if (!fs_ready) { */
/*     chprintf(chp, "File System not mounted\r\n"); */
/*     return; */
/*   } */
/*   err = f_getfree("/", &clusters, &fsp); */
/*   if (err != FR_OK) { */
/*     chprintf(chp, "FS: f_getfree() failed\r\n"); */
/*     return; */
/*   } */
/*   chprintf(chp, */
/*            "FS: %lu free clusters, %lu sectors per cluster, %lu bytes free\r\n", */
/*            clusters, (uint32_t)SDC_FS.csize, */
/*            clusters * (uint32_t)SDC_FS.csize * (uint32_t)MMCSD_BLOCK_SIZE); */
/*   fbuff[0] = 0; */
/*   scan_files(chp, (char *)fbuff); */
/* } */

/* static const ShellCommand commands[] = { */
/*   {"mem", cmd_mem}, */
/*   {"threads", cmd_threads}, */
/*   {"test", cmd_test}, */
/*   {"tree", cmd_tree}, */
/*   {NULL, NULL} */
/* }; */

/* static const ShellConfig shell_cfg1 = { */
/*   (BaseSequentialStream *)&SDU2, */
/*   commands */
/* }; */

/* /\*===========================================================================*\/ */
/* /\* Main and generic code.                                                    *\/ */
/* /\*===========================================================================*\/ */

/* /\* */
/*  * Card insertion event. */
/*  *\/ */
/* static void InsertHandler(eventid_t id) { */
/*   FRESULT err; */

/*   (void)id; */
/*   /\* */
/*    * On insertion SDC initialization and FS mount. */
/*    *\/ */
/*   if (sdcConnect(&SDCD1)) */
/*     return; */

/*   err = f_mount(&SDC_FS, "/", 1); */
/*   if (err != FR_OK) { */
/*     sdcDisconnect(&SDCD1); */
/*     return; */
/*   } */
/*   fs_ready = TRUE; */
/* } */

/* /\* */
/*  * Card removal event. */
/*  *\/ */
/* static void RemoveHandler(eventid_t id) { */

/*   (void)id; */
/*   sdcDisconnect(&SDCD1); */
/*   fs_ready = FALSE; */
/* } */

/* /\* */
/*  * Green LED blinker thread, times are in milliseconds. */
/*  *\/ */
/* /\* static THD_WORKING_AREA(waThread1, 128); *\/ */
/* /\* static THD_FUNCTION(Thread1, arg) { *\/ */

/* /\*   (void)arg; *\/ */
/* /\*   chRegSetThreadName("blinker"); *\/ */
/* /\*   while (true) { *\/ */
/* /\*     palToggleLine(LINE_ARD_D13); *\/ */
/* /\*     chThdSleepMilliseconds(fs_ready ? 250 : 500); *\/ */
/* /\*   } *\/ */
/* /\* } *\/ */

/* /\* */
/*  * Application entry point. */
/*  *\/ */
/* int main(void) { */
/*   static thread_t *shelltp = NULL; */
/*   static const evhandler_t evhndl[] = { */
/*     InsertHandler, */
/*     RemoveHandler */
/*   }; */
/*   // event_listener_t el0, el1; */

/*   /\* */
/*    * System initializations. */
/*    * - HAL initialization, this also initializes the configured device drivers */
/*    *   and performs the board-specific initializations. */
/*    * - Kernel initialization, the main() function becomes a thread and the */
/*    *   RTOS is active. */
/*    *\/ */
/*   halInit(); */
/*   chSysInit(); */
/*   palSetLineMode(SENSORTILE_LED, PAL_MODE_OUTPUT_PUSHPULL); */
/*   //palSetLine(SENSORTILE_LED); */
/*   palClearLine(SENSORTILE_LED); */
/*   palSetLine(SENSORTILE_LED); */
/*   while (1); */

/*   /\* */
/*    * Initialize board LED. */
/*    *\/ */
/*   /\* palSetLineMode(LINE_ARD_D13, PAL_MODE_OUTPUT_PUSHPULL); *\/ */

/*   /\* */
/*    * Initializes a serial-over-USB CDC driver. */
/*    *\/ */
/*   // sduObjectInit(&SDU2); */
/*   // sduStart(&SDU2, &serusbcfg); */

/*   /\* */
/*    * Activates the USB driver and then the USB bus pull-up on D+. */
/*    * Note, a delay is inserted in order to not have to disconnect the cable */
/*    * after a reset. */
/*    *\/ */
/*   // usbDisconnectBus(serusbcfg.usbp); */
/*   //chThdSleepMilliseconds(1500); */
/*   //usbStart(serusbcfg.usbp, &usbcfg); */
/*   //usbConnectBus(serusbcfg.usbp); */

/*   /\* */
/*    * Shell manager initialization. */
/*    *\/ */
/*   // shellInit(); */

/*   /\* */
/*    * Activates the serial driver 1 and SDC driver 1 using default */
/*    * configuration. */
/*    *\/ */
/*   //  sdStart(&SD1, NULL); */
/*   // sdcStart(&SDCD1, NULL); */

/*   /\* */
/*    * Activates the card insertion monitor. */
/*    *\/ */
/*   // tmr_init(&SDCD1); */

/*   /\* */
/*    * Creates the blinker thread. */
/*    *\/ */
/*   /\* chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL); *\/ */

/*   /\* */
/*    * Creates the HTTP thread (it changes priority internally). */
/*    *\/ */
/*   /\* chThdCreateStatic(wa_http_server, sizeof(wa_http_server), NORMALPRIO + 1, *\/ */
/*   /\*                   http_server, NULL); *\/ */

/*   /\* */
/*    * Normal main() thread activity, in this demo it does nothing except */
/*    * sleeping in a loop and listen for events. */
/*    *\/ */
/* //  chEvtRegister(&inserted_event, &el0, 0); */
/* //  chEvtRegister(&removed_event, &el1, 1); */
/* //  while (true) { */
/* //    if (!shelltp && (SDU2.config.usbp.state == USB_ACTIVE)) */
/* //      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO); */
/* //    else if (chThdTerminatedX(shelltp)) { */
/* //      chThdRelease(shelltp);    /\* Recovers memory of the previous shell.   *\/ */
/* //      shelltp = NULL;           /\* Triggers spawning of a new shell.        *\/ */
/* //    } */
/*     /\* if (palReadPad(GPIOI, GPIOI_BUTTON_USER) != 0) { *\/ */
/*     /\* } *\/ */
/* //    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, MS2ST(500))); */
/*  // } */
/* } */
