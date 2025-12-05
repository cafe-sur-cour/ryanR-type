#version 130

uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    mat3 deuteranopiaMatrix = mat3(
        0.625, 0.375, 0.0,
        0.7, 0.3, 0.0,
        0.0, 0.3, 0.7
    );

    vec3 adjusted = deuteranopiaMatrix * color.rgb;

    gl_FragColor = vec4(adjusted, color.a);
}