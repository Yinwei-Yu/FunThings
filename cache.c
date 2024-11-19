#include <stdio.h>
#include <stdlib.h>
#define BS 4
#define size 8
#define cache_size 32
typedef __uint8_t bool;

typedef struct
{
  char *block[BS];
  unsigned int time;
} Block;

Block *cache[cache_size / BS];

//check if the pointer is in the cache
bool inCache(char *p)
{
  for (int i = 0; i < cache_size / BS; i++)
    for (int j = 0; j < BS; j++)
      if (cache[i]->block[j] == p)
        return 1;
  return 0;
}

//find latest and least used block
int maxTime()
{
  int block_num = 0;
  int i;
  for (i = 0; i < cache_size / BS; i++)
    if (cache[i]->time > cache[block_num]->time)
      block_num = i;
  return block_num;
}

//update time
void timeSet(int block_num)
{
  cache[block_num]->time = 0;
  for (int i = 0; i < cache_size / BS; i++)
    if (i != block_num)
      cache[i]->time++;
}

//find which block the pointer is in
int where(char *p)
{
  for (int i = 0; i < cache_size / BS; i++)
  {
    for (int j = 0; j < BS; j++)
      if (cache[i]->block[j] == p)
        return i;
  }
  return -1;
}

void normal_add(char A[size][size], char B[size][size]);
void bench_block_adder(char A[size][size], char B[size][size]);

int main(unsigned argc, char *argv[])
{

  //initial A and B
  char A[size][size];
  char B[size][size];
  for (int i = 1; i <= size; i++)
  {
    for (int j = 1; j <= size; j++)
    {
      A[i][j] = i * j;
      B[i][j] = size * size - i * j;
    }
  }

  normal_add(A, B);
  printf("\n");
  bench_block_adder(A, B);

  return 0;
}

//access hit or miss
void access(char (*A)[size], int *miss, int *hit, int i, int j)
{
  if (!inCache(&A[i][j]))
  {
    (*miss)++;
    int left = j - (j + BS) % BS;
    int right = BS + left - 1;
    int block_num = maxTime();
    for (int k = left, m = 0; k <= right; k++, m++)
    {
      cache[block_num]->block[m] = &A[i][k];
    }
    timeSet(block_num);
  }
  else
  {
    (*hit)++;
    int block_num = where(&A[i][j]);
    timeSet(block_num);
  }
}

void normal_add(char (*A)[size], char (*B)[size])
{
  for (int i = 0; i < cache_size / BS; i++)
    cache[i] = (Block *)malloc(sizeof(Block));
  int miss = 0;
  int hit = 0;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
    {
      access(A, &miss, &hit, i, j);
      access(B, &miss, &hit, j, i);
    }
  for (int i = 0; i < cache_size / BS; i++)
    free(cache[i]);
  printf("normal\n");
  printf("size:%d\nblocksize:%d\n", size, BS);
  printf("miss times:%d\nhit times:%d\n", miss, hit);
}

void bench_block_adder(char (*A)[size], char (*B)[size])
{
  for (int i = 0; i < cache_size / BS; i++)
  {
    cache[i] = (Block *)malloc(sizeof(Block));
    cache[i]->time = 0;
  }
  int miss = 0;
  int hit = 0;
  int block_size = 2;
  for (int ii = 0; ii < size; ii += block_size)
    for (int jj = 0; jj < size; jj += block_size)
      for (int i = ii; i < ii + block_size; i++)
        for (int j = jj; j < jj + block_size; j++)
        {
          access(A, &miss, &hit, i, j);
          access(B, &miss, &hit, j, i);
        }
  for (int i = 0; i < cache_size / BS; i++)
    free(cache[i]);
  printf("bench block:\n");
  printf("size:%d\nblocksize:%d\n", size, block_size);
  printf("miss times:%d\nhit times:%d\n", miss, hit);
}