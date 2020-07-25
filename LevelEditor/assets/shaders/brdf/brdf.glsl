#ifndef PREAMBLE_GLSL
#define PREAMBLE_GLSL

typedef struct _Light {
	vec4 Color;
	vec4 Position;
} Light;

// Uniform locations
#define BRDF_CAMERA_POSITION 0
#define BRDF_NUM_LIGHTS 1
#define BRDF_LIGHTS 2
#define BRDF_ALBEDO_TEXTURE 3
#define BRDF_METAL_TEXTURE 4
#define BRDF_AO_TEXTURE 5
#define BRDF_HEIGHT_TEXTURE 6
#define BRDF_NORMAL_TEXTURE 7
#define BRDF_ROUGHNESS_TEXTURE 8

#endif // PREAMBLE_GLSL