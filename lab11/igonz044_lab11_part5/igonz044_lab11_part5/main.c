 /* Name & Email: Itzel G. igonz044@ucr.edu
 *   Lab Section: 026
 *    Assignment: igonz044_lab11_part5.c
 *        Author: BackupBoot 
 *       Created: 5/24/2019 5:17:26 PM
 * Exercise Description: Keypad with LCD will save numbers inputted
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>
#include "io.c"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"

unsigned char PINAtmp;
unsigned char resetFlag = 0;

//SM1 Declarations
enum PlayPauseStates {init, Press, Release} PP_State;
int PP_next;
int PPFlag = 0;  	//0 is pause, 1 is play

//SM2 Declarations
enum Obstacle_State {init_OS, play_OS, pause_OS} OS_State;
int OS_next;
unsigned char Position1;
unsigned char Position2;

//SM3 Declarations
enum Player_State {init_PS, wait_PS, pressUp_PS, pressDown_PS, release_PS, pause_PS} PS_State;
int PS_next;
unsigned char PlayerPosition;

//SM4 Declarations
enum Display_State {init_DS, check_DS, display_DS, winner_DS, loser_DS, pause_DS} DS_State;
int DS_next;

int PlayPauseTick(int A);
int ObstacleTick(int B);
int PlayerTick(int C);
int DisplayTick(int D);

int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFC; PORTA = 0x03;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	// Period for the tasks

	//Declare an array of tasks
	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2,&task3,&task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	//There was only one task to do so the size = 1;
	task1.state = 0;
	task1.period = 10; //Task Period.
	task1.elapsedTime =task1.period;
	task1.TickFct = &PlayPauseTick;

	task2.state = 0;
	task2.period = 200; //Task Period.
	task2.elapsedTime =task2.period;
	task2.TickFct = &ObstacleTick;
	
	task3.state = 0;
	task3.period = 200; //Task Period.
	task3.elapsedTime =task3.period;
	task3.TickFct = &PlayerTick;
	
	task4.state = 0;
	task4.period = 200; //Task Period.
	task4.elapsedTime =task4.period;
	task4.TickFct = &DisplayTick;

	// Set the timer and turn it on
	TimerSet(10);
	TimerOn();
	LCD_init();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		PINAtmp = ~PINA & 0x1C;
		for ( i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}

int PlayPauseTick (int PP_State)
{
	unsigned char Reset = PINAtmp & 0x04;
	switch(PP_State)
	{
		case init:
		PP_next = Press;
		break;
		case Press:
		

		if(Reset == 1)
		{
			PP_next = Release;
		}
		else
		{
			PP_next = Press;
		}
		break;

		case Release:
		if(Reset == 0)
		{
			PP_next = Press;
		}
		else
		{
			PP_next = Release;
		}
		break;

		default:
		PP_next = init;
		break;
	}

	switch(PP_State)
	{
		case init:
		PPFlag = 0;
		break;
		case Press:
		if(resetFlag == 1)
		{
			PPFlag = 0;
		}
		PPFlag = PPFlag;
		break;

		case Release:
		if(PP_next == Press)
		{
			PPFlag = !PPFlag;
		}
		break;

		default:
		PPFlag = 0;
		break;
	}

	PP_State = PP_next;
	return PP_State;
}

int ObstacleTick(int OS_State)
{
	switch(OS_State)
	{
		case init_OS:
		if(PPFlag == 1)
		{
			OS_next = play_OS;
		}
		
		else
		{
			OS_next = init_OS;
		}
		break;

		case play_OS:
		if(resetFlag == 1)
		{
			OS_next = init_OS;
		}
		if(PPFlag == 0)
		{
			OS_next = pause_OS;
		}
		else
		{
			OS_next = play_OS;
		}
		break;
		
		case pause_OS:
		if(PPFlag == 1)
		{
			OS_next = play_OS;
		}
		else
		{
			OS_next = pause_OS;
		}
		
		break;

		default:
		OS_next = init;
		break;
	}

	switch(OS_State)
	{
		case init_OS:
		Position1 = 14;
		Position2 = 32;
		break;

		case play_OS:
		Position1 = Position1 > 1 ? Position1 - 1 : 1;
		Position2 = Position2 > 1 ? Position2 - 1 : 1;
		break;
		
		case pause_OS:
		Position1 = Position1;
		Position2 = Position2;
		break;
		
		default:
		Position1 = 14;
		Position2 = 32;
		break;
	}

	OS_State = OS_next;
	return OS_State;
}

//enum Player_State {init_PS, wait_PS, pressUp_PS, pressDown_PS, release_PS, pause_PS} PS_State;
int PlayerTick(int PS_State)
{
	unsigned char UpDown = PINAtmp & 0x18;
	switch(PS_State)
	{
		case init_PS:
		PS_next = PPFlag == 1 ? wait_PS : init_PS;
		break;

		case wait_PS:
		if(resetFlag == 1)
		{
			PS_next = init_PS;
		}
		else if(PPFlag == 0)
		{
			PS_next = pause_PS;
		}
		else if (UpDown == 0x08)
		{
			PS_next = pressUp_PS;
		}
		else if (UpDown == 0x10)
		{
			PS_next = pressDown_PS;
		}
		else
		{
			PS_next = wait_PS;
		}
		break;

		case pressUp_PS:
		PS_next = Release;
		break;
		
		case pressDown_PS:
		PS_next = Release;
		break;
		
		case release_PS:
		if(UpDown = 0x00)
		{
			PS_next = wait_PS;
		}
		else
		{
			PS_next = release_PS;
		}
		break;
		
		case pause_PS:
		if(PPFlag = 1)
		{
			PS_next = wait_PS;
		}
		else
		{
			PS_next = pause_PS;
		}
		break;

		default:
		PS_next = init_PS;
		break;
	}

	switch(OS_State)
	{
		case init_PS:
		PlayerPosition = 1;
		break;

		case wait_PS:
		PlayerPosition++;
		break;

		case pressUp_PS:
		PlayerPosition = PlayerPosition + 16;
		break;
		
		case pressDown_PS:
		PlayerPosition = PlayerPosition - 16;
		break;
		
		case release_PS:
		PlayerPosition = PlayerPosition;
		break;
		
		case pause_PS:
		PlayerPosition = PlayerPosition;
		break;

		default:
		PlayerPosition = 1;
		break;
	}

	OS_State = OS_next;
	return OS_State;
}

int DisplayTick(int DS_State)
{
	switch(DS_State)
	{
		case init_DS:
		if(PPFlag == 1)
		{
			DS_next = check_DS;
		}
		else
		{
			DS_next = init_DS;
		}
		break;

		case check_DS:
		if(PPFlag == 0)
		{
			DS_next = pause_DS;
		}
		else if(PlayerPosition == Position1 || PlayerPosition == Position2)
		{
			DS_next = loser_DS;
		}
		else if(PlayerPosition == 32 || PlayerPosition == 16)
		{
			DS_next = winner_DS;
		}
		else
		{
			DS_next = display_DS;
		}
		break;

		case display_DS:
		DS_next = check_DS;
		break;

		case winner_DS:
		if(PPFlag == 1)
		{
			DS_next = init_DS;
		}
		else
		{
			DS_next = winner_DS;
		}
		break;

		case loser_DS:
		if(PPFlag == 1)
		{
			DS_next = init_DS;
		}
		else
		{
			DS_next = winner_DS;
		}
		break;

		case pause_DS:
		if(PPFlag == 1)
		{
			DS_next = check_DS;
		}
		else
		{
			DS_next = winner_DS;
		}
		break;

		default:
		DS_next = init_DS;
		break;
	}

	switch(DS_State)
	{
		case init:
		resetFlag = 0;
		LCD_ClearScreen();
		if(PPFlag == 1)
		{
			LCD_Cursor(PlayerPosition);
			LCD_WriteData ('>');

			LCD_Cursor(Position1);
			LCD_WriteData ('#');

			LCD_Cursor(Position2);
			LCD_WriteData ('#');
		}
		break;

		case check_DS:
		resetFlag = 0;
		break;

		case display_DS:
		LCD_ClearScreen();

		LCD_Cursor(PlayerPosition);
		LCD_WriteData ('>');

		LCD_Cursor(Position1);
		LCD_WriteData ('#');

		LCD_Cursor(Position2);
		LCD_WriteData ('#');
		break;

		case winner_DS:
		LCD_DisplayString(1, "You Win!");
		resetFlag = 1;
		break;

		case loser_DS:
		LCD_DisplayString(1, "You Lose :(");
		resetFlag = 1;
		break;

		case pause_DS:
		LCD_DisplayString(1, "Pause");
		resetFlag = 1;
		break;

		default:
		LCD_ClearScreen();
		resetFlag = 0;
		break;
	}

	DS_State = DS_next;
	return DS_State;
}


