#ifndef _ihex_H_INCLUDED_
#define _ihex_H_INCLUDED_

void ihex_init(void);
int ihex_decode(char *data_buf, int size);
uint32 *ihex_startaddr(void);

#define IHEX_ERR_STARTCHAR -100
#define IHEX_ERR_CHECKSUM -101
#define IHEX_ERR_RECORDTYPE -102

#endif
