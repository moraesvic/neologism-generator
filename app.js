const express = require('express');
const path = require('path');

const app = express();
app.use('/static', express.static('public'));

/* in order to read POST material */
// Parse URL-encoded bodies (as sent by HTML forms)
app.use(express.urlencoded({ extended: false }));

// Parse JSON bodies (as sent by API clients)
app.use(express.json());

const PUBLIC = path.join(__dirname, 'public/');
const neogen = require('./neogen.js');

app.get('/', function(req, res){
  res.send('ok');
});

app.get('/neogen', function(req, res) {
    res.sendFile(PUBLIC + 'neogen.html');
});

app.post('/neogen', async function(req,res) {
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

const PORT = process.env.PORT || 5000

app.listen(PORT, function(req,res){
    console.log('Listening on port ' + PORT + '...');
});