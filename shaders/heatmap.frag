#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

//in vec2 TexCoord;

// texture samplers
uniform sampler2D heatmap_texture;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
	//FragColor = texture(texture1, TexCoord);
	float intensity = texture(heatmap_texture, fs_in.TexCoord).r;

    //FragColor = vec4(intensity, intensity, intensity, 1.0);

	//if (intensity > 0.5) {
	//	FragColor = vec4(intensity, 0, 0, 1.0);
	//} else {
	//	FragColor = vec4(0, 0, 1-intensity, 1.0);
	//}
	//FragColor = vec4(intensity, 0, 1-intensity, intensity + 0.2);
	FragColor = vec4(intensity, 0, 1-intensity, 0.2f);
	//FragColor = vec4(intensity, 0, 0, 0.5);
	//FragColor = vec4(intensity, 0, intensity, intensity + 0.2);
	//FragColor = vec4(intensity, intensity, intensity, 0.5);

}

