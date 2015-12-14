#version 330
in vec3 position;
in vec4 vertexColor;
out vec4 fragmentColor;
uniform vec2 Pos1 = vec2(-0.5, -0.5);
uniform vec2 ballPos = vec2(-0.5, -0.5);


uniform mat4 modelMatrix      = mat4(1.0);
uniform mat4 viewMatrix       = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

  

void main()
{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
		fragmentColor = vertexColor;

		gl_Position = vec4(position + translationVector, 0.0, 1.0);
		fragmentColor = vertexColor;

 		gl_Position = vec4(position + ballVector, 0.0, 1.0);
}

