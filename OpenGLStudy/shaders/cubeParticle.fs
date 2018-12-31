#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec2 UV;
in vec3 FragPos;

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;
uniform bool blinn;
uniform float gamma;

void main()
{
  const float kPi = 3.14159265;
  
  // object color
  vec2 bl = step(vec2(0.1), UV);
  vec2 tr = step(UV, vec2(0.9));
  float inside = bl.x * bl.y * tr.x * tr.y;
  vec3 objectColor = mix(color1, color2, inside);
  
  // ambient
  vec3 ambient = ambientStrength * lightColor;
  
  // diffuse
  vec3 norm = normalize(Norm);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor / kPi;
  
  // specular
  float spec;
  vec3 viewDir = normalize(viewPos - FragPos);
  if (blinn)
  {
    float energyConservation = ( 8.0 + shininess ) / ( 8.0 * kPi );
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = energyConservation * pow(max(dot(norm, halfwayDir), 0.0), shininess);
  }
  else
  {
    float energyConservation = ( 2.0 + shininess ) / ( 2.0 * kPi ); 
    vec3 reflectDir = reflect(-lightDir, norm);
    spec = energyConservation * pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  }
  vec3 specular = specularStrength * spec * lightColor; 
  
  // final result
  vec3 result = (ambient + diffuse + specular) * objectColor;
  result.rgb = pow(result.rgb, vec3(1.0/gamma));
  FragColor = vec4(result, 1.0);
}
