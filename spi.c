//
//	spi.c
//	SPI support for the AVR AT90CAN128 micro-controller.
//
//	Michael Jean <michael.jean@shaw.ca>
//

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

static spi_slave_desc_t slave_descs[8];

void
spi_init
(
	uint8_t	double_rate
)
{
	DDRB |= _BV (PB2) | _BV (PB1);
	SPCR |= _BV (SPE) | _BV (MSTR);

	if (double_rate)
		SPSR |= _BV (SPI2X);
}

void
spi_setup_slave
(
	uint8_t 		 	slave_id,
	spi_slave_desc_t 	*slave_desc
)
{
	*(slave_desc->port) |= _BV (slave_desc->pin); /* 1 */

	/* 2 */
	slave_descs[slave_id].port 			 = slave_desc->port;
	slave_descs[slave_id].pin 			 = slave_desc->pin;
	slave_descs[slave_id].select_delay   = slave_desc->select_delay;
	slave_descs[slave_id].deselect_delay = slave_desc->deselect_delay;
	slave_descs[slave_id].spi_mode		 = slave_desc->spi_mode;
}

//
//	1.	We pull the line high because this is the default `de-selected' mode.
//
//	2.	We copy the struct over because we've already allocated the memory
//		locally for the eight slave descriptors, and because the descriptor
//		that is passed in may go out of scope and become freed while we're
//		still running.
//

void
spi_slave_select
(
	uint8_t slave_id
)
{
	SPCR = (SPCR & 0xF3) | slave_descs[slave_id].spi_mode; /// Set the SPI mode

	*(slave_descs[slave_id].port) &= ~_BV (slave_descs[slave_id].pin);
	_delay_us (slave_descs[slave_id].select_delay);
}

void
spi_slave_deselect
(
	uint8_t slave_id
)
{
	*(slave_descs[slave_id].port) |= _BV (slave_descs[slave_id].pin);
	_delay_us (slave_descs[slave_id].deselect_delay);
}

uint8_t
spi_putch
(
	uint8_t byte
)
{
	SPDR = byte;
	loop_until_bit_is_set (SPSR, SPIF);

	return SPDR;
}

void
spi_puts
(
	uint8_t *src,
	uint8_t	n
)
{
	while (n-- > 0)
	{
		SPDR = *(src++);
		loop_until_bit_is_set (SPSR, SPIF);
	}
}

uint8_t
spi_getch (void)
{
	SPDR = 0;
	loop_until_bit_is_set (SPSR, SPIF);
	return SPDR;
}

void
spi_gets
(
	uint8_t *dst,
	uint8_t	n
)
{
	while (n-- > 0)
	{
		SPDR = 0;
		loop_until_bit_is_set (SPSR, SPIF);
		*(dst++) = SPDR;
	}
}
