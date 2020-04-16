#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec3 tangent;

// Camera system
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Outputs
out vec2 texCoords;
out vec3 fragPos;
out mat3 TBN;

void main()
{
	// Our fragment pos for lighting
	fragPos = vec3(modelMatrix * vec4(position, 1.0));
	// Map texture coordinates
	texCoords = textureCoords;
	
	// Create the TBN matrix
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 T = normalize(normalMatrix * normalize(tangent));
	vec3 N = normalize(normalMatrix * normalize(normal));
	// Re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// Retrieve bitangent B with the cross product of T and N
	vec3 B = cross(N, T);
	TBN = transpose(mat3(T, B, N));

	// Transformed position
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}