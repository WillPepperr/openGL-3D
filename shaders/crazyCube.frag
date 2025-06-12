#version 330 core
out vec4 FragColor;

in vec3 ObjectPos;
in vec3 ObjectNormal;
uniform float iTime;

vec2 getTriplanarUV(vec3 pos, vec3 normal) {
    vec3 blend = abs(normal);
    blend = pow(blend, vec3(4.0)); // make blend sharper
    blend /= (blend.x + blend.y + blend.z);

    vec2 uvX = pos.zy; // projection onto YZ
    vec2 uvY = pos.xz; // projection onto XZ
    vec2 uvZ = pos.xy; // projection onto XY

    return uvX * blend.x + uvY * blend.y + uvZ * blend.z;
}

void main()
{
    vec2 u = getTriplanarUV(ObjectPos, ObjectNormal) * 1.0f;

    vec4 o = vec4(1, 2, 3, 0);
    vec4 z = o;

    float a = 0.5;
    float t = iTime;
    float i = 0.0;

    for (; i < 19.0; ++i)
    {
        o += (1.0 + cos(z + t)) / length(
                    (1.0 + i * dot(u, u)) *
                        sin(1.5 * u / (0.5 - dot(u, u)) - 9.0 * u.yx + t)
                );

        vec2 tmp = cos(++t - 7.0 * u * pow(a += 0.03, i)) - 5.0 * u;
        u += tanh(
                40.0 * dot(
                        u *= mat2(cos(i + 0.02 * t - vec4(0, 11, 33, 0))),
                        u
                    ) * cos(100.0 * u.yx + t)
            ) / 200.0
                + 0.2 * a * u
                + cos(4.0 / exp(dot(o, o) / 100.0) + t) / 300.0;
    }

    o = 25.6 / (min(o, vec4(13.0)) + 164.0 / o) - dot(u, u) / 250.0;

    FragColor = o;
}
