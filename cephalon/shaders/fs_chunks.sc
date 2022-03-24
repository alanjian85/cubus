$input v_position, v_normal, v_color0, v_color1, v_texcoord0, v_texcoord1

#include <bgfx_shader.sh>

uniform vec4 u_fog;

SAMPLER2D(s_atlas, 0);
SAMPLER2D(s_heightmap, 1);

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 lightDir = vec3(-0.2, -1.0, 0.3);

	float ambientStrength = 0.4f;
	vec3 ambient = ambientStrength * vec3(v_color0) * vec3(texture2DBias(s_atlas, v_texcoord0, -3.5));

	float diff = max(dot(normal, -lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture2DBias(s_atlas, v_texcoord0, -3.5));

	float fogNear = u_fog.x;
	float fogFar = u_fog.y;
	float fogMin = u_fog.z;
	float fogMax = u_fog.w;

	float distance = length(vec3(v_position));
	float intensity = clamp(
		(fogFar - distance) / (fogFar - fogNear),
		fogMin,
		fogFar
	);

	float height = texture2D(s_heightmap, v_texcoord1).r;
	float shadow = v_color1 < height ? 1.0 : 0.0;

	gl_FragColor = vec4((ambient + diffuse) * (1.0 - shadow * 0.5), intensity);
}