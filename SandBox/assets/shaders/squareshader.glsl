#type vertex
#version 330 core
 layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


uniform mat4 u_ViewProjection;
uniform mat4 u_tranform;
out vec3 v_Position;
out vec2 v_TexCoord;

void main()
{
v_Position = a_Position;
v_TexCoord = a_TexCoord;

gl_Position = u_ViewProjection * u_tranform * vec4(a_Position, 1.0);
}
#type fragment
#version 330 core
 layout(location = 0) out vec4 color;
in vec3 v_Position;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;
void main()
{
color = texture(u_Texture,v_TexCoord); // Set the color to white
}