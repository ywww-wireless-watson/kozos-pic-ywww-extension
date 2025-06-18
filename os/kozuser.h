#ifndef _KOZUSER_H_INCLUDED_
#define _KOZUSER_H_INCLUDED_

/* ユーザ・タスクのメイン関数 */
int command_main(int argc, char *argv[]);
int periodic_main(int argc, char *argv[]);

// 周期タスクのパラメータ
typedef struct {
    unsigned long average;
    unsigned long min;
    unsigned long max;
    unsigned long previous;
  } periodic_state_t;

//周期タスクのパラメータ取得
#define MIN_LATENCY -1
#define MAX_LATENCY 1000
int* get_latency_counts();
void reset_latency_counts();

#endif
