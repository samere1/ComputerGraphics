#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// We always define a fragment color that we output.
out vec4 fragColor;

// Add a sampler to retrieve our color data from
uniform sampler2D tex;

// Whether or not to use texture coords for this object
uniform bool textureExists;

void main() {
  // Set our output fragment color to whatever we pull from our input texture
  fragColor = textureExists ? texture(tex, texCoords) : vec4(1.0, 1.0, 1.0, 1.0);
}