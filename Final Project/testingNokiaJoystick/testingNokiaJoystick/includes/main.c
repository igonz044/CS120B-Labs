/* testingNokiaJoystick.c
 *
 * Created: 6/2/2019 12:38:04 AM
 * Author : BackupBoot
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "nokia5110.c"
#include "io.c"
#include "timer.h"
#include "scheduler.h"

/* All the global definitions go here */
#define total_game_length 16   // although the LCD is a 16x2 I still needed the game to run more to introduce new levels if possible
#define b_one 7        //Bottom first obstacle position
#define T_one 11       //Top row first obstacle position
#define T_two 16       //Top row 2nd obstacle position
#define b_two 14	   //Bottom 2nd row obstacle position

//All variable definitions go here:
unsigned char start_game = 0; //common variable to help me start the game
unsigned char a, b;           //cursor positions
unsigned short x,y;           //somehow defining it into the function did not solve, have to define it globally.
//designing a special character for my  dodger (in bits mode to help with 4 bit input)
unsigned char sp_2 [8] = {0b11111,0b01101,0b00110,0b10110,0b01110,0b00110,0b00100,0b11110 };
const unsigned char First_row[] = "          *    *";  // Top track
const unsigned char Second_row[] = "      *      *   ";  // Bottom track
const unsigned char GameOver1[] = "      Game      "; // Top game over message
const unsigned char GameOver2[] = "    OVER :--(   "; // Bottom game over message
unsigned char charcater_location = 0;
unsigned char paused, game_OVER;
unsigned char firstRow_obs, secondRow_obstacle;
// initializing the code for joystick.
//The reason why
void InitADC() {
	ADMUX=(1<<REFS0);                   //setting the reference of ADC such THAT Areff=Avcc
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
//used something new data type her to check if it works or not, unsigned integer short can also be written as unint16_t

uint16_t ReadADC(uint8_t ch)
{	InitADC();
	ch=ch&0b00000111; //im using this channel to be a extra safety net for the correct reading
	ADMUX|=ch; // it will read from the first three channels
	//conversion steps
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF); // got from one of the labs
	return(ADC);
}
//code for reading which direction is the joystick pointing to0
uint16_t find_direction_from_xy(uint16_t x,uint16_t y)
{
	//1-> up, 2-> right, 3-> down 4-> left
	if(x>255 && x<765 && y>765) // we know 1024 is the max value, these values are exact half of the max values which i had using the joystick
	return(1); // direction is up

	else if(x>765 && y>255 && y<765)
	return(2); //direction is right

	else if(x>255 && x<765 && y<255)
	return(3); //direction is down

	else if(x<255 && y>255 && y<765)
	return(4); // direction is left

	else
	return(0); //default state
}
//instructions to initializing the games
const unsigned char strt [] = { 'M','o','v','e',' ','J','o','y','s','t','i','c','k',' ' ,'t','o',' ' , 'S', 't', 'a', 'r', 't'};
const unsigned short msg_strt = sizeof(strt)/sizeof(char);
//Welcome message.
const unsigned char Wlcme_Mssg [] = {'W','e','l','c','o','m', 'e',' ','t','o',' ','t','h','e','   ','D','o','d','g','i','n','g',' ','G','a','m','e','!'};
const unsigned short msg_wlcm = sizeof(Wlcme_Mssg)/sizeof(char);
//displaying the welcome message and the initializing the game
enum L_States {Init, Menu_Welcome, Menu_Start} m_state;
void L_Tick() //16x2 LCD tick function
{
	switch (m_state)
	{
		case Init:m_state= Menu_Welcome;  _delay_ms(2000);break;
		case Menu_Welcome: _delay_ms(200); m_state = Menu_Start; LCD_ClearScreen();    break;
		case Menu_Start: if(find_direction_from_xy(x,y)==1 || find_direction_from_xy(x,y)==2 ||find_direction_from_xy(x,y)==3 || find_direction_from_xy(x,y)==4){
			start_game= 0x01;
		_delay_ms(200); LCD_ClearScreen(); }
	}
	switch (m_state)
	{
		case Init: break;
		case Menu_Welcome: a = 0;
		b = 0;
		while(a < msg_wlcm){
			LCD_Cursor(a + 1);
			LCD_WriteData(Wlcme_Mssg[a]);
		a++;} _delay_ms(2000);	break;
		case Menu_Start:a = 0; 	b = 0;
		while(a < msg_strt){
			LCD_Cursor(a + 1);
			LCD_WriteData(strt[a]);
		a++;}
		default:break;
	}
}
enum gameTrack_States { gameTrack_start, gameTrack_initial, gameTrack_wait, gameTrack_gameOver };
//Initial start of the game
int gameTrack_tickfct(int state) {

	switch(state) { // Transitions
		case gameTrack_start: state = gameTrack_initial;break;
		case gameTrack_initial: charcater_location = 0;  state = gameTrack_wait; break;// sets the initial track charcater to be at 0;
		case gameTrack_wait:if (game_OVER) {state= gameTrack_gameOver;}  //if game was lost, then character freezes. and reset to first position
		else if (charcater_location >= total_game_length) charcater_location = 0;//otherwise restart the track
		break;
		case gameTrack_gameOver: if(!game_OVER) state = gameTrack_initial; break;//resetting the game
		default: state = gameTrack_start; break; //just the default state.
	}

	switch(state) { // Actions
		case gameTrack_wait: charcater_location++; break; //moving forward with the game
		default: break;
	}

	return state;
}
enum firstRow_States { firstRow_start, firstRow_initial, firstRow_wait, firstRow_gameOver };
//first row track
int firstRow_tickfct(int state) {
	static unsigned char a, b;  //changing it to static to make it into active state as it changes with the game
	switch(state) { // Transitions
		case firstRow_start: state = firstRow_initial;break;
		case firstRow_initial: a = 1; //cursor position at 1
		b = charcater_location; state = firstRow_wait;  break;
		case firstRow_wait: //debug state
		if(game_OVER) { //if game is over in the first row then it resets the cursor and the character to be 0
			state = firstRow_gameOver; 	b = firstRow_obs = 0;
			a = 1;
		}
		else if (a > total_game_length) { //if reached the end of the screen then restart
			state = firstRow_initial;
		}break;
		case firstRow_gameOver: if(a <= total_game_length) {
			LCD_Cursor(a);  LCD_WriteData(GameOver1[b % total_game_length]);
			a++; b++;
		}
		else if(!game_OVER) state = firstRow_initial; break; //new game
	default: state = firstRow_start; break;   }
	
	switch(state) {// Actions
		case firstRow_wait:  LCD_Cursor(a);   LCD_WriteData(First_row[b % total_game_length]);
		a++; b++; //ading the track
		if ( charcater_location== T_one || charcater_location == T_two) firstRow_obs = 1;   // if found obstacle in first row
		else firstRow_obs = 0;   // clear flag otherwise
		break;
		default: break;
	}

	return state;
}
enum secondRow_States { secondRow_start, secondRow_initial, secondRow_wait, secondRow_lose };
//second row track
int TickFct_Track2(int state) {	static unsigned char a, b; //debug: making it active again

	switch(state) { // Transitions
		case secondRow_start: state = secondRow_initial; break;
		case secondRow_initial: a = 17;   //bottom row so n-1
		b = charcater_location; state = secondRow_wait;break;
		case secondRow_wait: if(game_OVER) { state = secondRow_lose; b = secondRow_obstacle = 0;
		a = 17;   }
		else if (a > total_game_length*2) { //same as first row track
		state = secondRow_initial; } break;
		case secondRow_lose: 	if(a <= total_game_length*2) { //legit copy paste from the first lol
			LCD_Cursor(a);  LCD_WriteData(GameOver2[b % total_game_length]);
			a++; b++;
		} else if(!game_OVER) state = secondRow_initial;
		break;
		default: state = secondRow_start; break;   // something went wrong, go back to START
	}

	// Actions
	switch(state) {
		case secondRow_wait:LCD_Cursor(a); LCD_WriteData(Second_row[b % total_game_length]);
		a++; b++;
		if (charcater_location == b_one || charcater_location==b_two ) secondRow_obstacle = 1;   // we setted the obstacles in the global definitions
		else secondRow_obstacle = 0;  break;
		default: break;
	}

	return state;
}
enum PlayerMovement_States { PlayerMovement_Start, PlayerMovement_Initial, PlayerMovement_Wait, PlayerMovement_GameOver };
//movement of custom character
int PlayerMovement_tickfct(int state) {static unsigned char a; //not really need another
	switch(state) { // Transitions
		case PlayerMovement_Start: 	state = PlayerMovement_Initial; break;
		case PlayerMovement_Initial: a = 2; //set to 2 since otherwise goes out of LCD screen
		game_OVER = 0; state = PlayerMovement_Wait; break;
		case PlayerMovement_Wait: //checking joystick position for jumping
		if((find_direction_from_xy(x,y)==4)) { a = 2; }
		//move the character to the bottom row
		else if ((find_direction_from_xy(x,y)==2) ){ a = 18; }
		//restart the game
		else if (find_direction_from_xy(x,y)==1 || find_direction_from_xy(x,y)==3) {
			state = PlayerMovement_GameOver;        if (game_OVER) game_OVER = 0;
		}
		//checking collision
		if ((a == 2 && firstRow_obs) || (a == 18 && secondRow_obstacle)) {
			game_OVER = 1;          //end game
			state = PlayerMovement_GameOver;
		} break;
		case PlayerMovement_GameOver:
		//move the joystick to restart.
		if (find_direction_from_xy(x,y)==1 || find_direction_from_xy(x,y)==3) { start_game = 0x00;
			LCD_ClearScreen();
		}
		else state = PlayerMovement_Wait; 	break;
		default:	state = PlayerMovement_Start; break;
	}

	switch(state) { //actions
		case PlayerMovement_Wait: LCD_Cursor(a);  LCD_WriteData(1);
		if (!game_OVER) LCD_Cursor(a); 	break;
		default: break;
	}

	return state;
}//main logic

int main(void){
	//setting input and output lines
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//using game scheduler
	unsigned long int gameTrack_Period_calc = 100;
	unsigned long int First_row_Period_calc = 10;
	unsigned long int second_Row_Period_calc = 10;
	unsigned long int Player_Movement_Period_calc = 5;
	//find gcd
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(gameTrack_Period_calc, First_row_Period_calc);
	tmpGCD = findGCD(tmpGCD, second_Row_Period_calc);
	tmpGCD = findGCD(tmpGCD, Player_Movement_Period_calc);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int Game_period = gameTrack_Period_calc/GCD;
	unsigned long int Row1_period = First_row_Period_calc/GCD;
	unsigned long int Row2_period = second_Row_Period_calc/GCD;
	unsigned long int player_period = Player_Movement_Period_calc/GCD;
	
	static task task1, task2, task3, task4;
	
	task *tasks[] = { &task1, &task2, &task3, &task4 };
	
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = gameTrack_start;
	task1.period = Game_period;
	task1.elapsedTime = Game_period;
	task1.TickFct = &gameTrack_tickfct;

	task2.state = firstRow_start;
	task2.period = Row1_period;
	task2.elapsedTime = Row1_period;
	task2.TickFct = &firstRow_tickfct;

	task3.state = secondRow_start;
	task3.period = Row2_period;
	task3.elapsedTime = Row2_period;
	task3.TickFct = &TickFct_Track2;

	task4.state = PlayerMovement_Start;
	task4.period = player_period;
	task4.elapsedTime = player_period;
	task4.TickFct = &PlayerMovement_tickfct;
	
	//creating that special character
	LCD_CreateCharacter(sp_2, 1);	
	
	//Initialize LCD module
	LCD_init();
	LCD_ClearScreen();

	TimerSet(GCD);
	TimerOn();
	
	InitADC();
	nokia_lcd_init();
	nokia_lcd_clear();
	
	unsigned short h;

	while (1) {
		x=ReadADC(0);  //Read ADC Channel 0
		y=ReadADC(1);  //Read ADC Channel 1
		
		if(start_game == 0x00){
			L_Tick();
			//setting the nokia LCD for instructions
			nokia_lcd_clear();
			nokia_position(0,0);
			nokia_writeString("Instructions ", 1);
			nokia_position(0,10);
			nokia_writeString("1.Move Joystick to Start",1);
			nokia_position(0,30);
			nokia_writeString("2.Move Right/Left to restart",1);
			nokia_lcd_render();
		}
		else if(start_game == 0x01){
			nokia_lcd_clear();
			nokia_position(0,0);
			nokia_writeString("Good", 2);
			nokia_position(10,30);
			nokia_writeString("Luck!", 2);
			
			nokia_lcd_render();
			for(h = 0; h < numTasks; h++) {
				// task is ready to tick
				if(tasks[h]->elapsedTime == tasks[h]->period) {
					// setting next state for task
					tasks[h]->state = tasks[h]->TickFct(tasks[h]->state);
					// reset the elapsed time for the next tick
					tasks[h]->elapsedTime = 0;
				}
				tasks[h]->elapsedTime += 1;
			}
			while(!TimerFlag);
			TimerFlag = 0;
		}	
	}
}