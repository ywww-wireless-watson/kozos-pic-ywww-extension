#ifndef _XMODEM_H_INCLUDED_
#define _XMODEM_H_INCLUDED_

long xmodem_streaming(int (*program_decode)(char *data_buf, int size));

#endif
