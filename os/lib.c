#include "defines.h"
#include "serial.h"
#include "lib.h"

void *memset(void *b, int c, long len)
{
  char *p;
  for (p = b; len > 0; len--)
    *(p++) = c;
  return b;
}

void *memcpy(void *dst, const void *src, long len)
{
  char *d = dst;
  const char *s = src;
  for (; len > 0; len--)
    *(d++) = *(s++);
  return dst;
}

int memcmp(const void *b1, const void *b2, long len)
{
  const char *p1 = b1, *p2 = b2;
  for (; len > 0; len--) {
    if (*p1 != *p2)
      return (*p1 > *p2) ? 1 : -1;
    p1++;
    p2++;
  }
  return 0;
}

int strlen(const char *s)
{
  int len;
  for (len = 0; *s; s++, len++)
    ;
  return len;
}

char *strcpy(char *dst, const char *src)
{
  char *d = dst;
  for (;; dst++, src++) {
    *dst = *src;
    if (!*src) break;
  }
  return d;
}

int strcmp(const char *s1, const char *s2)
{
  while (*s1 || *s2) {
    if (*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, int len)
{
  while ((*s1 || *s2) && (len > 0)) {
    if (*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
    len--;
  }
  return 0;
}

/* １文字送信 */
int putc(unsigned char c)
{
  if (c == '\n')
    serial_send_byte(SERIAL_DEFAULT_DEVICE, '\r');
  return serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

/* １文字受信 */
unsigned char getc(void)
{
  unsigned char c = serial_recv_byte(SERIAL_DEFAULT_DEVICE);
  c = (c == '\r') ? '\n' : c;
  putc(c); /* エコー・バック */
  return c;
}

/* 文字列送信 */
int puts(unsigned char *str)
{
  while (*str)
    putc(*(str++));
  return 0;
}

/* 文字列受信 */
int gets(unsigned char *buf)
{
  int i = 0;
  unsigned char c;
  do {
    c = getc();
    if (c == '\n')
      c = '\0';
    buf[i++] = c;
  } while (c);
  return i - 1;
}

/* 数値の16進表示 */
int putxval(unsigned long value, int column)
{
  char buf[9];
  char *p;

  p = buf + sizeof(buf) - 1;
  *(p--) = '\0';

  if (!value && !column)
    column++;

  while (value || column) {
    *(p--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    if (column) column--;
  }
  
  puts("0x");
  puts(p + 1);

  return 0;
}

/* 数値の10進表示 */
int putdval(unsigned long value, int column)
{
  char buf[9];
  char *p;

  p = buf + sizeof(buf) - 1;
  *(p--) = '\0';

  if (!value && !column)
    column++;
  
  while (value || column) {
    *(p--) = "0123456789"[value % 10];
    value /= 10;
    if (column) column--;
  }

  puts(p + 1);

  return 0;
}

/* 数値を16進文字列に変換 */
char *xvaltos(unsigned long value, int column)
{
  static char buf[9]; /* 十分なサイズを確保 (符号なし32ビット整数の最大桁数+終端文字) */
  char *p;

  p = buf + sizeof(buf) - 1;
  *p = '\0';

  if (!value && !column)
    column++;

  while (value || column) {
    *(--p) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    if (column) column--;
  }

  return p;
}

/* 数値を10進文字列に変換 */
char *dvaltos(unsigned long value, int column)
{
  static char buf[12]; /* 十分なサイズを確保 (符号なし32ビット整数の最大桁数+終端文字) */
  char *p;

  p = buf + sizeof(buf) - 1;
  *p = '\0';

  if (!value && !column)
    column++;

  while (value || column) {
    *(--p) = "0123456789"[value % 10];
    value /= 10;
    if (column) column--;
  }

  return p;
}
