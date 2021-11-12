#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "servo.h"
#include "h_bridge.h"

#define LEDPIN_GeelBrug1     PC7// pin 30_
#define LEDPIN_GeelBrug2     PC6// pin 31_
#define LEDPIN_RoodBlink1    PC2// pin 35_
#define LEDPIN_RoodBlink2    PC3// pin 34_
#define LEDPIN_GroenBoot1R   PC5// pin 32 *
#define LEDPIN_GroenBoot1L   PC4// pin 33
#define LEDPIN_GroenBoot1B   PC1// pin 36
#define LEDPIN_RoodBoot1R    PC0// pin 37
#define LEDPIN_RoodBoot1L    PD7// pin 38
#define LEDPIN_RoodBoot1B    PG2// pin 39 *

#define LEDPIN_RoodOpen      PA6// pin 28_
#define LEDPIN_RoodDicht     PA7// pin 29_
#define LEDPIN_BlauwOpenen   PA0// pin 22_
#define LEDPIN_BlauwSluiten  PA1// pin 23_
#define LEDPIN_GeelPersoon   PB2// pin 51_
#define LEDPIN_GeelBoot      PB3// pin 50_

#define Ultrasoon_Boot1     PB1// pin 52
#define Ultrasoon_Boot2     PL7// pin 42
#define Ultrasoon_Persoon1  PG2// pin 39
#define Ultrasoon_Persoon2  PG0// pin 41

#define Eindschakelaar_Open  PL2// pin 48 *
#define Eindschakelaar_Dicht PL1// pin 47 *
#define Button_Openen        PL6// pin 43
#define Button_Sluiten       PL5// pin 44
#define Button_Noodstop      PL4// pin 45

#define Geluidssignaal       PE5// pin 3

#define led_aan              PB2// pin 53
#define windsensor           PK0// pin A8


int windmeter = 0;
int winddrempel = 100;
unsigned long millisec = 0, millisecBlinkLights = 0, millisecWindmeter = 0, millisecPushButton = 0;

void Verlichtingbrugdicht(int aanD)
{
    if (aanD == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC |= (1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD |= (1 << LEDPIN_RoodBoot1L);   //uit
    PORTG |= (1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC &= ~(1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopent1Aan(int aanO1)
{
    if (aanO1 == 1)
    {
    PORTC |= (1 << LEDPIN_GroenBoot1R); //uit
    PORTC |= (1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD |= (1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopent1Uit(int uitO1)
{
    if (uitO1 == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC &= ~(1 << LEDPIN_GeelBrug1);    //uit
    PORTC &= ~(1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopent2Aan(int aanOt2)
{
    if (aanOt2 == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC |= (1 << LEDPIN_RoodBoot1R);   //uit
    PORTC |= (1 << LEDPIN_GroenBoot1L); //uit
    PORTD |= (1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopent2Uit(int uitOt2)
{
    if (uitOt2 == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopen1Aan(int aanO1)
{
    if (aanO1 == 1)
    {
    PORTC |= (1 << LEDPIN_GroenBoot1R); //aan
    PORTC |= (1 << LEDPIN_RoodBoot1R);   //uit
    PORTC |= (1 << LEDPIN_GroenBoot1L); //aan
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //uit
    PORTC |= (1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopen1Uit(int uitO1)
{
    if (uitO1 == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //aan
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //aan
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopen2Aan(int aanO2)
{
    if (aanO2 == 1)
    {
    PORTC |= (1 << LEDPIN_GroenBoot1R); //aan
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC |= (1 << LEDPIN_GroenBoot1L); //aan
    PORTD |= (1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //uit
    PORTC |= (1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void Verlichtingbrugopen2Uit(int uitO2)
{
    if (uitO2 == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //aan
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //aan
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //aan
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void VerlichtingbrugsluitAan(int aanS)
{
    if (aanS == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC |= (1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD |= (1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //uit
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}
void VerlichtingbrugsluitUit(int uitS)
{
    if (uitS == 1)
    {
    PORTC &= ~(1 << LEDPIN_GroenBoot1R); //uit
    PORTC &= ~(1 << LEDPIN_RoodBoot1R);   //uit
    PORTC &= ~(1 << LEDPIN_GroenBoot1L); //uit
    PORTD &= ~(1 << LEDPIN_RoodBoot1L);   //uit
    PORTG &= ~(1 << LEDPIN_RoodBoot1B);  //aan
    PORTC &= ~(1 << LEDPIN_GroenBoot1B);   //uit
    PORTC |= (1 << LEDPIN_GeelBrug1);    //uit
    PORTC |= (1 << LEDPIN_GeelBrug2);    //uit
    }
}

void LedsbedieningpaneelRoodOpenAan(int roodopenaan)
{if (roodopenaan == 1){PORTA |= (1 << LEDPIN_RoodOpen);}}

void LedsbedieningpaneelRoodOpenUit(int roodopenuit)
{if (roodopenuit == 1){PORTA &= ~(1 << LEDPIN_RoodOpen);}}

void LedsbedieningpaneelRoodDichtAan(int rooddichtaan)
{if (rooddichtaan == 1){PORTA |= (1 << LEDPIN_RoodDicht);}}

void LedsbedieningpaneelRoodDichtUit(int rooddichtuit)
{if (rooddichtuit == 1){PORTA &= ~(1 << LEDPIN_RoodDicht);}}

void LedsbedieningpaneelBlauwOpenenAan(int blauwopenaan)
{if (blauwopenaan == 1){PORTA |= (1 << LEDPIN_BlauwOpenen);}}

void LedsbedieningpaneelBlauwOpenenUit(int blauwopenuit)
{if (blauwopenuit == 1){PORTA &= ~(1 << LEDPIN_BlauwOpenen);}}

void LedsbedieningpaneelBlauwSluitenAan(int blauwsluitaan)
{if (blauwsluitaan == 1){PORTA |= (1 << LEDPIN_BlauwSluiten);}}

void LedsbedieningpaneelBlauwSluitenUit(int blauwsluituit)
{if (blauwsluituit == 1){PORTA &= ~(1 << LEDPIN_BlauwSluiten);}}

void LedsbedieningpaneelGeelBootAan(int geelbootaan)
{if (geelbootaan == 1){PORTB |= (1 << LEDPIN_GeelBoot);}}

void LedsbedieningpaneelGeelBootUit(int geelbootuit)
{if (geelbootuit == 1){PORTB &= ~(1 << LEDPIN_GeelBoot);}}

void LedsbedieningpaneelGeelPersoonAan(int geelpersoonaan)
{if (geelpersoonaan == 1){PORTB |= (1 << LEDPIN_GeelPersoon);}}

void LedsbedieningpaneelGeelPersoonUit(int geelpersoonuit)
{if (geelpersoonuit == 1){PORTB &= ~(1 << LEDPIN_GeelPersoon);}}

void LedsbedieningpaneelNoodstopAan(int noodaan)
{if (noodaan == 1){PORTD |= (1 << led_aan);}}

void LedsbedieningpaneelNoodstopUit(int nooduit)
{if (nooduit == 1){PORTD &= ~(1 << led_aan);}}


void waarschuwingslichtenAan(int waaraan)
{
    if (waaraan == 1)
    {
            PORTC &= ~(1 << LEDPIN_RoodBlink1);
            PORTC |= (1 << LEDPIN_RoodBlink2);
            _delay_ms(200);
            PORTC |= (1 << LEDPIN_RoodBlink1);
            PORTC &= ~(1 << LEDPIN_RoodBlink2);
            _delay_ms(200);
    }
}
void waarschuwingslichtenUit(int waaruit)
{
    if (waaruit == 1) {PORTC |= (1 << LEDPIN_RoodBlink1);      PORTC |= (1 << LEDPIN_RoodBlink2);}}

void init()
{init_h_bridge();               init_servo();}

void Slagbomendicht(int slagdicht)
{
    if (slagdicht == 1) {servo1_set_percentage(-80);    servo2_set_percentage(-70);}}

void Slagbomenopen(int slagopen)
{
    if (slagopen == 1) {servo1_set_percentage(80);     servo2_set_percentage(70);}}

void Motoruit()
{h_bridge_set_percentage(0);}

void Brugomhoog()
{h_bridge_set_percentage(-33);}

void Brugomlaag()
{h_bridge_set_percentage(33);}

void informatie()
{       //lees windmeter uit
    if (millisec - millisecWindmeter >= 5000)
    {
    if (ADCH > winddrempel)
    {
      windmeter = 1;
      millisecWindmeter = millisec;
    }
    else
      windmeter = 0;
    }
}

int main(void)
{

    init();
    DDRC |= (1 << LEDPIN_GroenBoot1R);
    DDRC |= (1 << LEDPIN_RoodBoot1R);
    DDRC |= (1 << LEDPIN_GroenBoot1L);
    DDRD |= (1 << LEDPIN_RoodBoot1L);
    DDRG |= (1 << LEDPIN_RoodBoot1B);
    DDRC |= (1 << LEDPIN_GroenBoot1B);
    DDRC |= (1 << LEDPIN_GeelBrug1);
    DDRC |= (1 << LEDPIN_GeelBrug2);
    DDRA |= (1 << LEDPIN_BlauwOpenen);
    DDRA |= (1 << LEDPIN_BlauwSluiten);
    DDRA |= (1 << LEDPIN_RoodOpen);
    DDRA |= (1 << LEDPIN_RoodDicht);
    DDRB |= (1 << LEDPIN_GeelBoot);
    DDRB |= (1 << LEDPIN_GeelPersoon);

    PORTC |= (1 << LEDPIN_GroenBoot1R);
    PORTC |= (1 << LEDPIN_RoodBoot1R);
    PORTC |= (1 << LEDPIN_GroenBoot1L);
    PORTD |= (1 << LEDPIN_RoodBoot1L);
    PORTG |= (1 << LEDPIN_RoodBoot1B);
    PORTC |= (1 << LEDPIN_GroenBoot1B);
    PORTC |= (1 << LEDPIN_GeelBrug1);
    PORTC |= (1 << LEDPIN_GeelBrug2);
    PORTA |= (1 << LEDPIN_BlauwOpenen);
    PORTA |= (1 << LEDPIN_BlauwSluiten);
    PORTA |= (1 << LEDPIN_RoodOpen);
    PORTA |= (1 << LEDPIN_RoodDicht);
    PORTB |= (1 << LEDPIN_GeelBoot);
    PORTB |= (1 << LEDPIN_GeelPersoon);

    DDRL &= ~(1 << Button_Noodstop);
    DDRL &= ~(1 << Button_Openen);
    DDRL &= ~(1 << Button_Sluiten);
    DDRL &= ~(1 << Eindschakelaar_Open);
    DDRL &= ~(1 << Eindschakelaar_Dicht);

    DDRB &= ~(1 << Ultrasoon_Boot1);
    DDRL &= ~(1 << Ultrasoon_Boot2);
    DDRG &= ~(1 << Ultrasoon_Persoon1);
    DDRG &= ~(1 << Ultrasoon_Persoon2);

    while (1)
    {
        int KnopOpen = 0;
        int KnopDicht = 0;
        int KnopNood = 0;
        int EindOpen = 0;
        int EindDicht = 0;
        int stand = 1;
        int ultrasoonboot1 = 0;
        int ultrasoonboot2 = 0;
        int ultrasoonpersoon1 = 0;
        int ultrasoonpersoon2 = 0;
        int BrugbesturingAan = 1;

        Slagbomenopen(1);
        h_bridge_set_percentage(0);

        while ((BrugbesturingAan & 1) != 0)
        {

            if (!(PINL & (1 << Button_Openen)))
            {
                KnopOpen = 1;
            }
            if (!(PINL & (1 << Button_Sluiten)))
            {
                KnopDicht = 1;
            }
            if (!(PINL & (1 << Button_Noodstop)))
            {
                KnopNood = 1;
            }
            if (!(PINL & (1 << Eindschakelaar_Open)))
            {
                EindOpen = 1;
            }
            if (!(PINL & (1 << Eindschakelaar_Dicht)))
            {
                EindDicht = 1;
            }
            if (!(PINB & (1 << Ultrasoon_Boot1)))
            {
                ultrasoonboot1 = 1;
                ultrasoonboot2 = 0;
            }
            if (!(PINL & (1 << Ultrasoon_Boot2)))
            {
                ultrasoonboot2 = 1;
                ultrasoonboot1 = 0;
            }
            if (!(PING & (1 << Ultrasoon_Persoon1)))
            {
                ultrasoonpersoon1 = 1;
            }
            if (!(PING & (1 << Ultrasoon_Persoon2)))
            {
                ultrasoonpersoon2 = 1;
            }

            switch(stand)
            {
                case 1:
                    Slagbomenopen(1);
                    Verlichtingbrugdicht(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(0);
                    LedsbedieningpaneelBlauwSluitenUit(1);
                    LedsbedieningpaneelRoodOpenAan(0);
                    LedsbedieningpaneelRoodOpenUit(1);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(0);
                    LedsbedieningpaneelGeelBootUit(1);

                        if ((ultrasoonboot1 == 1) || (ultrasoonboot2 == 1))
                        {
                            Slagbomenopen(1);
                            Verlichtingbrugdicht(1);
                            LedsbedieningpaneelRoodDichtAan(1);
                            LedsbedieningpaneelRoodDichtUit(0);
                            LedsbedieningpaneelBlauwOpenenAan(0);
                            LedsbedieningpaneelBlauwOpenenUit(1);
                            LedsbedieningpaneelBlauwSluitenAan(0);
                            LedsbedieningpaneelBlauwSluitenUit(1);
                            LedsbedieningpaneelRoodOpenAan(0);
                            LedsbedieningpaneelRoodOpenUit(1);
                            LedsbedieningpaneelGeelPersoonAan(0);
                            LedsbedieningpaneelGeelPersoonUit(1);
                            LedsbedieningpaneelNoodstopUit(0);
                            LedsbedieningpaneelNoodstopAan(1);
                            LedsbedieningpaneelGeelBootAan(1);
                            LedsbedieningpaneelGeelBootUit(0);
                            stand = 2;
                        }
                        else
                        {
                            stand = 1;
                        }
                    break;
                case 2:
                        Slagbomenopen(1);
                        Verlichtingbrugdicht(1);
                        LedsbedieningpaneelRoodDichtAan(1);
                        LedsbedieningpaneelRoodDichtUit(0);
                        LedsbedieningpaneelBlauwOpenenAan(0);
                        LedsbedieningpaneelBlauwOpenenUit(1);
                        LedsbedieningpaneelBlauwSluitenAan(0);
                        LedsbedieningpaneelBlauwSluitenUit(1);
                        LedsbedieningpaneelRoodOpenAan(0);
                        LedsbedieningpaneelRoodOpenUit(1);
                        LedsbedieningpaneelGeelPersoonAan(0);
                        LedsbedieningpaneelGeelPersoonUit(1);
                        LedsbedieningpaneelNoodstopUit(0);
                        LedsbedieningpaneelNoodstopAan(1);
                        LedsbedieningpaneelGeelBootAan(1);
                        LedsbedieningpaneelGeelBootUit(0);
                            if ((KnopOpen == 1) && (windmeter == 0))
                            {
                                Slagbomenopen(1);
                                Verlichtingbrugopent1Aan(1);
                                waarschuwingslichtenAan(1);
                                LedsbedieningpaneelRoodDichtAan(1);
                                LedsbedieningpaneelRoodDichtUit(0);
                                LedsbedieningpaneelBlauwOpenenAan(0);
                                LedsbedieningpaneelBlauwOpenenUit(1);
                                LedsbedieningpaneelBlauwSluitenAan(1);
                                LedsbedieningpaneelBlauwSluitenUit(0);
                                LedsbedieningpaneelRoodOpenAan(0);
                                LedsbedieningpaneelRoodOpenUit(1);
                                LedsbedieningpaneelGeelPersoonAan(0);
                                LedsbedieningpaneelGeelPersoonUit(1);
                                LedsbedieningpaneelNoodstopUit(0);
                                LedsbedieningpaneelNoodstopAan(1);
                                LedsbedieningpaneelGeelBootAan(1);
                                LedsbedieningpaneelGeelBootUit(0);
                                stand = 3;
                            }
                            else
                            {
                                stand = 2;
                            }
                    break;
                case 3:
                    Slagbomenopen(1);
                    Verlichtingbrugopent1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(1);
                    LedsbedieningpaneelBlauwSluitenUit(0);
                    LedsbedieningpaneelRoodOpenAan(0);
                    LedsbedieningpaneelRoodOpenUit(1);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(1);
                    LedsbedieningpaneelGeelBootUit(0);
                        if ((ultrasoonpersoon1 == 1) || (ultrasoonpersoon2 == 1))
                        {
                            LedsbedieningpaneelGeelPersoonAan(1);
                            _delay_ms(3000);
                            ultrasoonpersoon1 = 0;
                            ultrasoonpersoon2 = 0;
                            stand = 3;
                        }
                        else
                        {
                            stand = 4;
                        }
                    break;
                case 4:
                    Slagbomenopen(1);
                    Verlichtingbrugopent1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(1);
                    LedsbedieningpaneelBlauwSluitenUit(0);
                    LedsbedieningpaneelRoodOpenAan(0);
                    LedsbedieningpaneelRoodOpenUit(1);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(1);
                    LedsbedieningpaneelGeelBootUit(0);
                    _delay_ms(2000);
                    stand = 5;
                    break;
                case 5:
                    Slagbomendicht(1);
                    Verlichtingbrugopent1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(1);
                    LedsbedieningpaneelBlauwSluitenUit(0);
                    LedsbedieningpaneelRoodOpenAan(0);
                    LedsbedieningpaneelRoodOpenUit(1);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(1);
                    LedsbedieningpaneelGeelBootUit(0);
                    stand = 6;
                    break;
                case 6:
                    Slagbomendicht(1);
                    Verlichtingbrugopent1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(1);
                    LedsbedieningpaneelBlauwSluitenUit(0);
                    LedsbedieningpaneelRoodOpenAan(0);
                    LedsbedieningpaneelRoodOpenUit(1);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(1);
                    LedsbedieningpaneelGeelBootUit(0);
                        if (EindOpen == 0)
                        {
                            Motoruit();
                            stand = 7;
                        }
                        else
                        {
                            Brugomlaag();
                            _delay_ms(2300);
                            Motoruit();
                            stand = 7;
                        }
                    break;
                case 7:
                    Slagbomendicht(1);
                    Verlichtingbrugopen1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(0);
                    LedsbedieningpaneelRoodDichtUit(1);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(1);
                    LedsbedieningpaneelBlauwSluitenUit(0);
                    LedsbedieningpaneelRoodOpenAan(1);
                    LedsbedieningpaneelRoodOpenUit(0);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(1);
                    LedsbedieningpaneelGeelBootUit(0);
                    if ((ultrasoonboot1 == 0) && (ultrasoonboot2 == 1))
                    {
                        stand = 8;
                    }
                    else
                    {
                        stand = 7;
                    }
                    break;
                case 8:
                    Slagbomendicht(1);
                    Verlichtingbrugopen1Aan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(0);
                    LedsbedieningpaneelRoodDichtUit(1);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(0);
                    LedsbedieningpaneelBlauwSluitenUit(1);
                    LedsbedieningpaneelRoodOpenAan(1);
                    LedsbedieningpaneelRoodOpenUit(0);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(0);
                    LedsbedieningpaneelGeelBootUit(1);
                        if (KnopDicht == 1)
                        {
                            Slagbomendicht(1);
                            LedsbedieningpaneelBlauwOpenenAan(1);
                            _delay_ms(3000);
                            stand = 9;
                        }
                        else
                        {
                            stand = 8;
                        }
                case 9:
                    Slagbomendicht(1);
                    VerlichtingbrugsluitAan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(0);
                    LedsbedieningpaneelRoodDichtUit(1);
                    LedsbedieningpaneelBlauwOpenenAan(1);
                    LedsbedieningpaneelBlauwOpenenUit(0);
                    LedsbedieningpaneelBlauwSluitenAan(0);
                    LedsbedieningpaneelBlauwSluitenUit(1);
                    LedsbedieningpaneelRoodOpenAan(1);
                    LedsbedieningpaneelRoodOpenUit(0);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(0);
                    LedsbedieningpaneelGeelBootUit(1);
                        if (EindDicht == 0)
                        {
                            Motoruit();
                            stand = 10;
                        }
                        else
                        {
                            Brugomlaag();
                            _delay_ms(2100);
                            Motoruit();
                            stand = 10;
                        }
                    break;
                case 10:
                    Slagbomendicht(1);
                    VerlichtingbrugsluitAan(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(1);
                    LedsbedieningpaneelBlauwOpenenUit(0);
                    LedsbedieningpaneelBlauwSluitenAan(0);
                    LedsbedieningpaneelBlauwSluitenUit(1);
                    LedsbedieningpaneelRoodOpenAan(1);
                    LedsbedieningpaneelRoodOpenUit(0);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(0);
                    LedsbedieningpaneelGeelBootUit(1);
                    _delay_ms(2000);
                    Slagbomendicht(0);
                    Slagbomenopen(1);
                    Verlichtingbrugdicht(1);
                    stand = 11;
                    break;
                case 11:
                    Slagbomenopen(1);
                    Verlichtingbrugdicht(1);
                    waarschuwingslichtenAan(1);
                    LedsbedieningpaneelRoodDichtAan(1);
                    LedsbedieningpaneelRoodDichtUit(0);
                    LedsbedieningpaneelBlauwOpenenAan(0);
                    LedsbedieningpaneelBlauwOpenenUit(1);
                    LedsbedieningpaneelBlauwSluitenAan(0);
                    LedsbedieningpaneelBlauwSluitenUit(1);
                    LedsbedieningpaneelRoodOpenAan(1);
                    LedsbedieningpaneelRoodOpenUit(0);
                    LedsbedieningpaneelGeelPersoonAan(0);
                    LedsbedieningpaneelGeelPersoonUit(1);
                    LedsbedieningpaneelNoodstopUit(0);
                    LedsbedieningpaneelNoodstopAan(1);
                    LedsbedieningpaneelGeelBootAan(0);
                    LedsbedieningpaneelGeelBootUit(1);
                    _delay_ms(2000);
                    waarschuwingslichtenUit(1);
                    KnopOpen = 0;
                    KnopDicht = 0;
                    EindOpen = 0;
                    EindDicht = 0;
                    ultrasoonboot1 = 0;
                    ultrasoonboot2 = 0;
                    ultrasoonpersoon1 = 0;
                    ultrasoonpersoon2 = 0;
                    stand = 1;
                    break;
                }
            if (!(PINL & (1 << Button_Noodstop)))
            {
                BrugbesturingAan++;
            }

        while ((BrugbesturingAan & 1) == 0)
        {
            init();
            h_bridge_set_percentage(0);
            VerlichtingbrugsluitAan(1);
            waarschuwingslichtenAan(1);
            LedsbedieningpaneelRoodDichtAan(0);
            LedsbedieningpaneelRoodDichtUit(1);
            LedsbedieningpaneelBlauwOpenenAan(0);
            LedsbedieningpaneelBlauwOpenenUit(1);
            LedsbedieningpaneelBlauwSluitenAan(0);
            LedsbedieningpaneelBlauwSluitenUit(1);
            LedsbedieningpaneelRoodOpenAan(0);
            LedsbedieningpaneelRoodOpenUit(1);
            LedsbedieningpaneelGeelPersoonAan(0);
            LedsbedieningpaneelGeelPersoonUit(1);
            LedsbedieningpaneelNoodstopUit(1);
            LedsbedieningpaneelNoodstopAan(0);
            LedsbedieningpaneelGeelBootAan(0);
            LedsbedieningpaneelGeelBootUit(1);
                if (!(PINL & (1 << Button_Noodstop)))
                {
                    BrugbesturingAan++;
                }
        }
        }
    }
    return (0);
    }
