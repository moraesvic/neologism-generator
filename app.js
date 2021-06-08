const express = require('express');
const path = require('path');

const app = express();
const PUBLIC = path.join(__dirname, 'public/');

app.get('/', function(req, res){
  res.send('ok');
});

const PORT = process.env.PORT || 5000

app.listen(PORT, function(req,res){
    console.log('Listening on port ' + PORT + '...');
});