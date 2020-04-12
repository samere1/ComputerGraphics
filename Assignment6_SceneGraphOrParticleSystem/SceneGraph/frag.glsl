#version 330

in vec2 texCoords;

out vec4 fragColor;

// Add a sampler to retrieve our color data from
uniform sampler2D tex;

// Whether or not to use texture coords for this object
uniform bool textureExists;

void main() {
  // Set our output fragment color to whatever we pull from our input texture
  fragColor = texture(tex, texCoords);
}