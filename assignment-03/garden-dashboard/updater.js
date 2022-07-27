$('document').ready(function(){
    const interval = setInterval(function() {
        $.ajax({
            type: 'GET',
            url: "http://NICO-PC:8124/api/all",  
            success: function(data) {
                console.log(data);
                document.getElementById("mode").innerHTML = "Mode: " + data.mode;
                document.getElementById("temperature").innerHTML = "Temperature: " + data.temp;
                document.getElementById("light").innerHTML = "Light: " + data.light;
                document.getElementById("irrigator").innerHTML = "Irrigator: " + data.irrigator;

                document.getElementById("light1").innerHTML = "Light 1: " + data.l1;
                document.getElementById("light2").innerHTML = "Light 2: " + data.l2;
                document.getElementById("light3").innerHTML = "Light 3: " + data.l3  + "%";
                document.getElementById("light4").innerHTML = "Light 4: " + data.l4 + "%";
            }
        });
    }, 200);

});
