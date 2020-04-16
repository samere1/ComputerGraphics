#version 330

// We always define a fragment color that we output
out vec4 fragColor;

// Take in inputs from vertex shader
in vec2 texCoords;
in vec3 fragPos;
in mat3 TBN;

// Define our light(s)
struct PointLight {
  vec3 color;
  vec3 position;
  float ambientIntensity;
  float specularIntensity;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

// Maintain our uniforms
uniform sampler2D diffuseMap;       // Primary texture
uniform sampler2D normalMap;        // Normal texture
uniform PointLight pointLight;      // Lights
uniform bool textureExists;         // Whether or not to use texture coords for this object

void main() {
  // Obtain normal from normal map in range [0, 1]
  vec3 normal = texture(normalMap, texCoords).rgb;
  // Transform normal vector to range [-1, 1]
  normal = normalize(normal * 2.0 - 1.0); // Normal is in tangent space
  
  // Store final texture color
  vec3 diffuseColor = textureExists ? texture(diffuseMap, texCoords).rgb : vec3(1.0, 1.0, 1.0);  
  
  // Properties
  vec3 tangentFragPos = TBN * fragPos;
  vec3 viewPos = vec3(0.0, 0.0, 0.0);
  vec3 viewDir = TBN * normalize((TBN * viewPos) - tangentFragPos);
  
  // Compute ambient light
  vec3 ambient = pointLight.ambientIntensity * pointLight.color;
  
  // Compute diffuse light
  vec3 lightDir = normalize((TBN * pointLight.position) - tangentFragPos);
  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * pointLight.color;
  
  // Compute specular lighting
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
  vec3 specular = pointLight.specularIntensity * spec * pointLight.color;
  
  // Combine lights
  vec3 lighting = ambient + diffuseLight + specular;
  
  // Final color
  fragColor = vec4(diffuseColor * lighting, 1.0);
}
