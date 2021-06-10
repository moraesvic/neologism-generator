#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCEFILE "../data/ru_50k.txt"

#define OTHER_BYTES  0x80
#define BYTE_TWO     0xc0
#define BYTE_THREE   0xe0
#define BYTE_FOUR    0xf0

typedef struct {
  unsigned cdpt;
  unsigned char byteGroup[4];
} CodecKey;

void fillZero(unsigned char * arr, int sz){
  int i;
  for(i = 0; i < sz; i++)
    arr[i] = 0;
}

int readUnicode(unsigned char * buf, int i, int n, unsigned char * byteGroup){
  int j;
  for(j = 1; j <= n; j++)
    if((buf[i+j] & OTHER_BYTES) == OTHER_BYTES)
      byteGroup[j] = buf[i+j];
    else {
      printf("error!");
      return 0;
    }
  return n;
}

CodecKey * genCodec(unsigned char * buf, int sz){
  CodecKey * codec = calloc(255, sizeof(CodecKey));
  unsigned offset = 0;
  int i, j, cdpt, inArray;
  unsigned char * byteGroup = calloc(4, sizeof(char));

  for(i = 0; i < sz; i++){
    fillZero(byteGroup, 4);
    byteGroup[0] = buf[i];
    if((byteGroup[0] & BYTE_FOUR) == BYTE_FOUR)
      i += readUnicode(buf, i, 3, byteGroup);
    else if((byteGroup[0] & BYTE_THREE) == BYTE_THREE)
      i += readUnicode(buf, i, 2, byteGroup);
    else if((byteGroup[0] & BYTE_TWO) == BYTE_TWO)
      i += readUnicode(buf, i, 1, byteGroup);

    cdpt = 0;
    for(j = 0; j < 4; j++)
      cdpt += byteGroup[j] << (j*8);

    inArray = 0;
    for(j = 0; j < offset; j++)
      if(cdpt == codec[j].cdpt)
        inArray = 1;
    
    if(!inArray){
      // printf("not yet in array\n");
      codec[offset].cdpt = cdpt;
      for(j = 0; j < 4; j++)
        codec[offset].byteGroup[j] = byteGroup[j];
      offset++;
    }
    else
      ; //printf("already in array\n");
  }
  return codec;
}

unsigned char * bufFromFile(char * fstr, int * sz){
  FILE * f;
  unsigned char * buf;

  f = fopen(fstr, "rb");
  fseek(f, 0, SEEK_END);
  *sz = ftell(f); 
  rewind(f);
  buf = calloc(*sz, sizeof(char));
  fread(buf, 1, *sz, f);
  fclose(f);

  return buf;
}

void printCodecKeys(CodecKey * cdcKey){
  int i, j;
  for(i = 0; cdcKey[i].cdpt != 0; i++){
    printf("cdpt: %x\n[ ", cdcKey[i].cdpt);
    for(j = 0; j < 4; j++)
      printf("%x ", cdcKey[i].byteGroup[j]);
    printf("]\n");
  }
}

int main(){
  char fstr[64] = SOURCEFILE;
  unsigned char * buf;
  int sz;
  CodecKey * cdcKey;
  
  buf = bufFromFile(fstr, &sz);
  cdcKey = genCodec(buf, sz);
  printCodecKeys(cdcKey);

  return 0; 
}