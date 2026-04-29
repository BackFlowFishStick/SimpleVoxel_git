// #shader vertex
#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

in vec2 tc;

uniform mat4 mv_Matrix;
uniform mat4 proj_Matrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(mv_Matrix * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(mv_Matrix))) * aNormal;
    //Normal = normalMatrix * aNormal;
    gl_Position = proj_Matrix * mv_Matrix * vec4(aPosition, 1.0);
}

// #shader fragment
#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform int SelectedPrimitive;
void main()
{
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    float isSelected = float(gl_PrimitiveID == SelectedPrimitive);
    vec3 finalColor = mix(result, vec3(0.0, 1.0, 0.0), isSelected);
    FragColor = vec4(finalColor, 1.0);
}