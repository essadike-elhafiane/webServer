let text = document.getElementById("text");
let text1 = document.getElementById("text1");
let select = document.getElementById("select");

text.addEventListener('input', ()=>{
oncode();
});
select.addEventListener('change', ()=>{
oncode();
});

function oncode(){
    if(select.value == ("encode")){
        text1.value = btoa(text.value);
 
    }else{
        text1.value = atob(text.value);

    }
}