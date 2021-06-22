function formToJSONStr(form){
    const sz = form.children.length;
    let dict = {};
    for(let i = 0; i < sz; i++)
        formToJSONStr2(form.children[i], dict);
    return JSON.stringify(dict);
}

function formToJSONStr2(el, dict){
    const validTags = ["INPUT", "SELECT", "TEXTAREA", "DATALIST", "OUTPUT"];
    if (validTags.includes(el.tagName) && el.id)
        dict[el.name] = el.value;
    
    const sz = el.children.length;
    for(let i = 0; i < sz; i++)
        formToJSONStr2(el.children[i], dict);
}

function formToJSONNative(form){
    const fd = new FormData(form);
    let dict = {};
    for(let [key, value] of fd.entries())
        dict[key] = value;
    return JSON.stringify(dict);
}

/* I actually am not sure if the things below work 
In doubt, just use the ones above */