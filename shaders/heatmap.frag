#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

uniform sampler2D heatmap_texture;
uniform sampler1D colormap_lut_texture;

void main()
{
    float intensity = texture(heatmap_texture, fs_in.TexCoord).r;
    if (intensity <= 0) {
        FragColor = vec4(0, 0, 0, 0);
    } else {
        vec4 heatmap_col = texture(colormap_lut_texture, intensity);
        FragColor = heatmap_col;
    }
}

