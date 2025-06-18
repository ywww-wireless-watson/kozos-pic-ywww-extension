#include "defines.h"
#include "kozos.h"
#include "interrupt.h"
#include "lib.h"
#include "kozuser.h"
#include "hardware.h"

/* システム・タスクとユーザ・タスクの起動 */
static int start_threads(int argc, char *argv[])
{
  kz_run(consdrv_main, "consdrv",  1, 0x400, 0, NULL);
  kz_run(tmrdrv_main, "tmrdrv",  2, 0x400, 0, NULL);
  kz_run(periodic_main, "periodic",  0, 0x400, 0, NULL);
  kz_run(command_main, "command",  8, 0x400, 0, NULL);

  kz_chpri(15); /* 優先順位を下げて，アイドルスレッドに移行する */
  INTR_ENABLE; /* 割込み有効にする */
  while (1) {
    asm volatile ("wait"); /* 省電力モードに移行 */
  }

  return 0;
}

static int init(void)
{
  extern int _bss_start, _ebss;

  /* Init BSS rigion */
  memset(&_bss_start, 0, (uint32)&_ebss - (uint32)&_bss_start);

  system_init();
  timer_32bit_init(); /* 時刻計測用タイマの初期化 */

  return 0;
}

int main(void)
{
  INTR_DISABLE; /* 割込み無効にする */

  init();

  puts("kozos boot succeed!\n");

  /* OSの動作開始 */
  kz_start(start_threads, "idle", 0, 0x200, 0, NULL);
  /* ここには戻ってこない */

  return 0;
}
