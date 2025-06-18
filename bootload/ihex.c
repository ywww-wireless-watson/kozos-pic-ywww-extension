#include "defines.h"
#include "lib.h"
#include "ihex.h"

static uint8 *startaddr = NULL;
static uint16 addr_offset = 0;

// ihex処理の初期化
void ihex_init()
{
  startaddr = NULL;
}

// 16進数文字を数値に変換する
static unsigned char h2d(unsigned char c)
{
  if ((c >= '0') && (c <= '9'))
    return c - '0';
  if ((c >= 'a') && (c <= 'f'))
    return c - 'a' + 0xa;
  if ((c >= 'A') && (c <= 'F'))
    return c - 'A' + 0xa;
  return -1;
}

// 16進数文字列を数値に変換する
static unsigned char getval(unsigned char **pp, unsigned char *sum)
{
  unsigned char val;
  unsigned char *p;

  p = *pp;
  val = h2d(*(p++)) << 4; // 16進数の1桁目を取り出し，4ビット左にシフト
  val |= h2d(*(p++));     // 16進数の2桁目を取り出し，1桁目と結合する
  *pp = p;                // 次の文字へのポインタを更新

  if (sum)
    *sum += val; // チェックサムを計算

  return val;
}

// レコードの1行をデコードする
static int decode_record(char *from)
{
  uint16 i, record_len, record_type, record_addr, checksum, sum = 0;
  unsigned char record_data[128];
  unsigned char *p;

  // レコードの先頭が':'でなければエラー
  p = from;
  if (*(p++) != ':')
  {
    if (*(p - 1) == '\0') // 改行コードのみの行は無視，それ以外はエラー
      return 0;
    else
      return IHEX_ERR_STARTCHAR;
  }
  record_len = getval(&p, &sum);       // レコードのデータのバイト数を取得
  record_addr = getval(&p, &sum) << 8; // レコードのアドレス（上位バイト）を取得
  record_addr |= getval(&p, &sum);     // レコードのアドレス（下位バイト）を取得
  record_type = getval(&p, &sum);      // レコードのタイプを取得
  for (i = 0; i < record_len; i++)     // レコードのデータを取得
  {
    record_data[i] = getval(&p, &sum);
  }
  checksum = getval(&p, NULL); // チェックサムを取得
  // チェックサムエラー処理
  if ((sum + checksum) & 0xff)
    return IHEX_ERR_CHECKSUM;

  switch (record_type)
  {
  case 0: // データレコード
    for (i = 0; i < record_len; i++)
    {
      *((uint8 *)((addr_offset << 16) + record_addr + i)) = record_data[i]; // 開始アドレスからデータを書き込む
    }
    break;
  case 1: // 終了レコード(End Of File)
    return 1;
  case 4: // 拡張リニアアドレスレコード：オフセットの取得
    addr_offset = record_data[0] << 8;
    addr_offset |= record_data[1];
    break;
  case 5: // スタートアドレスレコード
          // 4バイトのスタートアドレスを，ビッグエンディアンで取得
    for (i = 0; i < record_len; i++)
    {
      startaddr += record_data[i] << 8 * (3 - i);
    }
    break;
  default:
    return IHEX_ERR_RECORDTYPE;
  }
  return 0;
}

// 1文字デコード
static inline int decode_data(unsigned char c)
{
  static char ihex_buf[270];
  static int i = 0;

  // 改行コードが来るまでバッファにためる．
  ihex_buf[i++] = c;
  if ((c == '\r') || (c == '\n'))
  {
    ihex_buf[--i] = '\0'; // 改行コードを終端に変更
    i = 0;                // 次にバッファを使うときは，先頭から
    int result = decode_record(ihex_buf);
    if (result < 0)
      return result;
  }

  return 0;
}

// ihexデータから抜き出したブロックをデコードする
int ihex_decode(char *data_buf, int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    int result = decode_data(data_buf[i]); // 1文字ずつデコード
    if (result < 0)
      return result;
  }

  return 0;
}

// プログラムのスタートアドレスを返す
uint32 *ihex_startaddr()
{
  return startaddr;
}
