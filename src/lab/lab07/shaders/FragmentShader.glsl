#version 330

#define M_PI 3.1415926535897932384626433832795

// Input
// TODO(student): Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 uv_coord;

// Uniforms for light properties
uniform float point_lights_count;
uniform vec3 point_light_positions[10];
uniform vec3 point_light_colors[10];
uniform vec3 spot_light_positions[10];
uniform vec3 spot_light_colors[10];
uniform vec3 spot_light_directions[10];
uniform float spot_light_angles[10];
uniform vec3 eye_position;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;
uniform int bonus;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;

vec3 ka;
vec3 kd;
vec3 ks;

vec3 ComputePhongIllumination(vec3 light_position, vec3 light_color)
{
    // TODO(student): Compute the diffuse component of the Lambert illumination model
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 diffuse_component = kd * light_color * max(dot(N, L), 0);

    // TODO(student): Compute the specular component of the Phong illumination model
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, N);
    vec3 specular_component = ks * light_color * pow(max(dot(V, R), 0), material_shininess);

    if (length(diffuse_component) > 0)
    {

    }

    // TODO(student): Compute the final illumination as the sum of the diffuse and specular components
    vec3 illumination = diffuse_component + specular_component;

    return illumination;
}

float ComputeDistanceAttenuation(vec3 light_position, vec3 point_position)
{
    // TODO(student): Compute the light attenuation factor based on the distance
    // between the position of the illuminated point and the position of the light source.
    float d = distance(light_position, point_position);
    return 1.0 / (d * d + 1.0);
}

vec3 ComputePointLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<10;i++) {
        vec3 light_position = point_light_positions[i];
        vec3 light_color = point_light_colors[i];

        vec3 light_illumination = ComputePhongIllumination(light_position, light_color);
        float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

        // TODO(student): Add to the illumination of all light sources the result
        // of multiplying the illumination of the light source from the current iteration
        // with the attenuation of the illumination and the color of the illumination.
        lights_illumination += illumination_attenuation * light_illumination;
    }

    return lights_illumination;
}

vec3 ComputeSpotLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<10;i++) {
        vec3 light_position = spot_light_positions[i];
        vec3 light_color = spot_light_colors[i];
        vec3 light_direction = spot_light_directions[i];
        float cut_off_angle = spot_light_angles[i];

        vec3 L = normalize(light_position-world_position);
        float cos_theta_angle = dot(-L, light_direction);
        float cos_phi_angle = cos(cut_off_angle);
        if (cos_theta_angle > cos_phi_angle)
        {
            vec3 light_illumination = ComputePhongIllumination(light_position, light_color);
            float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

            // TODO(student): Compute the attenuation factor specific to the spot light source
            float spot_linear_att_factor = (cos_theta_angle - cos_phi_angle) / (1.0 - cos_phi_angle);
            float quadratic_spot_light_att_factor = pow(spot_linear_att_factor, 2);

            // TODO(student): Add to the illumination of all light sources the result
            // of multiplying the illumination of the light source from the current iteration
            // with the attenuation of the illumination, the attenuation factor specific
            // to the light spot source and the color of the illumination.
            lights_illumination += illumination_attenuation * quadratic_spot_light_att_factor * light_illumination;
        }
    }

    return lights_illumination;
}

vec3 ComputeAmbientComponent()
{
    vec3 global_ambient_color = vec3(0.25f);

    // TODO(student): Compute the ambient component of global illumination
    vec3 ambient_component = ka * global_ambient_color;

    return ambient_component;
}

void main()
{
    if (bonus == 1) {
        vec2 uv = uv_coord;
        uv = uv * 2 - 1;

        float d = length(uv);
        d -= 0.3;

        float anim = sin(2 * time) * 0.25 + 0.25;
        //float anim = (2.0 / M_PI) * (asin(sin(2 * time))) * 0.25 + 0.25;
        d = step(anim, d);

        ka = vec3(1, d, 0);
        ks = vec3(1, d, 0);
        kd = vec3(1, d, 0);
    }
    else {
        ka = material_ka;
        ks = material_ks;
        kd = material_kd;
    }

    vec3 illumination = ComputePointLightSourcesIllumination()
        + ComputeSpotLightSourcesIllumination ()
        + ComputeAmbientComponent();

    out_color = vec4 (illumination, 1);
}
