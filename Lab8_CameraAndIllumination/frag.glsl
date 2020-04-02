#version 330

// Inputs from vertex shader
in vec2 texCoords;
in vec3 norm;
in vec3 fragPos;

// Fragment color that we output
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

#define NUM_POINT_LIGHTS 4

// Maintain our uniforms
uniform sampler2D tex;              // Primary texture
uniform PointLight pointLights[NUM_POINT_LIGHTS];  // Lights

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  // Ambient light
  vec3 ambient = light.ambientIntensity * light.color;
  
  // Diffuse light
  vec3 lightDir = normalize(light.position - fragPos);
  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * light.color;
  
  // Specular lighting
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specularIntensity * spec * light.color;
  
  // Attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  ambient *= attenuation;
  diffuseLight *= attenuation;
  specular *= attenuation;
  
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
  for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
	  lighting += CalcPointLight(pointLights[i], normal, fragPos, viewDir);
  }
  
  // Final color
  fragColor = vec4(diffuseColor * lighting, 1.0);
}