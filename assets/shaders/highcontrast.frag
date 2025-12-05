#version 130

uniform sampler2D texture;

void main() {
    // Get the pixel color
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Increase contrast significantly by remapping values around 0.5
    // Using a factor of 4.0 instead of 2.0 for much stronger contrast
    vec3 adjusted = (color.rgb - 0.5) * 4.0 + 0.5;

    // Clamp values to [0, 1]
    adjusted = clamp(adjusted, 0.0, 1.0);

    // Set the output color
    gl_FragColor = vec4(adjusted, color.a);
}
