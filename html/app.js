document.getElementById("compte").addEventListener("click",()=>{
    location.replace("html/compte.html");
    //document.getElementById("compte").href = "compte.html"
  });
  document.getElementById("button").addEventListener("click",()=>{
      //localStorage.setItem("name","saddik");
     var email = document.getElementById("email").value;
     var passwordd = document.getElementById("password").value;
     if(email==localStorage.getItem("name") && passwordd == localStorage.getItem("password")){
         location.replace("html/app-coder.html");            
     }else{
         alert("votre mote passe ou email est incorrect !")
     }
  });
  class slider{
  constructor(){
      this.images = [];
      this.images[0] = "/images/i.jpeg";
      this.images[1] = "/images/S.jpeg";
      this.images[2] = "/images/facebook.jpeg";
      this.contor=0;

      this.playss();
      setInterval(()=>{
          this.playss();
      },300);
  }
  playss(){
      if(this.contor < this.images.length-1){
          this.contor++; 
      }else{
          this.contor=0;
      }
      document.getElementById("name").src = this.images[this.contor];
  }
} 

onload =new slider();