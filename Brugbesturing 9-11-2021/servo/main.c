#define F_CPU	16000000ul
#include <util/delay.h>

#include "servo.h"

void init(void)
{
	init_servo();
}

int main(void)
{
	init ();
	while (1)
	{
		for (int i = -100 ; i <= 100 ; i+=10)
		{
			servo1_set_percentage(i);
			_delay_ms (100);
		}
	}
	return 0;
}
