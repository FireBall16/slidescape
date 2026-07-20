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

    mat4 aMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
                      0.0, 0.57, -0.82, 0.0,  // 2. column
                      0.0, 0.82, 0.57, 0.0,  // 3. column
                      0.0, 0.0, 0.0, 1.0);

    mat4 bMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
                      0.0, 1.0, 0.0, 0.0,  // 2. column
                      0.0, 0.0, 1.0, 0.0,  // 3. column
                      0.0, 0.0, 0.0, 1.0);

    mat4 cMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
                      0.0, 1.0, 0.0, 0.0,  // 2. column
                      0.0, 0.0, 1.0, 0.0,  // 3. column
                      0.0, 0.0, 0.0, 1.0);

    mat4 dMat4 = mat4(1000.0, 0.0, 0.0, 0.0,  // 1. column
                      0.0, 1000.0, 0.0, 0.0,  // 2. column
                      0.0, 0.0, 1000.0, 0.0,  // 3. column
                      0.0, 0.0, 0.0, 1.0);

    mat4 eMat4 = mat4(1.099960915, 0.0, 0.0, 0.0,  // 1. column
                      0.0, -1.499263641, 0.0, 0.0,  // 2. column
                      0.0, 0.0, 0.00999999978, 0.0,  // 3. column
                      -6194.979915,	6716.701109, 0.0, 1.0);

    //vec4 pos = projection_view_matrix * bMat4 * vec4(aPos, -0.5f, 1.0f);
    //vec4 pos = projection_view_matrix * cMat4 * vec4(aPos, -0.5f, 1.0f);
    vec4 pos = eMat4 * vec4(aPos, -0.5f, 1.0f);
	//gl_Position = bMat4 * vec4(aPos, -0.5f, 1.0f);
	//vec4 temp_pos = cMat4 * bMat4 * vec4(aPos, 1.0f, 1.0f);
	//gl_Position = vec4(temp_pos.x, temp_pos.y, 0.0f, 1.0f);
	pos.z = -0.5f;
	//pos.xy *= 1000.0;
	//pos.x += aPos.x;
	//pos.y += aPos.y;
	//pos.w = 1.0f;
	//gl_Position = pos;
	//gl_Position = eMat4 * vec4(aPos, -0.5f, 1.0f);
	gl_Position = vec4(aPos, -0.5f, 1.0f);
	//gl_Position = vec4(aPos.xy, -0.5f, 1.0f);
	vs_out.TexCoord = aTexCoord;
}