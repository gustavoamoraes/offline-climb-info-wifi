var comment_box = document.getElementById("comment-box");
var comment_prefab = document.getElementById("comment-father");

fetch("/comments")
.then(res => res.text())
.then(data => csvJSON(data))
.then(text => JSON.parse(text))
.then(json => spawn_comments(json));

function csvJSON(csv)
{
    var lines=csv.split("\n");
    var result = [];
    var headers=lines[0].split(",");
  
    for(var i=1;i<lines.length;i++){
  
        var obj = {};
        var currentline=lines[i].split(",");
  
        for(var j=0;j<headers.length;j++){
            obj[headers[j]] = currentline[j];
        }
  
        result.push(obj);
    }
  
    return JSON.stringify(result); //JSON
}

function submit_comment ()
{
    let form = document.forms.namedItem('comment-form');

    let date = new Date().toLocaleString('pt-br', { day: 'numeric' , month: 'long', year: 'numeric'});
    let name = form.querySelector('#name').value;
    let content = encodeURIComponent(form.querySelector('#content').value);

    fetch("/comments", 
    {
        method: "POST", 
        body: 
        `${name},${date},${content}`
    });
}

function spawn_comments (comments)
{
    for (let i = 0; i < comments.length; i++) 
    {
        var new_el = comment_prefab.cloneNode(true);

        new_el.querySelector("#comment-content").innerHTML = decodeURIComponent(comments[i]['content']);
        new_el.querySelector("#comment-date").innerHTML = "&nbsp- " + comments[i]['date'];
        new_el.querySelector("#author-name").innerHTML = comments[i]['name'];

        comment_box.appendChild(new_el);
    }

    comment_prefab.remove();
}