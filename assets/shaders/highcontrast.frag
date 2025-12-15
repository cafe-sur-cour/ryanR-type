#version 120

uniform sampler2D texture;

const float contrast = 3.0;     // Strong contrast
const float brightness = 0.0;   // Normal brightness
const float gamma = 1.0;        // Normal gamma

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    color.rgb += brightness;

    color.rgb = ((color.rgb - 0.5) * contrast) + 0.5;

    color.rgb = pow(color.rgb, vec3(1.0 / gamma));

    gl_FragColor = color;
}
