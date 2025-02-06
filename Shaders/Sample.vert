#version 330 core 

//Gets the data at attrib index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//Declare a variable to hold the data
// that we're going to pass
//uniform float x;
//uniform float y;

uniform mat4 transform;

void main(){
//gl_Position is predefined
//this denotes the final position of the vertex / point

//vec3 newPos = vec3(aPos.x + x, aPos.y+y, aPos.z);

gl_Position =  transform * vec4(aPos, 1.0); //Turns the vec3 into a vec4

}