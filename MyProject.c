unsigned short mode = 0;
unsigned short street = 0;
unsigned short trafficState = 0;
unsigned short lastSelect;
unsigned short i;

void westDisplay(unsigned short value)
{
    unsigned short tens;
    unsigned short ones;

    tens = value / 10;
    ones = value % 10;

    PORTB = (PORTB & 0xE1) | ((tens & 0x0F) << 1);
    PORTC = (PORTC & 0xF0) | (ones & 0x0F);
}

void southDisplay(unsigned short value)
{
    unsigned short tens;
    unsigned short ones;

    tens = value / 10;
    ones = value % 10;

    PORTC = (PORTC & 0x0F) | ((tens & 0x0F) << 4);
    PORTD = (PORTD & 0xF0) | (ones & 0x0F);
}

void clearDisplays()
{
    PORTB = PORTB & 0xE1;

    PORTC = PORTC & 0x00;

    PORTD = PORTD & 0xF0;
}

void showTime(unsigned short westTime, unsigned short southTime)
{
    westDisplay(westTime);
    southDisplay(southTime);
}

void westGreen()
{
    PORTB.B5 = 0;
    PORTB.B6 = 0;
    PORTB.B7 = 1;

    PORTD.B4 = 1;
    PORTD.B5 = 0;
    PORTD.B6 = 0;

    street = 0;
    trafficState = 0;
}

void westYellow()
{
    PORTB.B5 = 0;
    PORTB.B6 = 1;
    PORTB.B7 = 0;

    PORTD.B4 = 1;
    PORTD.B5 = 0;
    PORTD.B6 = 0;

    trafficState = 1;
}


void southGreen()
{
    PORTB.B5 = 1;
    PORTB.B6 = 0;
    PORTB.B7 = 0;

    PORTD.B4 = 0;
    PORTD.B5 = 0;
    PORTD.B6 = 1;

    street = 1;
    trafficState = 2;
}


void southYellow()
{
    PORTB.B5 = 1;
    PORTB.B6 = 0;
    PORTB.B7 = 0;

    PORTD.B4 = 0;
    PORTD.B5 = 1;
    PORTD.B6 = 0;

    trafficState = 3;
}

void yellowCountdown()
{
    unsigned short j;

    for(j = 3; j > 0; j--)
    {
        showTime(j, j);

        Delay_ms(1000);

        if(mode == 0)
            return;
    }

    showTime(0, 0);
}

void changeStreet()
{
    if(street == 0)
    {
        westYellow();

        yellowCountdown();

        if(mode == 1)
            southGreen();
    }
    else
    {
        southYellow();

        yellowCountdown();

        if(mode == 1)
            westGreen();
    }
}

void interrupt()
{
    if(INTF_bit == 1)
    {
        mode = !mode;
        INTF_bit = 0;
    }
}

void main()
{
    ADCON1 = 6;

    TRISA = 0x00;
    TRISB = 0b00000001;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE.B1 = 1;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    PORTA.B0 = 1;
    PORTA.B1 = 1;
    PORTA.B2 = 1;
    PORTA.B3 = 1;

    GIE_bit = 1;
    INTE_bit = 1;

    INTEDG_bit = 0;
    INTF_bit = 0;

    street = 0;
    trafficState = 0;

    westGreen();

    clearDisplays();

    lastSelect = PORTE.B1;


    while(1)
    {

        if(mode == 1)
        {
            clearDisplays();

            lastSelect = PORTE.B1;

            while(mode == 1)
            {
                clearDisplays();

                if(lastSelect == 1 && PORTE.B1 == 0)
                {
                    changeStreet();

                    while(PORTE.B1 == 0)
                    {
                    }

                    Delay_ms(50);

                    clearDisplays();
                }

                lastSelect = PORTE.B1;

                Delay_ms(10);
            }
        }

        else
        {

            street = 0;
            westGreen();
 for(i = 20; i > 0 && mode == 0; i--)
            {
                showTime(i, i + 3);

                Delay_ms(1000);
            }

            if(mode == 0)
            {
                showTime(0, 3);
            }

            if(mode == 1)
                continue;

            westYellow();

            for(i = 3; i > 0 && mode == 0; i--)
            {
                showTime(i, i);

                Delay_ms(1000);
            }

            if(mode == 0)
            {
                showTime(0, 0);
            }

            if(mode == 1)
                continue;

            street = 1;
            southGreen();

            for(i = 12; i > 0 && mode == 0; i--)
            {
                showTime(i + 3, i);

                Delay_ms(1000);
            }

            if(mode == 0)
            {
                showTime(3, 0);
            }

            if(mode == 1)
                continue;


            southYellow();

            for(i = 3; i > 0 && mode == 0; i--)
            {
                showTime(i, i);

                Delay_ms(1000);
            }

            if(mode == 0)
            {
                showTime(0, 0);
            }
        }
    }
}