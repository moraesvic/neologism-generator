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

void fillZero(unsigned char * arr, unsigned sz){
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

CodecKey * genCodec(unsigned char * buf, unsigned sz){
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
      if(cdpt == codec[j].cdpt){
        inArray = 1;
        break;
      }
    
    if(!inArray){
      codec[offset].cdpt = cdpt;
      for(j = 0; j < 4; j++)
        codec[offset].byteGroup[j] = byteGroup[j];
      offset++;
    }
  }
  return codec;
}

unsigned char * bufFromFile(char * fstr, unsigned * sz){
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

void doNothing(CodecKey * cdcKey){
  return;
}

int match(unsigned char * buf, int i, CodecKey cdcKey){
  int j;
  for(j = 0; cdcKey.byteGroup[j] != 0 && j < 4; j++)
    if(buf[i+j] != cdcKey.byteGroup[j])
      return 0;
  return j;
}

unsigned char * encode(unsigned char * readBuf, unsigned sz, CodecKey * codec){
  unsigned char * writeBuf = calloc(sz, sizeof(char));
  unsigned i, j, ret, offset = 0;
  for(i = 0; i < sz && offset < sz; )
    for(j = 0; codec[j].cdpt != 0; j++){
      ret = match(readBuf, i, codec[j]);
      if(ret){
        writeBuf[offset++] = j+1;
        i += ret;
        break;
      }
    }
  return writeBuf;
}

unsigned char * decode(unsigned char * readBuf, unsigned sz, CodecKey * codec){
  unsigned char * writeBuf = calloc(sz, sizeof(char));
  unsigned i, j, offset = 0;
  unsigned char * towrite;

  for(i = 0; readBuf[i] != 0 && offset < sz; i++){
    //printf("we read %x from encoded\nwriting bytes: ", readBuf[i]);
    towrite = codec[ readBuf[i] - 1 ].byteGroup;
    for(j = 0; towrite[j] != 0 && j < 4; j++){
      writeBuf[offset++] = towrite[j];
      //printf("%x ", towrite[j]);
    }
    //printf("\n");
  }
  return writeBuf;
}

unsigned getTail(unsigned char * buf, unsigned sz){ /* inline ? */
  unsigned tail;
  for(tail = 0; tail < sz; tail++)
    if(buf[sz - tail - 1] != 0)
      break;
  return tail;
}

int main(){
  char fstr[64] = SOURCEFILE;
  unsigned char * readBuf, * encodedBuf, * decodedBuf;
  unsigned sz, tail;
  CodecKey * codec;
  
  readBuf = bufFromFile(fstr, &sz);
  codec = genCodec(readBuf, sz);
  encodedBuf = encode(readBuf, sz, codec); // change name
  // fwrite(encodedBuf, sizeof(char), 100, stdout);
  // return 0;
  tail = getTail(encodedBuf, sz);
  // printf("%d\n", sz - tail);

  decodedBuf = decode(encodedBuf, sz, codec);
  fwrite(decodedBuf, sizeof(char), 100, stdout);

  free(readBuf);
  free(codec);
  free(encodedBuf);
  free(decodedBuf);
  return 0; 
}