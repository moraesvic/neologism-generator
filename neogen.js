/* NEOGEN MODULE */

const Path = require('path');

const C_SHRLIB = Path.join(__dirname, 'bin/neogen.so');
const C_BIN    = Path.join(__dirname, 'bin/perf');
const DATA     = Path.join(__dirname, 'data/');

const ref = require("ref-napi");
const ffi = require("ffi-napi");
const TeenPr = require("teen_process");

/* When things go awry... use this */
/*
const SegfaultHandler = require('segfault-handler');
SegfaultHandler.registerHandler("crash.log");
*/

const MAX_WORDS_IN_ANSWER = 1000;
const NON_ASCII_FACTOR    = 1.75;
const MAX_TRIE_DEPTH      = 10;
const MAX_TRIE_DEPTH_HARD = 15;

/* https://github.com/hermitdave/FrequencyWords/ */

const languageDecode = {};

languageDecode['ar'] = {file: 'ar.txt', nonAscii: true};
languageDecode['de'] = {file: 'de.txt', nonAscii: false};
languageDecode['el'] = {file: 'el.txt', nonAscii: true};
languageDecode['en'] = {file: 'en.txt', nonAscii: false};
/* Esperanto */
languageDecode['eo'] = {file: 'eo.txt', nonAscii: false};
languageDecode['es'] = {file: 'es.txt', nonAscii: false};
languageDecode['fr'] = {file: 'fr.txt', nonAscii: false};
languageDecode['he'] = {file: 'he.txt', nonAscii: true};
/* Serbo-Croatian Cyrillic */
languageDecode['hrsr_cy'] = {file: 'hrsr_cy.txt', nonAscii: true};
/* Serbo-Croatian Latin */
languageDecode['hrsr_la'] = {file: 'hrsr_la.txt', nonAscii: false};
languageDecode['it'] = {file: 'it.txt', nonAscii: false};
languageDecode['nl'] = {file: 'nl.txt', nonAscii: false};
languageDecode['no'] = {file: 'no.txt', nonAscii: false};
languageDecode['pl'] = {file: 'pl.txt', nonAscii: false};
languageDecode['pt_br'] = {file: 'pt_br.txt', nonAscii: false};
languageDecode['pt_pt'] = {file: 'pt_pt.txt', nonAscii: false};
languageDecode['ro'] = {file: 'ro.txt', nonAscii: false};
languageDecode['ru'] = {file: 'ru.txt', nonAscii: true};
/* Albanian */
languageDecode['sq'] = {file: 'sq.txt', nonAscii: false};
languageDecode['uk'] = {file: 'uk.txt', nonAscii: true};

const OK = 0;
/*
C ERRORS HERE
*/
const ERR_VALIDATE = 6;

const myFunction = async function neogen(nWords, lang, trieDepth, minWordLen){
    /* This is for use with the FFI library. However, it has a bug which 
     * leaks memory and a replacement is necessary */
    const ret =
    {
        status: OK,
        body:   null
    };

    /* validate and preprocess input */
    if (nWords === '' || lang === '' || trieDepth === '' || minWordLen === '') {
        console.log("Necessary input is empty");
        ret.status = ERR_VALIDATE;
        return ret;
    }
    nWords     = Number(nWords);
    trieDepth  = Number(trieDepth);
    minWordLen = Number(minWordLen);
    /* NaN, undefined and 0 evaluate to false */
    if (!nWords || !trieDepth || minWordLen === NaN) {
        console.log("Entered args were not numbers");
        ret.status = ERR_VALIDATE;
        return ret;
    }
    // console.log(`${nWords}, ${lang}, ${trieDepth}, ${minWordLen}`);
    if (!(lang in languageDecode)) {
        console.log("Invalid language");
        ret.status = ERR_VALIDATE;
        return ret;
    }
    if (trieDepth > MAX_TRIE_DEPTH || trieDepth < 1) {
        console.log("Trie-depth must be between 1 and %d", MAX_TRIE_DEPTH);
        ret.status = ERR_VALIDATE;
        return ret;
    }
    if (nWords > MAX_WORDS_IN_ANSWER) {
        console.log("That's too many words.");
        ret.status = ERR_VALIDATE;
        return ret;
    }

    langStruct = languageDecode[lang];
    filename = DATA + langStruct.file;
    if (langStruct.nonAscii) {
        trieDepth  = Math.floor(trieDepth  * NON_ASCII_FACTOR);
        if (trieDepth > MAX_TRIE_DEPTH_HARD)
            trieDepth = MAX_TRIE_DEPTH_HARD;
        minWordLen = Math.floor(minWordLen * NON_ASCII_FACTOR);
    }

    /* Maximum time shared library has */
    const timeout = 4000;
    /* If everything is fine ... do the work! */
    // return neogen_shrlib(ret, filename, trieDepth,
    // nWords, minWordLen, timeout);
    return neogen_C(ret, filename, trieDepth,
    nWords, minWordLen, timeout);
    
};

async function neogen_shrlib(ret, filename, trieDepth,
nWords, minWordLen, timeout)
{
    const charPtr = ref.refType('char');
    const cLib = ffi.Library(C_SHRLIB, {gen_words: 
            ['int', [charPtr, 'int', 'int', 'int', charPtr, 'int']]});

    const filenameBuf = Buffer.alloc(1000);
    filenameBuf.write(filename, 'ascii')

    let buf = Buffer.alloc(MAX_WORDS_IN_ANSWER * 100);
    const args = [filenameBuf, trieDepth, nWords, minWordLen, buf, timeout];
    /* later transform the following into async (use .async 
        with callback and other adaptations )*/
    const err = cLib.gen_words(...args);

    if (!err) {
        ret.body = buf.toString().replace(/\0/g, '');
        return ret;
    } else {
        console.log("Shared library returned error: %d", err);
        ret.status = err;
        return ret;    
    };
}

async function neogen_C(ret, filename, trieDepth,
nWords, minWordLen, timeout)
{
    let out = await TeenPr.exec(`${C_BIN}`,
            [ filename, trieDepth, nWords, minWordLen, timeout ]);
    console.log(out);
    ret.body = out.stdout;
    return ret;
}

module.exports = myFunction;