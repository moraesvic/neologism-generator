const fs = require('fs');
const path = require('path');
const { exit } = require('process');
const util = require('util');

const DATA = path.join(__dirname, '/data/');
const readFile = util.promisify(fs.readFile);

/* QUESTION 1 - how long is the avg English word? */
/* QUESTION 2 - how many different characters does our alphabet have? */

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

POWER_TWO = [1,256,65536,16777216]

function match(buf, i, codec){
  for(let j = 0; j < codec.length; j++)
    if(buf[i+j] !== codec[j])
      return 0;
  return codec.length;
}

{
  readFile(DATA + 'pt_br_50k.txt')
  .then( fileContent => {
    let arr = {};
    console.log(fileContent); // mudar para read
    for(let i = 0; i < fileContent.length; i++){
      let ch = fileContent[i];
      // console.log(`reading position ${i}, ch=${ch}`);
      let decode = [ch];
      if((ch & BYTE_FOUR) === BYTE_FOUR)
        i += readUnicode(fileContent, i, 3, decode);
      else if((ch & BYTE_THREE) === BYTE_THREE)
        i += readUnicode(fileContent, i, 2, decode);
      else if((ch & BYTE_TWO) === BYTE_TWO)
        i += readUnicode(fileContent, i, 1, decode);

      let cdpt = 0;
      for(let j = 0; j < decode.length; j++)
        cdpt += (decode[j] << (j*8));
      if(!(cdpt in arr))
        arr[cdpt] = decode;
    }
    // console.log(arr);
    let codec = []
    Object.keys(arr).forEach( el => {
      codec.push(arr[el]);
    });
    for(let i = 0; i < codec.length; i++)
      ; //console.log('codec[%s] = %s', i.toString(16), codec[i]);
    let buf = Buffer.alloc(fileContent.length); // mudar para write
    let offset = 0;
    for(let i = 0; i < fileContent.length; )
      for(let j = 0; j < codec.length; j++){
        let ret = match(fileContent, i, codec[j]);
        if(ret){
          // console.log(`${fileContent[i]} matches codec[${j}]=${codec[j]}`);
          buf.writeUInt8(j+1, offset++); // important to add 1, we don't want 0
          i += ret;
          break;
        }
      }
    let tail;
    for(tail = 1; tail < buf.length; tail++)
      if(buf.readUInt8(buf.length - tail) !== 0)
        break;
    console.log(buf);
    console.log(buf.length - tail + 1);

    let newbuf = Buffer.alloc(fileContent.length);
    offset = 0;
    for(let i = 0; buf.readUInt8(i) !== 0; i++ ){
      let towrite = codec[buf.readUInt8(i)-1];
      for(let j = 0; j < towrite.length; j++)
        newbuf.writeUInt8(towrite[j], offset++);
    }
    console.log(newbuf);
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
  })
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