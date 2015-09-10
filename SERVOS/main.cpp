#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <fcntl.h>
#include "kbhit.h"

#include "global.h"
#include "servo.h"

//-------------------------------------
//--------- Main-----------------------


//-------------------------------------
//TODO : CREATE a SERVO.h file in order to make subroutines
//
//
// If you set a servo width to 0 it turns off the servo output, without changing
// the current servo position.
// The code supports 8 servos, the control signals of which should be connected
// to P1 header pins as follows:
//      Servo number    GPIO number   Pin in P1 header
//           0               4             P1-7
//           1              17             P1-11
//           2              18             P1-12
//           3             21/27           P1-13
//           4              22             P1-15
//           5              23             P1-16
//           6              24             P1-18
//           7              25             P1-22
// P1-13 is connected to either GPIO-21 or GPIO-27, depending on board revision.
//
//
//-------------------------------------

using namespace std;
/*
	+ - +10 на все моторы
	- - -10 на все моторы
	0 - востоновить моторы до saved_state
	w - +5 вперед
	s - +5 назад
	a - +5 влево
	d - +5 вправо
	z - запись взлета в лог(нажать при взлете)
	x - запись в лог при полете в сторону
	esc - выход
 
*/


int main(int argc, char *argv[])
{
  int servoval[4];
  Servo ESC;
  bool hit=true;
  keyboard kb4;

  int saved_state = 1200;
  int step = 10;
  int step_action = 5;

  ESC.open_blaster();

  FILE *file=fopen("flylog.txt","w");

  printf("Fly started\n");
    servoval[0]=saved_state;
    servoval[1]=saved_state;
    servoval[2]=saved_state;
    servoval[3]=saved_state;
  hit=true;
  while(hit){

    if (kb4.kbhit()!=0) {
      switch(kb4.getch()){
	case '+':
	 servoval[0]+=step;
   	 servoval[1]+=step;
  	 servoval[2]+=step;
    	 servoval[3]+=step;
	 saved_state+=step;
	 break;
	case '-':
	 servoval[0]-=step;
   	 servoval[1]-=step;
  	 servoval[2]-=step;
    	 servoval[3]-=step;
	 saved_state-=step;
	 break;
	case '0':
	 servoval[0]=saved_state;
   	 servoval[1]=saved_state;
  	 servoval[2]=saved_state;
    	 servoval[3]=saved_state;
	 break;
	case 'w':
	 servoval[0]+=step_action;
   	 servoval[1]-=step_action;
  	 servoval[2]-=step_action;
    	 servoval[3]+=step_action;
	 break;
	case 's':
	 servoval[0]-=step_action;
   	 servoval[1]+=step_action;
  	 servoval[2]+=step_action;
    	 servoval[3]-=step_action;
	 break;
	case 'a':
	 servoval[0]-=step_action;
   	 servoval[1]-=step_action;
  	 servoval[2]+=step_action;
    	 servoval[3]+=step_action;
	 break;
	case 'd':
	 servoval[0]+=step_action;
   	 servoval[1]+=step_action;
  	 servoval[2]-=step_action;
    	 servoval[3]-=step_action;
	 break;
	case 'z':
	 fprintf(file, "Взлетел: %d\n", saved_state);
	 break;
	case 'x':
	 fprintf(file, "Нормальный полет в сторону: 0 = %d 1 = %d 2 = %d 3 = %d\n", servoval[0], servoval[1], servoval[2], servoval[3] );
	 break;
	case 27:
	 hit = false;
	 break;
	case 'm':
	 printf("0 = %d 1 = %d 2 = %d 3 = %d\n", servoval[0], servoval[1], servoval[2], servoval[3] );
	 break;

      }
    }
    ESC.set(servoval);
    sleep(0.5);
  }
    fclose(file);
  return 0;
}



