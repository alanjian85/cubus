$input v_normal, v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_atlas, 0);

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 lightDir = vec3(-0.2, -1.0, 0.3);

	float ambientStrength = 0.4f;
	vec3 ambient = ambientStrength * vec3(v_color0) * vec3(texture2D(s_atlas, v_texcoord0));

	float diff = max(dot(normal, -lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture2D(s_atlas, v_texcoord0));

	gl_FragColor = vec4(ambient + diffuse, 1.0);
}