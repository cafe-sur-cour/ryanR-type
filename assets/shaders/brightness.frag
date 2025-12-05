#version 130

uniform sampler2D texture;
uniform float brightness = 1.0;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    vec3 adjusted = color.rgb * brightness;

    gl_FragColor = vec4(adjusted, color.a);
}
