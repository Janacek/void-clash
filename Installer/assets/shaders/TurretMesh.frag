/********************************************************************
**
**              Mesh.frag
**              Created by : Vial Joris
**
********************************************************************/

#version 330

uniform sampler2D Texture;
uniform vec3 CameraPos;

in vec2 TextureCoord0;
in vec3 Normal0;
in vec3 Position0;
in vec4 Color0;

out vec4 FragmentColor;

vec3 lightPos = vec3(Position0.xy, 75);
vec3 specularComponent = vec3(0.5);

void main()
{
        FragmentColor = texture(Texture, TextureCoord0);

        if (FragmentColor.r > FragmentColor.g + FragmentColor.b)
                FragmentColor = Color0;
        else
                FragmentColor.rgb *= 0.25;

        // Diffuse
        vec3 norm = normalize(Normal0);
        vec3 lightDir = normalize(lightPos - Position0);
        float diffuse = clamp(dot(norm, lightDir), 0.0, 1.0);

        // Specular
	vec3 viewDir = normalize(CameraPos - Position0);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 64) * 2.0;

	float res = diffuse + specular;
        FragmentColor.rgb *= res;
        FragmentColor.a = Color0.a;
}
