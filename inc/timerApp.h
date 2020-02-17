/*
 * Author: Craig Einstein
 * Filename timeTria/h
 * Description: Structures and function definitions for the timer application
 */

#ifndef TIMEAPP_H
#define TIMEAPP_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>



#define NSEC 1000000000
#define NtoMS 1000000
#define SEC 1000 //second in ms
#define nanos(a) clock_gettime(CLOCK_MONOTONIC_RAW, a)


typedef struct timerVals{
  unsigned long rTimeMS;
  void (*task)(void *);
  void * taskData;
  struct timespec now;
  struct timespec prevTime;
  uint8_t h;
  uint8_t m;
  uint8_t s;
}timerVals_t;

//Timer functions
long diffTime(struct timespec * t2, struct timespec * t1);
struct timespec * diffTimeStruct(struct timespec * t2, struct timespec * t1);


#endif
