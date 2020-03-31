#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// And our normal
in vec3 norm;
// And our fragment position for lighting
in vec3 fragPos;

// We always define a fragment color that we output.
out vec4 fragColor;

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

// Maintain our uniforms.
uniform sampler2D tex;              // our primary texture
uniform mat4 view;                  // we need the view matrix for highlights
uniform PointLight pointLights[4];  // Our lights

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  // Compute ambient light
  vec3 ambient = light.ambientIntensity * light.color;
  
  // Compute diffuse light
  vec3 lightDir = normalize(light.position - fragPos);
  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * light.color;
  
  // Compute specular lighting
  vec3 reflectDir = reflect(-lightDir, norm);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specularIntensity * spec * light.color;
  
  return (ambient + diffuseLight + specular);
}

void main() {
  // Store final texture color
  vec3 diffuseColor = texture(tex, texCoords).rgb;
  
  // Properties
  vec3 normal = normalize(norm);
  vec3 viewPos = vec3(0.0, 0.0, 0.0);
  vec3 viewDir = normalize(viewPos - fragPos);
  
  // Calculate point lights
  vec3 lighting;
  for (int i = 0; i < 4; ++i) {
	  lighting += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
  }
  
  // Final color
  fragColor = vec4(diffuseColor * lighting, 1.0);
}