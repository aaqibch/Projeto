
var bxs = [];

var bx_size = 6;
var s = 50;

var selected = -1;
var r = 0;//random(255);
var g = 0;//random(255);

function shuf(arra1) {
    var ctr = arra1.length, temp, index;

// While there are elements in the array
    while (ctr > 0) {
// Pick a random index
        index = Math.floor(Math.random() * ctr);
// Decrease ctr by 1
        ctr--;
// And swap the last element with it
        temp = arra1[ctr];
        arra1[ctr] = arra1[index];
        arra1[index] = temp;
    }
    return arra1;
}

function createBoxes(){
	r = random(255);
	g = random(255);
	var wind = random(100);
	
	for(var i=0; i<bx_size; i++){
		var bx = {color:i * 25 + wind, x:110, y:i*(s) + 20};
		bxs.push(bx);
	}
	
	shuf(bxs);
	
	for(var i=0; i<bx_size; i++)
		bxs[i].y = i*(s) + 20;
}

function drawBoxes(){
	background(200);
	for(var i=0; i<bx_size; i++){
		fill(r,g,bxs[i].color);
		rect(bxs[i].x,bxs[i].y,s,s);
	}
}

function checkOrder(){
	
	var temp_color = 0;
	for(var i=0; i<bx_size; i++){
		
		if(bxs[i].color < temp_color)
			return false;
		temp_color = bxs[i].color;
	}
	background(0,255,100);
	return true;
}


function setup() {
	createCanvas(320,480);
	background(200);
	
	
	createBoxes();
	drawBoxes();
}

function mouseClicked(){
	
	for(var i =0; i<bx_size; i++){
		if(mouseX>110 && mouseX<100+s){
			if(mouseY>bxs[i].y && mouseY<bxs[i].y+s){
				
				if(selected != -1){
					var temp_color = bxs[i].color;
					bxs[i].color = bxs[selected].color;
					bxs[selected].color = temp_color;
					drawBoxes()					
					selected = -1;
				}
				
				else{
					selected = i;
					fill(255,0,0);
					rect(bxs[i].x - 20,bxs[i].y + 20 ,10,10);
					rect(bxs[i].x + 15 + s,bxs[i].y + 20 ,10,10);
				}
				
				checkOrder();
			}
		}
	}
	
}

