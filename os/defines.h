#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#define NULL ((void *)0)
#define SERIAL_DEFAULT_DEVICE 1
#define TMR_DEFAULT_DEVICE 0
#define TMR_SECOND_DEVICE 1
#define TMR_32LS_DEVICE 2
#define TMR_32MS_DEVICE 3
#define F_PBCLK 	48000000

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;

typedef uint32 kz_thread_id_t;
typedef int (*kz_func_t)(int argc, char *argv[]);
typedef void (*kz_handler_t)(void);

typedef enum {
  MSGBOX_ID_CONSINPUT = 0,
  MSGBOX_ID_CONSOUTPUT,
  MSGBOX_ID_TMRDRV,
  MSGBOX_ID_NUM
} kz_msgbox_id_t;

#endif
