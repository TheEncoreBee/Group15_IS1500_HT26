/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int timeoutcount=0; //Timeout counter

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause)
{}

/* Add your code here for initializing interrupts. */
void labinit(void)//add code to initialize the timer for time-outs every 100 ms (that is, 10 time-outs per second). Remember that the DTEK-V board frequency is 30 MHz.
{
    volatile unsigned short *PERLO = (unsigned short*) 0x04000028;
    volatile unsigned short *PERHI = (unsigned short*) 0x0400002c;

    *PERLO=((3000000-1) & 0xFFFF); //Set times
    *PERHI=((3000000-1) >> 16); //Eller den andra varianten i exercisen. Fast den här verkar mer rätt.

    volatile unsigned short *CTRL = (unsigned short*) 0x04000024; //Start
    *(CTRL) = 0x4;
}

void set_leds(int led_mask){
    volatile int* LED = (volatile int*) 0x04000000;
    *LED=led_mask;
}

void set_displays(int display_number, int value){
    volatile int* display_pointer=(volatile int*)0x04000050;
    int segment_patterns[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16};
    *(display_pointer+(display_number-1)*4)=segment_patterns[value];
}

int get_sw(void){
    //0x04000010
    /*the ten least significant bits of the return value should contain data from
    the switches. SW1 corresponds to the least significant bit. All other bits of the return
    value must be zero
    */
    volatile int* switch_ptr=(volatile int*)0x04000010;
    int sw_status=1023 & (*switch_ptr);
    //print_dec(sw_status);
    //print("LOL");
    return sw_status;
}
int get_btn(void){
    volatile int* push_button_ptr=(volatile int*)0x040000d0;
    int push_button_status=1 & (*push_button_ptr);
    return push_button_status;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();
  volatile unsigned short *STAT = (unsigned short*) 0x04000020;
  set_leds(0);
  int t = 0;
  while(t<=15){
    if((*STAT) & 0x1){ //Test time-out event flag
        (*STAT) = 0; //Reset time-out bit.
        if(timeoutcount==10){
            time2string( textstring, mytime ); // Converts mytime to string
            display_string( textstring ); //Print out the string 'textstring'
            set_leds(t);
            timeoutcount=0;
            tick( &mytime ); // Ticks the clock once
            t=t+1;
        }
        timeoutcount=timeoutcount+1;
        labinit();



  }
  }
  //t=0; if we want to reset it

  mytime=(mytime & 0) | (t%10) | ((t/10)<<4) ; //Maybe not?

  int switch_state;


  while (1) { // Enter a forever loop

    if((*STAT) & 0x1){ //Test time-out event flag
        (*STAT) = 0; //Reset time-out bit.
        if(timeoutcount==10){
            time2string( textstring, mytime ); // Converts mytime to string
            display_string( textstring ); //Print out the string 'textstring'

            set_displays(1, mytime & 0x00000f);
            set_displays(2, (mytime & 0x0000f0)>>4);
            set_displays(3, (mytime & 0x000f00)>>8);
            set_displays(4, (mytime & 0x00f000)>>12);
            set_displays(5, (mytime & 0x0f0000)>>16);
            set_displays(6, (mytime & 0xf00000)>>20);

            timeoutcount=0;
            tick( &mytime );// Ticks the clock once
        }
        timeoutcount=timeoutcount+1;
        labinit();
    }
    if(get_btn()){ //Button functionailty
        switch_state=get_sw();
        if((256 & switch_state) && !(512 & switch_state)){
            //display_string("Seconds/n")63;

            mytime=(mytime & 0xffff00) | ((switch_state & 63)%10) | (((switch_state & 63)/10)<<4);
            //mytime=mytime-1; //Change only certain bytes.
            set_displays(1, mytime & 0x00000f);
            set_displays(2, (mytime & 0x0000f0)>>4);

            //set_displays(2,((switch_state & 63)/10));//Ten-digit of seconds
            //set_displays(1, ((switch_state & 63)%10));// Single-digit of seconds
        }
        else if((512 & switch_state) && !(256 & switch_state)){

            mytime=(mytime & 0xff00ff) | (((switch_state & 63)%10)<<8) | (((switch_state & 63)/10)<<12);
            //mytime=(mytime-0x100);
            set_displays(3, (mytime & 0x000f00)>>8);
            set_displays(4, (mytime & 0x00f000)>>12);

            //set_displays(4,((switch_state & 63)/10));//Ten-digit of minutes
            //set_displays(3, ((switch_state & 63)%10));//Single-digit of seconds.
        }
        else if(768<=switch_state){

            mytime=(mytime & 0x00ffff) | (((switch_state & 63)%10)<<16) | (((switch_state & 63)/10)<<20);
            set_displays(5, (mytime & 0x0f0000)>>16);
            set_displays(6, (mytime & 0xf00000)>>20);


            //set_displays(6,((switch_state & 63)/10));//Ten-digit of hours
            //set_displays(5, ((switch_state & 63)%10)); //Single-digit of hours
        }
        if(128 & switch_state){
            break;
        }
    }

}
}
