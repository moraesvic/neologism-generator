async function postFormXHR(form, url) {
    var xhttp = new XMLHttpRequest();
    let myPromise = new Promise(function(resolve)
    {
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200)
              resolve( this.responseText );
          };
    });
    
    xhttp.open("POST", url, true);
    xhttp.setRequestHeader("Content-type", "application/json");

    /*
    const data = new FormData(form);

    for (var value of data.values()) {
        console.log(value);
     }

    let myJSON = {};
    for (let [key, value] of data.entries())
        myJSON[key] = value;

    console.log(myJSON);
    const sendString = JSON.stringify(myJSON);
    console.log(sendString);
    xhttp.send(sendString);
    */

    xhttp.send(new FormData(form));

    return await myPromise;
}

async function postJSONXHR(myJSON, url) {
    var xhttp = new XMLHttpRequest();
    let myPromise = new Promise(function(resolve)
    {
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200)
              resolve( this.responseText );
          };
    });
    
    xhttp.open("POST", url, true);
    xhttp.setRequestHeader("Content-type", "application/json");

    const sendString = JSON.stringify(myJSON);
    xhttp.send(sendString);

    return await myPromise;
}

async function postAndInject(myJSON, url, id){
  postJSONXHR(myJSON, url)
  .then( async (res) => {
    const elem = document.getElementById(id);
    const parsed = JSON.parse(res);
    elem.innerHTML += parsed.html;
    const scriptNode = document.createElement('script');
    scriptNode.innerHTML = parsed.js;
    elem.appendChild(scriptNode);
  });
}

async function postAndRewrite(myJSON, url, id){
  postJSONXHR(myJSON, url)
  .then( async (res) => {
    const elem = document.getElementById(id);
    const parsed = JSON.parse(res);
    elem.innerHTML = parsed.html;
    const scriptNode = document.createElement('script');
    scriptNode.innerHTML = parsed.js;
    elem.appendChild(scriptNode);
  });
}