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
    // identity matrix
    mat4 idMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
                       0.0, 1.0, 0.0, 0.0,  // 2. column
                       0.0, 0.0, 1.0, 0.0,  // 3. column
                       0.0, 0.0, 0.0, 1.0);

    // for small translation
    mat4 transMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column
                      0.0, 1.0, 0.0, 0.0,  // 2. column
                      0.0, 0.0, 1.0, 0.0,  // 3. column
                      0.5, 0.0, 0.0, 1.0);

    // for small scaling
    mat4 scaledMat4 = mat4(0.8, 0.0, 0.0, 0.0,  // 1. column
                       0.0, 0.8, 0.0, 0.0,  // 2. column
                       0.0, 0.0, 1.0, 0.0,  // 3. column
                       0.0, 0.0, 0.0, 1.0);

    // Should be equal to one of the loaded tiles using basic shader (projection_matrix * view_matrix * model_matrix)
    mat4 basic_projection_view_model = mat4(1.099960915, 0.0, 0.0, 0.0,  // 1. column
                      0.0, -1.499263641, 0.0, 0.0,                       // 2. column
                      0.0, 0.0, 0.00999999978, 0.0,                      // 3. column
                      -6194.979915,	6716.701109, 0.0, 1.0);

    //NOTE: Uncomment 1 at a time
    //vec4 pos = projection_view_matrix * vec4(aPos, -0.5f, 1.0f); // Doesn't show anything
    //vec4 pos = vec4(aPos.xy, -0.5f, 1.0f); // covers whole screen
    //vec4 pos = transMat4 * vec4(aPos.xy, -0.5f, 1.0f); // covers whole screen, but slightly translated
    //vec4 pos = scaledMat4 * vec4(aPos.xy, -0.5f, 1.0f); // covers whole screen, but slightly scaled down
    pos.z = -0.5f;
	gl_Position = pos;
	//gl_Position = vec4(aPos.xy, -0.5f, 1.0f);
	vs_out.TexCoord = aTexCoord;
}