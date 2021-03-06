//
//	spi.h
//	SPI support for the AVR AT90CAN128 micro-controller.
//
//	Michael Jean <michael.jean@shaw.ca>
//	John Hughes <jondo2010@gmail.com> (Small additions)

#ifndef _SPI_H
#define _SPI_H

//
//	Each possible slave has a `slave descriptor' that enables the SPI
//	system to control the slave select lines. Each slave also requires
//	a different amount of delay before/after altering the slave
//	select line.
//
//	N.B. We assume that the slave pins are already configured as outputs.
//

#define SPI_MODE_0	0x00
#define SPI_MODE_1	(_BV (CPHA))
#define SPI_MODE_2	(_BV (CPOL))
#define SPI_MODE_3	(_BV (CPHA) | _BV (CPOL))

typedef struct spi_slave_desc_t
{
	volatile 	uint8_t		*port;				// slave select port
				uint8_t 	pin;				// slave select pin
				uint8_t		spi_mode;			// SPI mode for bit timing, valid 0..3
				double		select_delay;		// slave select delay (us)
				double		deselect_delay;		// slave deselect delay (us)
}
spi_slave_desc_t;

//
//	Initialize the SPI subsystem. If double_rate is > 0, runs the SPI clock at
//  fclk/2 = 8Mhz, otherwise the SPI clock is run at fclk/4 = 4Mhz. Initializes
//	all of the SPI pin directions. Does not automatically setup the slave select
//	pin directions.
//

void
spi_init
(
	uint8_t	double_rate
);

//
//	Setup a slave. There are a maximum of eight slave ids, and `slave_id'
//	is valid for the range 0-7. The slave descriptor provided is copied in
//	to the local array of descriptors. You are still required to setup the
//	slave select pin directions (DDRx). Automatically pulls the slave select
//	line high.
//

void
spi_setup_slave
(
	uint8_t 		 	slave_id,
	spi_slave_desc_t 	*slave_desc
);

//
//	Pull the slave select line low for the slave with id `slave_id',
//	and delay for the amount of microseconds defined in the slave
//	descriptor.
//

void
spi_slave_select
(
	uint8_t slave_id
);

//
//	Release the slave select line for the slave with id `slave_id',
//	and delay for the amount of microseconds defined in the slave
//	descriptor.
//

void
spi_slave_deselect
(
	uint8_t slave_id
);

//
//	Write a single byte to the slave. Returns whatever was shifted
//	in by the slave into the data register.
//

uint8_t
spi_putch
(
	uint8_t byte
);

//
//	Write an array of `n' bytes to the slave from the array pointed
//	to by `dst'.
//

void
spi_puts
(
	uint8_t *src,
	uint8_t	n
);

//
//	Read a single byte from the slave and return it.
//

uint8_t
spi_getch (void);

//
//	Read an array of `n' bytes from the slave to the array pointed
//	to by `dst'.
//

void
spi_gets
(
	uint8_t *dst,
	uint8_t	n
);

#endif /* _SPI_H */
