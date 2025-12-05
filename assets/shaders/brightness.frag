#version 130

uniform sampler2D texture;
uniform float brightness = 1.0;

void main() {
    // Get the pixel color
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Apply brightness adjustment
    vec3 adjusted = color.rgb * brightness;

    // Set the output color
    gl_FragColor = vec4(adjusted, color.a);
}