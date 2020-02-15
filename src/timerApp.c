/*
 * Author: Craig Einstein
 * File: timeTrial.c
 * Description: Implements a timer that performs a function a set amount of times per second
 */

#include <timerApp.h>

//Returns the difference between two times in nanoseconds
long diffTime(struct timespec * t2, struct timespec * t1){
  return ((t2->tv_sec * NSEC) + t2->tv_nsec) - ((t1->tv_sec * NSEC) + t1->tv_nsec);
}

//Returns the difference between two times as a pointer to a timespec structure
struct timespec * diffTimeStruct(struct timespec * t2, struct timespec * t1){
  struct timespec * r;
  int sub;
  r = (struct timespec *)malloc(sizeof(struct timespec) * 1);
  sub = 0;
  if(t1->tv_nsec > t2->tv_nsec){
    sub = 1;
    r->tv_nsec = NSEC - (t1->tv_nsec - t2->tv_nsec);
    goto sec;
  }

  r->tv_nsec = t2->tv_nsec - t1->tv_nsec;

 sec:
  r->tv_sec = (t2->tv_sec - t1->tv_sec) - sub;

  return r;
}

//Outputs the time to the terminal
__attribute__ ((gnu_inline)) inline void printTime(timerVals_t * t){
  printf("\b\b\b\b\b\b\b\b");
    if(t->h < 10){
      printf("0");
    }
    printf("%d:",t->h);

    if(t->m < 10){
      printf("0");
    }
    printf("%d:",t->m);
    if(t->s < 10){
      printf("0");
    }
    printf("%d",t->s);
    fflush(stdout);

}

//Increases the time
__attribute__ ((gnu_inline)) inline void progressTime(timerVals_t * t){
    ++t->s;
    if(t->s > 59){
      t->s = 0;
      ++t->m;
    }

    if(t->m > 59){
      t->m = 0;
      ++t->h;
    }

    if(t->h > 23){
      t->h = 0;
    }
}  

//Measures the time and performs a function N times per second.
//N is set by the reaction time and the reaction time must be less than one second
//The reaction time must also be evenly divisible by 1000 (1000 % N == 0)
void timerFunc(timerVals_t * timer){
  int i;
  while(1){
    nanos(&timer->now);
    nanos(&timer->prevTime);
    for(i = 0; i < SEC/timer->rTimeMS; ++i){
      
      while((diffTime(&timer->now, &timer->prevTime) / NtoMS) < timer->rTimeMS){
	nanos(&timer->now);
      }
      if(timer->task){
      timer->task(timer->taskData ? timer->taskData : NULL);
      }
      
      nanos(&timer->prevTime);
    }

    progressTime(timer);
    printTime(timer);
  }

}

//Returns a pointer to a timer structure
timerVals_t * createTimer(){
  timerVals_t * t;
  t = (timerVals_t *)malloc(sizeof(timerVals_t) * 1);
  t->h = 0;
  t->m = 0;
  t->s = 0;
  t->rTimeMS = 0;
  t->task = NULL;
  t->taskData = NULL;
  return t;
}

//Finds the closest factor of SEC to the given reacion time
unsigned long searchForClosestFactor(unsigned long rTimeMS_){

  int i;
  int pos, neg;
  if(rTimeMS_ > SEC/2){
    return SEC;
  }

  pos = 0;
  i = rTimeMS_;
  while(SEC % i){
    ++i;
  }
  pos = i;

  
  neg = 0;
  i = rTimeMS_;
  while(SEC % i){
    --i;
  }
  neg = i;
  
  return (pos - rTimeMS_) < (rTimeMS_ - neg) ? pos : neg;
  
}

//Sets the reaction time of the structre
//Ensures that the reaction time conforms to the constraints
//Sets a value that conforms to the constraints and is as close as possible to the inputted value
//If rTime is greater than 1000, sets rTime to 1000
int setReactionTime(timerVals_t * t_, unsigned long rTimeMS_){
  int rTimeUpdate;

  if(rTimeMS_ > SEC || !rTimeMS_){
    t_->rTimeMS = SEC;
    return rTimeMS_ = SEC;
  }
  
  if(!(SEC % rTimeMS_)){
    t_->rTimeMS = rTimeMS_;
    return 0;
  }
  

  rTimeUpdate = searchForClosestFactor(rTimeMS_);
  t_->rTimeMS = rTimeUpdate;
  
  return rTimeUpdate - rTimeMS_;

}


//Sets the task for the timer and provides a pointer ot the taskData
//If there is no data for the task, NULL can be input
int setTaskFunction(timerVals_t * t_, void(*task_)(void *), void * taskData_){
  t_->task = task_;
  t_->taskData = taskData_;
  return 0;
    
}

int main(int argc, char ** argv){
  timerVals_t * t = createTimer();
  setReactionTime(t, 0);
  timerFunc(t);
  
  return 0;
}
