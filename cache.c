#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
void help();
int main(int argc, char *argv[])
{
  int opt;
  while ((opt=getopt(argc, argv, "hc:b:s:")) != -1)
  {
    switch (opt)
    {
    case 'h':
      help();
      return 0;
      break;
    case 'c':
      break;
    case 'b':
      
      break;
    case 's':
      
      break;
    default:
      help();
      break;
    }
  }


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

void help()
{
  printf("This is a simple program to check how many miss and hit is there for add two matrix like this\n");
  printf("for(int i=0;i<size;i++)\n");
  printf("  for (int j = 0; j < size; j++)\n");
  printf("    A[i][j]+=B[j][i];\n");
  printf("Usage: ./cache [-h-c-b-s]\n");
  printf("use -h for help\n");
  printf("use -c x for cache size where `x` is cache size you like\n");
  printf("use -b x for block size where `x` is block size you like\n");
  printf("use -s x for matrix size where `x` is matrix`s line size you like\n");
  return;
}

//access hit or miss
void my_access(char (*A)[size], int *miss, int *hit, int i, int j)
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
      my_access(A, &miss, &hit, i, j);
      my_access(B, &miss, &hit, j, i);
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
          my_access(A, &miss, &hit, i, j);
          my_access(B, &miss, &hit, j, i);
        }
  for (int i = 0; i < cache_size / BS; i++)
    free(cache[i]);
  printf("bench block:\n");
  printf("size:%d\nblocksize:%d\n", size, block_size);
  printf("miss times:%d\nhit times:%d\n", miss, hit);
}