#version 130 

in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform mat4 ModelViewObj;
uniform vec4 LightPosition;
uniform mat4 ProjectionObj;

void main()
{
    fN = vNormal;
    fE = (ModelViewObj*vPosition).xyz;
    fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
	fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = ProjectionObj*ModelViewObj*vPosition;
}
