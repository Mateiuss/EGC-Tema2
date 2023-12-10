#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Uniform for object hp
uniform int hp;

// Uniform for object type
uniform int obj_type;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

vec3 getColor(vec3 light_position) {
    vec3 world_pos = (Model * vec4(v_position,1)).xyz;
    vec3 world_normal = normalize(mat3(Model) * v_normal);
    vec3 L = normalize(light_position - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, world_normal);

    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);
    float specular_light = diffuse_light > 0 ? material_ks * pow(max(dot(V, R), 0), material_shininess) : 0;
    float d = distance(light_position, v_position);
    float attenuation_factor = 1 / (d * d);

	return object_color * (ambient_light + attenuation_factor * (diffuse_light + specular_light));
}

vec3 displacement(vec3 position, float hp) {
    float damage = 1 - hp / 100.0;

    return vec3(sin(position.x * 5) * damage, 0, sin(position.z * 5) * damage);
}

void main()
{
    vec3 dep = vec3(0);

    if (obj_type == 1 || obj_type == 2) {
		dep = displacement(v_position, hp);
	}

    color = getColor(light_position) + getColor(vec3(0, 7, 0));
    gl_Position = Projection * View * Model * vec4(v_position + dep, 1.0);
}
