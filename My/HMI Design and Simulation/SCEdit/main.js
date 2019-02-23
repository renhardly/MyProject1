

class element{
	constructor(ctx){
	this.focus_m=false;
	this.x=0;
	this.y=0;
	this.w=1;
	this.h=1;
	this.pickpointx=0;
	this.pickpointy=0;
	this.ctx=ctx;
	this.nexts={};
	this.previouses={};
	this.translation_X=0;
	this.translation_y=0;
	this.movable=false;
	}
	mynext(){
	return this.nexts;
	}
	getprevious(){
	return this.previouses;
	}	
	is_selected(x,y){
	false;
	}
	set_focus(isfocus)
	{
	this.focus_m=isfocus;
	}
	set_movable(ismovable)
	{
	this.movable=ismovable;
	}
	draw(){
	
	}
	redraw(){
		this.ctx.clear();
		
	}
}
function Cross_multiply(p1,p2)
{
	return p1.x_coordinate*p2.y_coordinate-p1.y_coordinate*p2.x_coordinate;
}
function vector_2p(p1,p2){
	
}
function Is_in_polygon(Check_point,Point_array){

  for (i=0; i<Point_array.length; i++) {
    var vector=vector_2p(Check_point,Point_array[i])
  }
}
class transition extends element{
  constructor(x,y,ctx) {
	super(ctx);
    this.start_point = 
	{
		x_coordinate:x,
		y_coordinate:y
	};
		this.first_Bezier_control_point = 
	{
		x_coordinate:x,
		y_coordinate:y+80
	};
	    this.second_Bezier_control_point = 
	{
		x_coordinate:x+180,
		y_coordinate:y+80
	};
	    this.end_point = 
	{
		x_coordinate:x+180,
		y_coordinate:y
	};
	this.framepoints=[];
	this.framepoints.push(this.start_point);
	this.framepoints.push(this.first_Bezier_control_point);
	this.framepoints.push(this.second_Bezier_control_point);
	this.framepoints.push(this.end_point);

  }
  is_in_frame(x,y){
  var Check_point = 
	{
		x_coordinate:x,
		x_coordinate:y
	};

	return Is_in_polygon(Check_point);
  }
  is_selected(x,y){
  
  var tmp=Math.pow(x-this.x-this.translation_X,2)+Math.pow(y-this.y-this.translation_y,2)<=Math.pow(this.radius,2);
  return tmp;
  }
  draw(){
	this.ctx.beginPath();


     this.ctx.moveTo(this.x, this.y = y);
     this.ctx.quadraticCurveTo(130, 100, 230, 150);



	if(this.focus_m)
      this.ctx.lineWidth = 3;
	  else
	  this.ctx.lineWidth = 1;

      this.ctx.strokeStyle = 'black';
      this.ctx.stroke();

}
}
class intial extends element{

  constructor(x,y,ctx) {
	super(ctx);
    this.x = x;
	this.y = y;
	this.radius=10;

	myelements.push(this);

  }
  is_selected(x,y){
  
  var tmp=Math.pow(x-this.x-this.translation_X,2)+Math.pow(y-this.y-this.translation_y,2)<=Math.pow(this.radius,2);
  return tmp;
  }
  draw(){
	this.ctx.beginPath();
	this.ctx.arc(this.x+this.translation_X, this.y+this.translation_y, this.radius, 0, 2 * Math.PI);
/*
  #    this.ctx.moveTo(170, 80);
  #    this.ctx.bezierCurveTo(130, 100, 130, 150, 230, 150);
  #   this.ctx.bezierCurveTo(250, 180, 320, 180, 340, 150);
  #    this.ctx.bezierCurveTo(420, 150, 420, 120, 390, 100);
  #    this.ctx.bezierCurveTo(430, 40, 370, 30, 340, 50);
  #    this.ctx.bezierCurveTo(320, 5, 250, 20, 250, 50);
  #    this.ctx.bezierCurveTo(200, 5, 150, 20, 170, 80);
*/
      // complete custom shape
      this.ctx.closePath();
	  if(this.focus_m)
      this.ctx.lineWidth = 5;
	  else
	  this.ctx.lineWidth = 1;
	  
      
      this.ctx.fillStyle = '#8ED6FF';
      this.ctx.fill();
      this.ctx.strokeStyle = 'blue';
      this.ctx.stroke();
  }
  
}
class state extends element{

  constructor(x,y,ctx) {
	super(ctx);
    this.x = x;
	this.y = y;
	
	this.w = 150;
	this.h = 100;
	myelements.push(this);

  }
  draw(){
	this.ctx.fillStyle = "NavajoWhite";
	this.ctx.fillRect(this.x, this.y, this.w, this.h);
  }
  is_selected(x,y){
	
	console.log(this);
	console.log(x+'------------'+y);
	var tmp=((this.x<=x&&(this.x+this.w)>=x)&&(this.y<=y&&this.y+this.h>=y));
	return tmp;
  }

}


var PX=100;
var PY=100;
var Mytrace;
var c;
var ctx;
var myelements=[];
window.onload = function() {
onloadfunction();
}
function onloadfunction(){
  c = document.getElementById("myCanvas");
  var floating_w = document.getElementById("floatingwindow_head");
  ctx= c.getContext("2d");
  var img = document.getElementById("myImage");
  Mytrace = document.getElementById("debug");
  c.width=document.documentElement.clientWidth;
  c.height=document.documentElement.clientHeight;
  ctx.drawImage(img, 10, 10);
  ctx.moveTo(0,0);
  ctx.lineTo(PX,PY);
  ctx.stroke();
  let inital_p = new intial(100,100,ctx);

  let state_p = new state(300,300,ctx);

  let transition_test=new transition(100,100,ctx);

  c.addEventListener("mouseup", onrelease, false);
  c.addEventListener("mousemove", onDrag, false);
  c.addEventListener("mousedown", onClick, false);
  floating_w.addEventListener("mousedown", floatingonpress, false);
  floating_w.addEventListener("mousemove", floatingonpress, false);
  redraw();
}
function redraw(){
	ctx.clearRect(0,0,c.width,c.height);
	for (i = 0, len = myelements.length; i < len; i++) { 
	myelements[i].draw();
	}
}
function delfocus(){
	for (i = 0, len = myelements.length; i < len; i++) { 
	myelements[i].set_movable(false);
	myelements[i].focus_m=false;
	}
}
function floatingonpress(e){
var floating_w = document.getElementById("floatingwindow");
floating_w.left=e.pageX;
floating_w.top=e.pageY;
	console.log("floating_w.left"+floating_w.left);
	console.log("floating_w.top"+floating_w.top);
}
function onrelease(e) {

delfocus();
redraw();
console.log(myelements);
}
function onDrag(e) {
	for (i = 0, len = myelements.length; i < len; i++) { 
	if(myelements[i].movable)
	{
	myelements[i].x=e.pageX-this.offsetLeft-myelements[i].pickpointx;
	myelements[i].y=e.pageY-this.offsetTop-myelements[i].pickpointy;
	}
	}
}
function onClick(e) {
    var clickX;
    var clickY;
	var rect = c.getBoundingClientRect();

    clickX = e.pageX-rect.left;
    clickY = e.pageY-rect.top;
    PX = clickX;
    PY = clickY;
	console.log("clike x:"+PX);
	console.log("clike y:"+PY);
	
	for (i = 0, len = myelements.length; i < len; i++) { 
	myelements[i].set_focus(myelements[i].is_selected(PX,PY));
	if(myelements[i].focus_m){
	myelements[i].set_movable(true);
	myelements[i].pickpointx=PX-myelements[i].x;
	myelements[i].pickpointy=PY-myelements[i].y;
	}
	
	}
redraw();
console.log(myelements);

}
function test(){

console.log(myelements);
}
function drow_an_intial(){
let inital_p = new intial(100,100,ctx);
console.log(myelements);
}
function drow_an_state(){

 let state_p = new state(300,300,ctx);
 console.log(myelements);
}

