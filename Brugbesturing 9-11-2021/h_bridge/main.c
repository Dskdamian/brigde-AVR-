#define F_CPU 16000000ul
#include <util/delay.h>
#include "h_bridge.h"

void init(void)
{
	init_h_bridge();
}

int main(void)
{
	init();

	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			h_bridge_set_percentage(i);
			_delay_ms(100);
		}
		for (int i = 100; i > -100; i--)
		{
			h_bridge_set_percentage(i);
			_delay_ms(100);
		}
		for (int i = -100; i < 0; i++)
		{
			h_bridge_set_percentage(i);
			_delay_ms(100);
		}
	}

	return 0;
}
