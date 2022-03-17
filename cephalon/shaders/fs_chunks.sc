$input v_normal, v_color0, v_color1

#include <bgfx_shader.sh>

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 lightDir = vec3(-0.2, -1.0, 0.3);

	float ambientStrength = 0.3f;
	vec3 ambient = ambientStrength * vec3(v_color0) * vec3(v_color1);

	float diff = max(dot(normal, -lightDir), 0.0);
	diff = 1.0;
	vec3 diffuse = diff * vec3(v_color1);

	gl_FragColor = vec4(ambient + diffuse, 1.0);
}