const fs = require('fs');
const path = require('path');
const { exit } = require('process');
const util = require('util');

const DATA = path.join(__dirname, '/data/');
const readFile = util.promisify(fs.readFile);

/* QUESTION 1 - how long is the avg English word?
   BETWEEN 4 AND 5 CHARACTERS.
*/

/* QUESTION 2 - how many different characters does our alphabet have?
   AT MOST SOME 60-70, considering diacritics
*/

const OTHER_BYTES = 0x80;
const BYTE_TWO = 0xc0;
const BYTE_THREE = 0xe0;
const BYTE_FOUR = 0xf0;

function readUnicode(buf, i, n, decode){
  for(let j = 1; j <= n; j++)
    if((buf[i+j] & OTHER_BYTES) === OTHER_BYTES)
      decode.push(buf[i+j]);
    else {
      console.log('error!');
      return 0;
    }
  return n;
}

function match(buf, i, codec){
  for(let j = 0; j < codec.byteGroup.length; j++)
    if(buf[i+j] !== codec.byteGroup[j])
      return 0;
  return codec.byteGroup.length;
}

function genCodec(readBuf){
  let codec = [];
  for(let i = 0; i < readBuf.length; i++){
    let ch = readBuf[i];
    let byteGroup = [ch];
    if((ch & BYTE_FOUR) === BYTE_FOUR)
      i += readUnicode(readBuf, i, 3, byteGroup);
    else if((ch & BYTE_THREE) === BYTE_THREE)
      i += readUnicode(readBuf, i, 2, byteGroup);
    else if((ch & BYTE_TWO) === BYTE_TWO)
      i += readUnicode(readBuf, i, 1, byteGroup);

    let cdpt = 0;
    for(let j = 0; j < byteGroup.length; j++)
      cdpt += (byteGroup[j] << (j*8));
    
    let inArray = 0;
    for(let j = 0; j < codec.length; j++)
      if(codec[j].cdpt === cdpt){
        inArray = 1;
        break;
      }

    if(!inArray)
      codec.push({cdpt: cdpt, byteGroup: byteGroup});
  }
  return codec;
}

function printCodec(codec){
  for(let i = 0; i < codec.length; i++){
    let s = '';
    for(let j = 0; j < codec[i].length; j++)
        s += codec[i][j].toString(16) + ' ';
    console.log(s);
  }
}

function encode(readBuf, codec){
  let writeBuf = Buffer.alloc(readBuf.length);
  let offset = 0;
  for(let i = 0; i < readBuf.length; )
    for(let j = 0; j < codec.length; j++){
      let ret = match(readBuf, i, codec[j]);
      if(ret){
        // console.log(`${fileContent[i]} matches codec[${j}]=${codec[j]}`);
        writeBuf.writeUInt8(j+1, offset++); // important to add 1, we don't want 0
        i += ret;
        break;
      }
    }
  return writeBuf;
}

{
  readFile(DATA + 'ru_50k.txt')
  .then( readBuf => {
    let codec = genCodec(readBuf);
    let encodedBuf = encode(readBuf, codec);
  
    let tail;
    for(tail = 0; tail < encodedBuf.length; tail++)
      if(encodedBuf.readUInt8(encodedBuf.length - tail - 1) !== 0)
        break;

    let newbuf = Buffer.alloc(readBuf.length);
    offset = 0;
    for(let i = 0; encodedBuf.readUInt8(i) !== 0; i++ ){
      let towrite = codec[ encodedBuf.readUInt8(i)-1 ].byteGroup; // important to subtract one
      for(let j = 0; j < towrite.length; j++)
        newbuf.writeUInt8(towrite[j], offset++);
    }
    console.log(newbuf.toString().slice(0, 100));
    exit(0);
    /* */
    let alph = {};
    for(let i = 0; i < fileContent.length; i++){
      let ch = fileContent[i];
      if( !(ch in alph) ) alph[ch] = 1;
      else alph[ch] += 1;
    }
    console.log(alph);
    console.log(Object.keys(alph).length);
    let utf8 = {};
    let code = 1;
    Object.keys(alph).forEach( el => {
      utf8[code++] = el;
    });
    console.log(utf8);
    exit(0);
    let dic = [];
    let array;
    array = fileContent.split('\n');
    array.forEach(el => {
      if(el === '') return;
      let temp = el.split(' ');
      dic.push({word: temp[0], freq: Number(temp[1])});
    });
    return dic;
  });
}
  /*
  .then( dic => {
    const MAX_LENGTH = 50;
    let wordsWithLength = new Array(MAX_LENGTH).fill(0);
    let totalFreq = 0;
    dic.forEach( el => {
      wordsWithLength[el.word.length] += el.freq;
      totalFreq += el.freq;
    });
    console.log(wordsWithLength);
    let avg = 0;
    for(let i = 0; i < MAX_LENGTH; i++)
      avg += i * wordsWithLength[i];
    avg /= totalFreq;
    console.log(avg);
  });
}

*/