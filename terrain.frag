#version 110

varying vec3 vNormal;
varying vec3 vPosition;
varying vec4 vColor;


void main() {
    vec3 lightDir = normalize(vec3(0.0, 0.0, -1.0));
    float lightIntensity = max(dot(normalize(vNormal), lightDir), 0.0);

    float radial = length(vPosition.xy);
    float falloff = pow(1.0 - clamp(radial / 200.0, 0.0, 1.0), 2.0);

    float ambient = 1.0; //should be 0.05
    float finalIntensity = lightIntensity * falloff * 0.8 + ambient;

    vec3 color = vColor.rgb * finalIntensity;
    gl_FragColor = vec4(color, 1.0);
}
