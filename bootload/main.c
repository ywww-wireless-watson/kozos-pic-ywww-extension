#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "lib.h"
#include "hardware.h"
#include "ihex.h"
#include "interrupt.h"
#include "intr.h"

static int init(void)
{
    extern int _erodata, _data_start, _edata, _bss_start, _ebss;
  memcpy(&_data_start, &_erodata, (long)&_edata - (long)&_data_start);
  memset(&_bss_start, 0, (long)&_ebss - (long)&_bss_start);
  
  system_init();

  /* ソフトウエア・割り込みベクタを初期化する */
  softvec_init();

  /* シリアルの初期化 */
  serial_init(SERIAL_DEFAULT_DEVICE);

  return 0;
}

static void wait()
{
  volatile long i;
  for (i = 0; i < 300000; i++)
    ;
}

int main(void)
{
  static char buf[16];
  static long size = -1;
  char *entry_point;
  void (*f)(void);

  INTR_DISABLE; /* 割込み無効にする */

  init();

  puts("kzload (kozos boot loader) started.\n");

  while (1) {
    puts("kzload> "); /* プロンプト表示 */
    gets(buf); /* シリアルからのコマンド受信 */

    if (!strcmp(buf, "load")) { /* XMODEMでのファイルのダウンロード */
      size = xmodem_streaming(ihex_decode); // xmodem_streaming()関数を呼び出し，ihex_decode()関数を用いて受信データをデコード
      wait(); /* 転送アプリが終了し端末アプリに制御が戻るまで待ち合わせる */
      if (size < 0) {
	puts("\nXMODEM receive error!\n");
      switch (size) {
      case IHEX_ERR_STARTCHAR:
        puts("IHEX error: Invalid start character!\n");
        break;
      case IHEX_ERR_CHECKSUM:
        puts("IHEX error: Checksum mismatch!\n");
        break;
      case IHEX_ERR_RECORDTYPE:
        puts("IHEX error: Unsupported record type!\n");
        break;
      default:
        break;
      }
      } else {
	puts("\nXMODEM receive succeeded.\n");
      }
    } else if (!strcmp(buf, "run")) { /* ihex形式ファイルの実行 */
      entry_point = ihex_startaddr(); // ihex_startaddr()関数を呼び出し，展開時に設定されたエントリ・ポイントを取得
      if (!entry_point) {
	puts("run error!\n");
      } else {
	puts("starting from entry point: ");
	putxval((unsigned long)entry_point, 0);
	puts("\n");
  init_BMX(entry_point);
	f = (void (*)(void))entry_point;
	f(); /* ここで，ロードしたプログラムに処理を渡す */
	/* ここには返ってこない */
      }
    } else {
      puts("unknown.\n");
    }
  }

  return 0;
}
