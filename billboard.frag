#version 110

uniform sampler2D texture;

varying vec2 vTexCoords; // Passed from vertex shader
varying vec4 vColor;

void main()
{

   // gl_FragColor = texture2D(texture,vTexCoords);
   gl_FragColor = vColor;
}
