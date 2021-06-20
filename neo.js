const Path = require('path');
const Minim = require('minimist');
const Events = require('events');

const C_BINARY = Path.join(__dirname, 'bin/neogen.so');
const DATA     = Path.join(__dirname, 'data/');

const ref = require("ref");
const ffi = require("ffi");

const SegfaultHandler = require('segfault-handler');
const { response } = require('express');
const EventEmitter = require('events');
SegfaultHandler.registerHandler("crash.log");

const MAX_WORDS_IN_ANSWER = 1000;

async function main(){
    const argv = Minim(process.argv.slice(2));

    const help = argv.help || false;
    if(help){
        console.log("Usage: node neo.js [-n --db --depth --len]");
        return;
    }

    let nWords = Number(argv.n) || 10;
    let freqlist = argv.db || 'en_50k.txt';
    let trieDepth = argv.depth || 3;
    let minWordLen = argv.len || 3;
    let timeout = 1000;

    if (nWords > MAX_WORDS_IN_ANSWER) {
        console.log("That's too many words.");
        return;
    }

    const charPtr = ref.refType('char');
    const cLib = ffi.Library(C_BINARY, {gen_words: 
             ['int', [charPtr, 'int', 'int', 'int', charPtr, 'int']]});

    const filename = Buffer.alloc(1000);
    filename.write(DATA + freqlist, 'ascii')

    const buf = Buffer.alloc(MAX_WORDS_IN_ANSWER * 100);
    const args = [filename, trieDepth, nWords, minWordLen, buf, timeout]
    const fn = cLib.gen_words.async(...args, function(err, res){
        if (!err && res == 0)
            console.log(buf.toString());
        else
            console.log("error: %d", res);
    });

    /*
    console.log(ret);
    if (ret.out != 0) {
        console.log("Request invalid. Return code %d", ret.out);
        // return;
    }
    console.log(buf.toString());
    */
    
}

main();