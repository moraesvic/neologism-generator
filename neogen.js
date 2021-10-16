/* NEOGEN MODULE */

const Path = require('path');

const C_BIN    = Path.join(__dirname, 'bin/perf');
const DATA     = Path.join(__dirname, 'data/');

const chpr = require("child_process");

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
    return neogen_C(ret, filename, trieDepth,
    nWords, minWordLen, timeout);
    
};

function chprPromise(cmd, args)
{
    return new Promise((resolve, reject) => {
        chpr.exec(cmd, args, function(err, stdout, stderr){
            if (err)
                reject(stderr);
            else
                resolve(stdout);
        });
    });
}

async function neogen_C(ret, filename, trieDepth,
nWords, minWordLen, timeout)
{
    try {
        ret.body = await chprPromise(`${C_BIN} ${filename} ${trieDepth} ${nWords} \
            ${minWordLen} ${timeout}`);
    } catch(err) {
        ret.body = err;
    } finally {
        return ret;
    }
}

module.exports = myFunction;