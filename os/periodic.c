#include "defines.h"
#include "kozos.h"
#include "consdrv.h"
#include "tmrdrv.h"
#include "lib.h"
#include "kozuser.h"

int time = 0;
int latency_counts[MAX_LATENCY - MIN_LATENCY + 1] = {0};

int *get_latency_counts()
{
  return latency_counts;
}

void reset_latency_counts()
{
  for (int i = 0; i < MAX_LATENCY - MIN_LATENCY + 1; i++)
  {
    latency_counts[i] = 0;
  }
}

int periodic_main(int argc, char *argv[])
{
  char *p;
  int size;

  tmr_interval(TMR_SECOND_DEVICE, 1);
  int offset = 0;
  int initial_iteration = 2;
  unsigned int current = 0;
  while (1)
  {
    unsigned int previous = current;
    current = get_current_time();
    if (initial_iteration)
    {
      initial_iteration--;
      offset = get_offset();
    }
    offset += 48000;
    int latency = current - offset;
    latency /= 48;
    if (!initial_iteration)
    {
      if (latency >= MIN_LATENCY && latency <= MAX_LATENCY)
      {
        latency_counts[latency - MIN_LATENCY]++;
      }
      else if (latency > MAX_LATENCY)
      {
        latency_counts[MAX_LATENCY - MIN_LATENCY]++;
      }
      else if (latency < MIN_LATENCY)
      {
        latency_counts[0]++;
      }
    }
    kz_sleep();
    time++;
  }
  return 0;
}
