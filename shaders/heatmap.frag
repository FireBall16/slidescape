#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

// texture samplers
uniform sampler2D heatmap_texture;
uniform sampler1D colormap_lut_texture;

void main()
{
	//float intensity = texture(heatmap_texture, fs_in.TexCoord).r;

	//FragColor = vec4(intensity, 0, 1-intensity, 0.2f);

    float intensity = texture(heatmap_texture, fs_in.TexCoord).r;
    vec4 heatmap_col = texture(colormap_lut_texture, intensity);
    FragColor = heatmap_col;
}

