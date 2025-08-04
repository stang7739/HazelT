#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// CPU-controlled uniforms instead of hardcoded values
uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;

uniform vec3 u_MaterialAmbient;
uniform vec3 u_MaterialDiffuse;
uniform vec3 u_MaterialSpecular;
uniform float u_MaterialShininess;

uniform float u_DeformationStrength;  // DM - Deformation parameter
uniform float u_FrameworkIntensity;   // FW - Framework parameter
uniform vec3 u_Core3DColor;           // Core 3D color

uniform sampler2D u_Texture;

void main()
{
    // Calculate lighting vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPosition - FragPos);
    vec3 viewDir = normalize(u_ViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Ambient lighting - CPU controlled
    vec3 ambient = u_MaterialAmbient * u_LightColor;
    
    // Diffuse lighting - CPU controlled
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_MaterialDiffuse * u_LightColor;
    
    // Specular lighting - CPU controlled
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_MaterialShininess);
    vec3 specular = spec * u_MaterialSpecular * u_LightColor;
    
    // Deformation effect - CPU controlled instead of hardcoded
    float deformation = sin(FragPos.x * u_DeformationStrength) * 0.1;
    
    // Framework intensity - CPU controlled
    float framework = u_FrameworkIntensity * length(FragPos);
    
    // Combine all effects
    vec3 result = ambient + diffuse + specular;
    result *= (1.0 + deformation);
    result = mix(result, u_Core3DColor, framework * 0.1);
    
    // Sample texture with CPU-controlled color mixing
    vec4 texColor = texture(u_Texture, TexCoord);
    
    FragColor = vec4(result * texColor.rgb, texColor.a);
}