#include "defines.h"
#include "kozos.h"
#include "consdrv.h"
#include "tmrdrv.h"
#include "lib.h"
#include "kozuser.h"

/* コンソール・ドライバの使用開始をコンソール・ドライバに依頼する */
static void send_use(int index)
{
  char *p;
  p = kz_kmalloc(3);
  p[0] = '0';
  p[1] = CONSDRV_CMD_USE;
  p[2] = '0' + index;
  kz_send(MSGBOX_ID_CONSOUTPUT, 3, p);
}

/* コンソールへの文字列出力をコンソール・ドライバに依頼する */
static void send_write(char *str)
{
  char *p;
  int len;
  len = strlen(str);
  p = kz_kmalloc(len + 2);
  p[0] = '0';
  p[1] = CONSDRV_CMD_WRITE;
  memcpy(&p[2], str, len);
  kz_send(MSGBOX_ID_CONSOUTPUT, len + 2, p);
}

void display_latency_counts()
{
  send_write("latency counts:\n");
  int *latency_counts = get_latency_counts();
  for (int latency = MIN_LATENCY; latency <= MAX_LATENCY; latency++)
  {
    if (latency_counts[latency - MIN_LATENCY] > 0)
    {
      send_write(dvaltos(latency, 0));
      send_write(": ");
      send_write(dvaltos(latency_counts[latency - MIN_LATENCY], 0));
      send_write("\n");
      tmr_sleep(TMR_DEFAULT_DEVICE, 10);
    }
  }
}

int command_main(int argc, char *argv[])
{
  char *p;
  int size;

  send_use(SERIAL_DEFAULT_DEVICE);

  while (1)
  {
    send_write("command> "); /* プロンプト表示 */

    /* コンソールからの受信文字列を受け取る */
    kz_recv(MSGBOX_ID_CONSINPUT, &size, &p);
    p[size] = '\0';

    if (!strncmp(p, "echo", 4))
    {                    /* echoコマンド */
      send_write(p + 4); /* echoに続く文字列を出力する */
      send_write("\n");
    }
    else if (!strncmp(p, "sleep", 5))
    {
      tmr_sleep(TMR_DEFAULT_DEVICE, 1000);
    }
    else if (!strncmp(p, "periodic", 8))
    {
      display_latency_counts();
    }
    else if (!strncmp(p, "benchmark", 9))
    {
      reset_latency_counts();
      send_write("reset latency counts.\n");
      for (int i = 0; i < 10000; i++)
      {
        send_write("benchmarking...");
        send_write(dvaltos(i, 0));
        send_write("\n");
        tmr_sleep(TMR_DEFAULT_DEVICE, 100);
      }
      send_write("benchmarking done.\n");
      display_latency_counts();
    }
    else if(!strncmp(p, "reset", 5))
    {
      reset_latency_counts();
      send_write("reset latency counts.\n");
    } 
    else
    {
      send_write("unknown.\n");
    }
    kz_kmfree(p);
  }

  return 0;
}
