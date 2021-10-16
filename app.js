const express = require('express');
const path = require('path');

const app = express();
app.use('/static', express.static(path.join(__dirname, 'public')));

/* in order to read POST material */
// Parse URL-encoded bodies (as sent by HTML forms)
app.use(express.urlencoded({ extended: false }));

// Parse JSON bodies (as sent by API clients)
app.use(express.json());

const neogen = require('./neogen.js');

app.get('/', function(req, res) {
    res.render('neogen.ejs', {});
});

app.post('/', async function(req,res) {
    /* nWords, lang, trieDepth, minWordLen */
    const nWords = req.body.nWords;
    const lang = req.body.lang;
    const trieDepth = req.body.trieDepth;
    const minWordLen = req.body.minWordLen;
    neogen(nWords, lang, trieDepth, minWordLen)
    .then(ret => {
        if (ret.status === 0) {
            res.json({text: ret.body});
        } else {
            res.json({text: "Error code " + ret.status})
        }
    });
});

const PORT = process.env.PORT || 4444

app.listen(PORT, function(req,res){
    console.log('Listening on port ' + PORT + '...');
});
