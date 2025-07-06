#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 uvw;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    
    vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
    uvw = aPos;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    gl_Position = gl_Position.xyww;
}
