#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out VS_OUT {
    vec2 TexCoord;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 projection_view_matrix;

void main()
{
	gl_Position = projection_view_matrix * model_matrix * vec4(aPos, 0.0f, 1.0f);
	vs_out.TexCoord = aTexCoord;
}
