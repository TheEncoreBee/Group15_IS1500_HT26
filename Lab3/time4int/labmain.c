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
extern void enable_interrupt(); //Do you have to do this?

int timeoutcount=0; //Timeout counter
int prime = 1234567;

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

void set_displays(int display_number, int value){
    volatile int* display_pointer=(volatile int*)0x04000050;
    int segment_patterns[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16};
    *(display_pointer+(display_number-1)*4)=segment_patterns[value];
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) //According to the isr_routine in boot.S, this function is called by any external interrupt, like from the timer.
{//An exception simply occours if a timeout is reached.
    volatile unsigned short *STAT = (unsigned short*) 0x04000020;
    (*STAT) = 0;
    if(timeoutcount==10){
        set_displays(1, mytime & 0x00000f);
        set_displays(2, (mytime & 0x0000f0)>>4);
        set_displays(3, (mytime & 0x000f00)>>8);
        set_displays(4, (mytime & 0x00f000)>>12);
        set_displays(5, (mytime & 0x0f0000)>>16);
        set_displays(6, (mytime & 0xf00000)>>20);

        tick(&mytime);
        timeoutcount=0;
    }
    timeoutcount=timeoutcount+1;

    volatile unsigned short *PERLO = (unsigned short*) 0x04000028;
    volatile unsigned short *PERHI = (unsigned short*) 0x0400002c;
    *PERLO=((3000000-1) & 0xFFFF); //Set times
    *PERHI=((3000000-1) >> 16); //Eller den andra varianten i exercisen. Fast den här verkar mer rätt.

    volatile unsigned short *CTRL = (unsigned short*) 0x04000024;
    *(CTRL) = 0x5; //Start the timer and set ITO to 1.
}

/* Add your code here for initializing interrupts. */
void labinit(void)//add code to initialize the timer for time-outs every 100 ms (that is, 10 time-outs per second). Remember that the DTEK-V board frequency is 30 MHz.
{
    volatile unsigned short *PERLO = (unsigned short*) 0x04000028;
    volatile unsigned short *PERHI = (unsigned short*) 0x0400002c;

    *PERLO=((3000000-1) & 0xFFFF); //Set times
    *PERHI=((3000000-1) >> 16); //Eller den andra varianten i exercisen. Fast den här verkar mer rätt.

    volatile unsigned short *CTRL = (unsigned short*) 0x04000024;
    *(CTRL) = 0x5; //Start the timer and set ITO to 1.

    enable_interrupt(); //I assume this is resolved in the build process.
}

void set_leds(int led_mask){
    volatile int* LED = (volatile int*) 0x04000000;
    *LED=led_mask;
}



int get_sw(void){
    /*the ten least significant bits of the return value should contain data from
    the switches.
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

    labinit();
    while (1) {
    print("Prime: ");
    prime=nextprime(prime);
    print_dec(prime);
    print("\n");
    }
}
