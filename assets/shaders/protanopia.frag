#version 120

uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    mat3 protanopiaMatrix = mat3(
        0.567, 0.433, 0.0,
        0.558, 0.442, 0.0,
        0.0, 0.242, 0.758
    );

    vec3 adjusted = protanopiaMatrix * color.rgb;

    gl_FragColor = vec4(adjusted, color.a);
}
