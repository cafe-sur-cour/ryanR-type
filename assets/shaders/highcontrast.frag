#version 130

uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    vec3 adjusted = (color.rgb - 0.5) * 2.0 + 0.5;

    adjusted = clamp(adjusted, 0.0, 1.0);

    gl_FragColor = vec4(adjusted, color.a);
}
