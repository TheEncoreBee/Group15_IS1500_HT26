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


int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause)
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}



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
  set_leds(0);
  int t = 0;
  while(t<=15){
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    set_leds(t);
    delay( 1000 );// Delays 1 sec (adjust this value), 25/9now delays approximately 1 second?
    tick( &mytime ); // Ticks the clock once
    t=t+1;
  }

  mytime=(mytime & 0) | (t%10) | ((t/10)<<4) ; //Maybe not?
  //t=0; if we want to reset it
  int switch_state;

  while (1) { // Enter a forever loop
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'

    set_displays(1, mytime & 0x00000f);
    set_displays(2, (mytime & 0x0000f0)>>4);
    set_displays(3, (mytime & 0x000f00)>>8);
    set_displays(4, (mytime & 0x00f000)>>12);
    set_displays(5, (mytime & 0x0f0000)>>16);
    set_displays(6, (mytime & 0xf00000)>>20);

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
    delay( 1000 );// Delays 1 sec (adjust this value), 25/9now delays approximately 1 second?
    tick( &mytime );// Ticks the clock once

  }
}
