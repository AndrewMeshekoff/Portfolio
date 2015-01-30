#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>


volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.


// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks


// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}


void TimerOn() {
        // AVR timer/counter controller register TCCR1
        TCCR1B = 0x0A;        // bit3 = 1: CTC mode (clear timer on compare)
        // bit2bit1bit0=011: prescaler /64
        // 00001011: 0x0B
        // SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
        // Thus, TCNT1 register will count at 125,000 ticks/s


        // AVR output compare register OCR1A.
        OCR1A = 100;        // Timer interrupt will be generated when TCNT1==OCR1A
        // We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
        // So when TCNT1 register equals 125,
        // 1 ms has passed. Thus, we compare to 125.
        // AVR timer interrupt mask register


        TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt


        //Initialize avr counter
        TCNT1 = 0;


        // TimerISR will be called every _avr_timer_cntcurr milliseconds
        _avr_timer_cntcurr = _avr_timer_M;


        //Enable global interrupts
        SREG |= 0x80;        // 0x80: 1000000
}


void TimerOff() {
        TCCR1B = 0x00; // bit3bit2bit1bit0=0000: timer off
}


void TimerISR() {
        TimerFlag = 1;
}


// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
        // CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
        _avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
        if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
        TimerISR(); // Call the ISR that the user uses
        _avr_timer_cntcurr = _avr_timer_M;
}
}


void ADC_init() {
        ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
        // ADEN: setting this bit enables analog-to-digital conversion.
        // ADSC: setting this bit starts the first conversion.
        // ADATE: setting this bit enables auto-triggering. Since we are
        //            in Free Running Mode, a new conversion will trigger whenever
        //            the previous conversion completes.
}


// Pins on PORTA are used as input for A2D conversion
// The default channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA
// used for A2D conversion
// Valid values range between 0 and 7, where the value
// represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
    ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
    // Allow channel to stabilize
    static unsigned char i = 0;
    for ( i=0; i<15; i++ ) { asm("nop"); }
}


//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
        unsigned long int c;
        while(1){
            c = a%b;
            if(c==0){return b;}
            a = b;
            b = c;
        }
        return 0;
}


//--------Task scheduler data structure---------------------------------------
typedef struct _task {
        /*Tasks should have members that include: state, period,
            a measurement of elapsed time, and a function pointer.*/
        signed char state; //Task's current state
        unsigned long int period; //Task period
        unsigned long int elapsedTime; //Time elapsed since last task tick
        int (*TickFct)(int); //Task tick function
} task;




//--------Shared Variables----------------------------------------------------
unsigned char posX;
unsigned char posY;
unsigned char playPos;


unsigned char height = 32;


unsigned char NoteBuf[4][32];
unsigned char speakCount[32];
unsigned char tones[32][8];


unsigned char frequencies[32];


unsigned char UI;


unsigned char regX;
unsigned char regY;
unsigned char playRegX;
unsigned char measures;


unsigned char col;
unsigned char screenLine;
unsigned char screenD;


unsigned char mode;
unsigned char zoom;


unsigned char tempo;


//--------Functions-----------------------------------------------------------
void transmit_data (unsigned char data1, unsigned char data2) {
        char i;
        for (i = 0; i < 8 ; ++i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTC = 0x88;
            // set SER = next bit of data to be sent.
            PORTC |= ((data1 >> i) & 0x01);
            if(i<4)
                    PORTC |= ((data2 << (4-i)) & 0x10);
            else
                    PORTC |= ((data2 >> (i-4)) & 0x10);


            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTC |= 0x22;
        }
        // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
        PORTC |= 0x44;
        // clears all lines in preparation of a new transmission
        PORTC = 0x00;
}


void updateField() {
    regX = posX/8;
    regY = posY/8;
}


void closeGaps (unsigned char * stack) {
    for (unsigned char i = 0; i<8; i++) {
            if(stack[i]==0)
                    for (unsigned char j = 1; j+i < 8; j++) {
                            if (stack[i+j]!=0) {
                                    stack[i] = stack[j];
                                    stack[j] = 0;
                                    j = 8;
                            }
                    }
    }
    
}


void addTone (int freq) {
    int i;
    for (i = 0; i<8&&tones[posX][i]>0; i++)
    {}
    tones[posX][i] = freq;
}


void removeTone (int freq) {
    int i;
    for (i = 0; i<8&&tones[posX][i]!=freq; i++)
    {}
    tones[posX][i] = 0;
    closeGaps (tones[posX]);
}


void reset() {
    mode = 1;
    posX = 0;
    posY = 31;
    updateField();
    
    for(unsigned char i = 0; i<31; i++) {
            speakCount[i] = 0;
            for(unsigned char j = 0; j<8; j++)
                    tones[i][j] = 0;
            for(unsigned char j = 0; j<4; j++)
                    NoteBuf[j][i] = 0;
    }
}
//--------User defined FSMs---------------------------------------------------


enum SM1_States {INIT, IDLE, DOWN, HOLD, UP, SHORT_WAIT};
    
enum SM3_States {MODE_INIT, COMP, COMP_DOWN, PLAY, PLAY_DOWN};


//Joystick
int SMTick1 (int state) {
    static short strength;
    static char Hwait = 0;
    static char Vwait = 0;
    
    if (mode == COMP) {
            Set_A2D_Pin(0);
            strength = ADC;
            strength -= 510;
            Hwait++;


            if (strength>15&&posX<measures*8-1) {
                    if (strength<170) {
                            if (Hwait>=10) {
                                    posX++;
                                    Hwait = 0;
                            }
                    }
                    else if (strength<400) {
                            if (Hwait>=5) {
                                    posX++;
                                    Hwait = 0;    
                            }
                    }
                    else
                            posX++;
                    regX = posX/8;
                    regY = posY/8;
            }
            else if (strength<-15&&posX>0) {
                    if (strength>-170) {
                            if (Hwait>=10) {
                                    posX--;
                                    Hwait = 0;
                            }
                    }
                    else if (strength>-400) {
                            if (Hwait>=5) {
                                    posX--;
                                    Hwait = 0;    
                            }
                    }
                    else
                            posX--;
                    regX = posX/8;
                    regY = posY/8;
            }


            Set_A2D_Pin(1);    
            strength = ADC;
            strength -= 510;
            Vwait++;


            if (strength>15&&posY<(height-1)) {
                    if (strength<170) {
                            if (Vwait>=10) {
                                    posY++;
                                    Vwait = 0;
                            }
                    }
                    else if (strength<400) {
                            if (Vwait>=5) {
                                    posY++;
                                    Vwait = 0;    
                            }
                    }
                    else
                            posY++;
                    updateField();
            }
            else if (strength<-15&&posY>0) {
                    if (strength>-170) {
                            if (Vwait>=10) {
                                    posY--;
                                    Vwait = 0;
                            }
                    }
                    else if (strength>-400) {
                            if (Vwait>=5) {
                                    posY--;
                                    Vwait = 0;    
                            }
                    }
                    else
                            posY--;
                    updateField();
            }
    }
 
    return state;
}


//Composer Button
int SMTick2 (int state) {    
    unsigned char tempA = ~PINA;
    
    if (mode == COMP) {
            switch(state) {
                    case INIT:
                            state = IDLE;
                            break;
                    case IDLE:
                            if(tempA&0x10)
                                    state = DOWN;
                            break;
                    case DOWN:
                            state = HOLD;
                            break;
                    case HOLD:
                            if(tempA&0x10)
                                    state = HOLD;
                            else
                                    state = IDLE;
                            break;
                    default:
                            state = INIT;
                            break;
            }
    
            switch(state) {
                    case INIT:
                            break;
                    case IDLE:
                            break;
                    case DOWN:
                            if((NoteBuf[regY][posX])&(0x01<<(posY%8))) {
                                    removeTone(frequencies[posY]);
                                    speakCount[posX]--;
                                    NoteBuf[regY][posX] &= ~(0x01<<posY%8);
                            }
                            else {
                                    if(speakCount[posX]<8) {
                                            addTone(frequencies[posY]);
                                            speakCount[posX]++;
                                            NoteBuf[regY][posX] |= (0x01<<posY%8);
                                    }
                            }
                            break;
                    case HOLD:
                            break;
                    default:
                            state = INIT;
                            break;
            }
    }


    return state;
}


//Mode Button
int SMTick3 (int state) {
    unsigned char tempA = ~PINA;


    switch (state) {
            case MODE_INIT:
                    mode = COMP;
                    state = COMP;
                    break;
            case COMP:
                    if(tempA&0x04)
                            state = PLAY_DOWN;
                    break;
            case COMP_DOWN:
                    if(!(tempA&0x04))
                            state = COMP;
                    break;
            case PLAY:
                    if(tempA&0x04)
                            state = COMP_DOWN;
                    break;
            case PLAY_DOWN:
                    if(!(tempA&0x04))
                            state = PLAY;
                    break;
            default:
                    state = INIT;
                    break;
    }
    
    switch (state) {
            case MODE_INIT:
                    state = COMP;
                    break;
            case COMP:
                    break;
            case COMP_DOWN:
                    mode = COMP;
                    break;
            case PLAY:
                    break;
            case PLAY_DOWN:
                    mode = PLAY;
                    break;
            default:
                    state = INIT;
                    break;
    }
    
    return state;
}


//Play State
int SMTick4 (int state) {
    static unsigned char barTime = 0;
            
    if(mode==PLAY) {
            if(barTime>=tempo) {
                    playPos = (playPos<measures*8-1) ? playPos+1 : 0;
                    playRegX = playPos/8;
                    barTime = 0;
            }
            else
                    barTime++;
    }
    else {
            barTime = 0;
            playPos = posX;
    }
    
    return state;
}


//Screen Print
int SMTick5 (int state) {
    screenD = ~(1<<col);
    
    if (mode == COMP) {
            if(!zoom) {
                    screenLine = NoteBuf[regY][(regX*8)+col];
                    if (col==posX%8)
                            UI = 1<<(posY%8);
                    else
                            UI = 0;
            }
            else {
                    if (col<measures)
                            screenLine = 0x3C;
                    else
                            screenLine = 0x00;
                    if (col==regX) {
                            UI = 0x01<<(2+regY);
                            screenLine &= ~UI;
                    }
                    else
                            UI = 0x00;
                    
            }
    }
    else if (mode == PLAY) {
            screenLine = NoteBuf[regY][(playRegX*8)+col];
            if (col==playPos%8)
                    UI = 0xFF;
            else
                    UI = 0;
    }
            
        col = (col>7) ? 0 : col+1;
        transmit_data(screenLine, UI);
        PORTD = screenD;
    
    return state;
}


//Measures
int SMTick6 (int state) {
    if (mode == COMP) {
            unsigned char tempA = ~PINA;
    
            switch (state) {
                    case INIT:
                            state = IDLE;
                            measures = 1;
                            break;
                    case IDLE:
                            if (tempA&0x08)
                                    state = DOWN;
                            break;
                    case DOWN:
                            state = HOLD;
                            break;
                    case HOLD:
                            if (!(tempA&0x08))
                                    state = IDLE;
                            break;
                    default:
                            state = INIT;
            }
    
            switch (state) {
                    case INIT:
                            break;
                    case IDLE:
                            break;
                    case DOWN:
                            measures = measures<4 ? measures + 1 : 1;
                            if (posX>measures*8-1) {
                                    posX=measures*8-1;
                                    updateField();    
                            }
                            break;
                    case HOLD:
                            break;
                    default:
                            state = INIT;
            }
    }


    return state;
}


//Tempo
int SMTick7 (int state) {
            unsigned char tempA = ~PINA;
    
            switch (state) {
                    case INIT:
                            state = IDLE;
                             tempo = 2;
                            break;
                    case IDLE:
                            if (tempA&0x20)
                                    state = DOWN;
                            break;
                    case DOWN:
                            state = HOLD;
                            break;
                    case HOLD:
                            if (!(tempA&0x20))
                                    state = IDLE;
                            break;
                    default:
                            state = INIT;
            }
    
            switch (state) {
                    case INIT:
                            break;
                    case IDLE:
                            break;
                    case DOWN:
                            tempo = tempo < 3 ? tempo + 1 : 1;
                            break;
                    case HOLD:
                            break;
                    default:
                            state = INIT;
            }


    return state;
}


//Speakers
int SMTick8 (int state) {
    static unsigned char spt[8];
    static unsigned char tempBit;
    static unsigned char freq;
    
    if (mode == PLAY) {
            for (int i = 0; i<speakCount[playPos]; i++) {
                    freq = tones[playPos][i];
                    if(spt[i]>=freq&&freq>0) {
                            tempBit = 0x01<<i;
                            PORTB = PINB&(tempBit) ? PINB & (~tempBit) : PINB | tempBit;
                            spt[i] = 0;
                    }
                    else
                            spt[i]++;
            }
    }
    
    return state;
}


//Zoom mode
int SMTick9 (int state) {
    unsigned char tempA = ~PINA;
    
    if (mode == COMP) {
            switch (state) {
                    case INIT:
                            state = IDLE;
                            zoom = 0;
                            break;
                    case IDLE:
                            if(tempA&0x40)
                                    state = HOLD;
                            break;
                    case HOLD:
                            if(!(tempA&0x40))
                                    state = IDLE;
                            break;
                    default:
                            state = IDLE;
            }
            switch (state) {
                    case INIT:
                            break;
                    case IDLE:
                            zoom = 0;
                            break;
                    case HOLD:
                            zoom = 1;
                            break;
                    default:
                            state = IDLE;
            }
    }
    
    return state;
}


//Reset
int SMTick10 (int state) {
            unsigned char tempA = ~PINA;
    
            switch (state) {
                    case IDLE:
                            if (tempA&0x80)
                                    state = DOWN;
                            break;
                    case DOWN:
                            state = HOLD;
                            break;
                    case HOLD:
                            if (!(tempA&0x80))
                                    state = IDLE;
                            break;
                    default:
                            state = IDLE;
            }
    
            switch (state) {
                    case IDLE:
                            break;
                    case DOWN:
                            reset();
                            break;
                    case HOLD:
                            break;
                    default:
                            state = IDLE;
            }


    return state;
}


// --------END User defined FSMs-----------------------------------------------


// Implement scheduler code from PES.
int main()
{
    // Set Data Direction Registers
    DDRA = 0x00; PORTA = 0xFC;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;


    // Period for the tasks
    unsigned long int SMTick1_calc = 500;
    unsigned long int SMTick2_calc = 300;
    unsigned long int SMTick3_calc = 300;
    unsigned long int SMTick4_calc = 500;
    unsigned long int SMTick5_calc = 12;
    unsigned long int SMTick6_calc = 300;
    unsigned long int SMTick7_calc = 300;
    unsigned long int SMTick8_calc = 1;
    unsigned long int SMTick9_calc = 300;
    unsigned long int SMTick10_calc = 300;


    //Calculating GCD
    unsigned long int tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
    tmpGCD = findGCD(tmpGCD, SMTick3_calc);
    tmpGCD = findGCD(tmpGCD, SMTick4_calc);
    tmpGCD = findGCD(tmpGCD, SMTick5_calc);
    tmpGCD = findGCD(tmpGCD, SMTick6_calc);
    tmpGCD = findGCD(tmpGCD, SMTick7_calc);
    tmpGCD = findGCD(tmpGCD, SMTick8_calc);
    tmpGCD = findGCD(tmpGCD, SMTick9_calc);
    tmpGCD = findGCD(tmpGCD, SMTick10_calc);


    //Greatest common divisor for all tasks or smallest time unit for tasks.
    unsigned long int GCD = tmpGCD;


    //Recalculate GCD periods for scheduler
    unsigned long int SMTick1_period = SMTick1_calc/GCD;
    unsigned long int SMTick2_period = SMTick2_calc/GCD;
    unsigned long int SMTick3_period = SMTick2_calc/GCD;
    unsigned long int SMTick4_period = SMTick4_calc/GCD;
    unsigned long int SMTick5_period = SMTick5_calc/GCD;
    unsigned long int SMTick6_period = SMTick6_calc/GCD;
    unsigned long int SMTick7_period = SMTick7_calc/GCD;
    unsigned long int SMTick8_period = SMTick8_calc/GCD;
    unsigned long int SMTick9_period = SMTick9_calc/GCD;
    unsigned long int SMTick10_period = SMTick10_calc/GCD;


    //Declare an array of tasks
    static task task1, task2, task3, task4, task5, task6, task7, task8, task9, task10;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8, &task9, &task10};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);


    // Task 1
    task1.state = INIT;//Task initial state.
    task1.period = SMTick1_period;//Task Period.
    task1.elapsedTime = SMTick1_period;//Task current elapsed time.
    task1.TickFct = &SMTick1;//Function pointer for the tick.
    
    task2.state = INIT;//Task initial state.
    task2.period = SMTick2_period;//Task Period.
    task2.elapsedTime = SMTick2_period;//Task current elapsed time.
    task2.TickFct = &SMTick2;//Function pointer for the tick.
    
    task3.state = MODE_INIT;//Task initial state.
    task3.period = SMTick3_period;//Task Period.
    task3.elapsedTime = SMTick3_period;//Task current elapsed time.
    task3.TickFct = &SMTick3;//Function pointer for the tick.
    
    task4.state = INIT;//Task initial state.
    task4.period = SMTick4_period;//Task Period.
    task4.elapsedTime = SMTick4_period;//Task current elapsed time.
    task4.TickFct = &SMTick4;//Function pointer for the tick.
    
    task5.state = INIT;//Task initial state.
    task5.period = SMTick5_period;//Task Period.
    task5.elapsedTime = SMTick5_period;//Task current elapsed time.
    task5.TickFct = &SMTick5;//Function pointer for the tick.
    
    task6.state = INIT;//Task initial state.
    task6.period = SMTick6_period;//Task Period.
    task6.elapsedTime = SMTick6_period;//Task current elapsed time.
    task6.TickFct = &SMTick6;//Function pointer for the tick.
    
    task7.state = INIT;//Task initial state.
    task7.period = SMTick7_period;//Task Period.
    task7.elapsedTime = SMTick7_period;//Task current elapsed time.
    task7.TickFct = &SMTick7;//Function pointer for the tick.
    
    task8.state = INIT;//Task initial state.
    task8.period = SMTick8_period;//Task Period.
    task8.elapsedTime = SMTick8_period;//Task current elapsed time.
    task8.TickFct = &SMTick8;//Function pointer for the tick.
    
    task9.state = INIT;//Task initial state.
    task9.period = SMTick9_period;//Task Period.
    task9.elapsedTime = SMTick9_period;//Task current elapsed time.
    task9.TickFct = &SMTick9;//Function pointer for the tick.
    
    task10.state = IDLE;//Task initial state.
    task10.period = SMTick10_period;//Task Period.
    task10.elapsedTime = SMTick10_period;//Task current elapsed time.
    task10.TickFct = &SMTick10;//Function pointer for the tick.


    // Initialize Global Variables
    
    col = 0;
    
    for(unsigned char i = 0; i<32; i++) {
            frequencies[i] = 32-i;
    }
    frequencies[7] = 17;
    frequencies[6] = 19;
    frequencies[5] = 20;
    frequencies[4] = 23;
    frequencies[3] = 25;
    frequencies[2] = 29;
    frequencies[1] = 30;
    frequencies[0] = 34;
    
    reset();


    ADC_init();
    // Set the timer and turn it on


    TimerOn();
    TimerSet(GCD);


    unsigned short i; // Scheduler for-loop iterator
    while(1) {
            // Scheduler code
            for ( i = 0; i < numTasks; i++ ) {
                    // Task is ready to tick
                    if ( tasks[i]->elapsedTime == tasks[i]->period ) {
                            // Setting next state for task
                            tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                            // Reset the elapsed time for next tick.
                            tasks[i]->elapsedTime = 0;
                    }
                    tasks[i]->elapsedTime += 1;
            }
            while(!TimerFlag);
            TimerFlag = 0;
    }


    // Error: Program should not exit!
    return 0;
}