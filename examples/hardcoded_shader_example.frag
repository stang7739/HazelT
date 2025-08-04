// EXAMPLE: HARDCODED shader (BAD PRACTICE - DON'T DO THIS)
// This demonstrates what NOT to do as requested in the problem statement

#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_Texture;

void main()
{
    // BAD: Hardcoded values that cannot be changed from CPU
    vec3 lightPosition = vec3(2.0, 2.0, 2.0);  // HARDCODED
    vec3 lightColor = vec3(1.0, 1.0, 1.0);     // HARDCODED
    vec3 viewPosition = vec3(0.0, 0.0, 3.0);   // HARDCODED
    
    vec3 materialAmbient = vec3(0.2, 0.2, 0.2);   // HARDCODED
    vec3 materialDiffuse = vec3(0.8, 0.8, 0.8);   // HARDCODED
    vec3 materialSpecular = vec3(1.0, 1.0, 1.0);  // HARDCODED
    float materialShininess = 32.0;               // HARDCODED
    
    float deformationStrength = 1.0;  // HARDCODED DM parameter
    float frameworkIntensity = 0.5;   // HARDCODED FW parameter
    vec3 core3DColor = vec3(0.5, 0.7, 1.0); // HARDCODED
    
    // Calculate lighting vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Lighting calculations with hardcoded values
    vec3 ambient = materialAmbient * lightColor;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * materialDiffuse * lightColor;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = spec * materialSpecular * lightColor;
    
    // Effects with hardcoded parameters
    float deformation = sin(FragPos.x * deformationStrength) * 0.1;
    float framework = frameworkIntensity * length(FragPos);
    
    vec3 result = ambient + diffuse + specular;
    result *= (1.0 + deformation);
    result = mix(result, core3DColor, framework * 0.1);
    
    vec4 texColor = texture(u_Texture, TexCoord);
    FragColor = vec4(result * texColor.rgb, texColor.a);
}

/*
PROBLEMS WITH HARDCODED APPROACH:
1. Cannot change parameters at runtime
2. Need to recompile shader for any adjustments
3. No flexibility for different materials
4. Difficult to debug and tune
5. Cannot respond to user input or game state
6. Poor maintainability

This is exactly what the problem statement wants to AVOID!
*/