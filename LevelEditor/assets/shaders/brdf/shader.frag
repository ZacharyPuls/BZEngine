
in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fWorldPosition;

out vec4 FragColor;

layout(binding = BRDF_CAMERA_POSITION)
uniform vec3 CAMERA_POSITION;

layout(binding = BRDF_NUM_LIGHTS)
uniform int NUM_LIGHTS;

layout(binding = BRDF_LIGHTS)
uniform Light LIGHTS[NUM_LIGHTS];

layout (binding = BRDF_ALBEDO_TEXTURE)
uniform sampler2D albedoTexture;

layout (binding = BRDF_METAL_TEXTURE)
uniform sampler2D metalTexture;

layout (binding = BRDF_AO_TEXTURE)
uniform sampler2D aoTexture;

layout (binding = BRDF_HEIGHT_TEXTURE)
uniform sampler2D heightTexture;

layout (binding = BRDF_NORMAL_TEXTURE)
uniform sampler2D normalTexture;

layout (binding = BRDF_ROUGHNESS_TEXTURE)
uniform sampler2D roughnessTexture;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}  

void main()
{
    vec3 N = normalize(fNormal);
    vec3 V = normalize(CAMERA_POSITION - fWorldPosition);
    vec3 Lo = vec3(0.0f);

    for (int i = 0; i < NUM_LIGHTS; ++i) {
        vec3 L = normalize(LIGHTS[i].Position - fWorldPosition);
        vec3 H = normalize(V + L);

        float distance = length(LIGHTS[i].Position - fWorldPosition);
        float attenuation = 1.0f / (distance * distance);
        vec3 radiance = LIGHTS[i].Color * attenuation;

        vec3 F0 = vec3(0.04f); 
        F0 = mix(F0, albedo, metallic);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    }
}