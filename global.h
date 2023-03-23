#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define ARDUINO 1
#define ARDUINO_UNO 1
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#endif // GLOBAL_H_INCLUDED
