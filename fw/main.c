#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>

/* L: 60usec / R: 120usec */
enum { RIGHT, LEFT = 0x10 };

/* PWM: with prescaler 1, timer ticks are 1us each, so in inverted FastPWM mode,
 * for a pulse of 60us we need a comparison value of simply 256 - 60.
 */
#define PWM_OCR_L	(256 - 60)
#define PWM_OCR_R	(256 - 120)

uint8_t cur_eye, next_eye;

int main(void)
{
	/* B0: PWM output, B4: stereo input */

	DDRB = 0x07;	/* B[0,2] outputs, B[3,5] inputs */
	PORTB = 0xfd;	/* disable pullups on B1 */

	power_timer0_enable();

	/* PWM goes to pin 5 (OC0A), Fast PWM, inverted (high pulses) */
	TCCR0A = 0xc3;	/* COM0A0 | COM0A1 | WGM00 | WGM01 */
	/* clock prescaler: 1 */
	TCCR0B = 1;		/* CS00 */
	OCR0A = PWM_OCR_L;

	for(;;) {
		next_eye = PINB & 0x10;
		if(next_eye != cur_eye) {
			OCR0A = next_eye == RIGHT ? PWM_OCR_R : PWM_OCR_L;
			cur_eye = next_eye;
		}
	}

	return 0;
}
