#version 130

uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    mat3 tritanopiaMatrix = mat3(
        0.95, 0.05, 0.0,
        0.0, 0.433, 0.567,
        0.0, 0.475, 0.525
    );

    vec3 adjusted = tritanopiaMatrix * color.rgb;

    gl_FragColor = vec4(adjusted, color.a);
}
