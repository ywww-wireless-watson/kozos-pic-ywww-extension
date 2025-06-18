#ifndef _TMRDRV_H_INCLUDED_
#define _TMRDRV_H_INCLUDED_

void tmr_sleep(int index, int msec);
void tmr_interval(int index, int msec);
unsigned int get_current_time();
unsigned int get_offset();
void tmrdrv_32bit_init();

#endif
