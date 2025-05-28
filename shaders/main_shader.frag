#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Flashlight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outerCutoff;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec4 FragPos;
in vec3 ModelPos;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform DirectionalLight dirLight;
uniform PointLight posLight;
uniform Flashlight flashlight;
uniform vec3 edge;
uniform bool mouseDown;
uniform bool useTexture;
uniform vec4 fillColor;
uniform vec4 borderColor;
uniform bool useMultiTextures;
uniform bool focused;
uniform bool selected;
uniform float borderWidth;
uniform bool showGrid;
uniform bool showPlaceQuad;
uniform vec2 placeQuadSize;
uniform vec2 placeQuadPos;
uniform vec3 worldPos;
uniform vec3 worldSize;
uniform float radius;
uniform float desaturate;

void main()
{
    // Ambient light (there is always some baseline amount of light in a scene)
    //float ambientStrength = 0.1;


    vec4 color = vec4(0);

    if (useMultiTextures) {
        if (ModelPos.z == 0.5 || ModelPos.z == -0.5) {
            color = texture(tex1, TexCoord);
        }
        else if (ModelPos.x == 0.5 || ModelPos.x == -0.5) {
            color = texture(tex2, TexCoord);
        }
        else if (ModelPos.y == 0.5 || ModelPos.y == -0.5) {
            color = texture(tex3, TexCoord);
        }
    } else {
        color = texture(tex1, TexCoord);
    }

    if (!useTexture) {
        color = fillColor;
    }
    FragColor = color;
    FragPos;

    vec4 selCol = mix(color, vec4(mouseDown ? 0.6 : 1, 0, 0, 1), 0.5);

    if (edge.z != 0) {
        if (ModelPos.z == edge.z) {
            FragColor = selCol;
        }
    } else if (edge.x != 0) {
        if (ModelPos.x == edge.x) {
            FragColor = selCol;
        }
    } else if (edge.y != 0) {
        if (ModelPos.y == edge.y) {
            FragColor = selCol;
        }
    }

    //    if (selected) {
    //        float thresh = 0.4;
    //        if (ModelPos.x < -thresh || ModelPos.x > thresh || ModelPos.y > thresh || ModelPos.y < -thresh) {
    //            float f = mouseDown ? 0.6f : 1.f;
    //            FragColor = vec4(borderColor.xyz * f, 1.f);
    //        }
    //    }
    //
    //
    //    if (focused) {
    vec3 thresh = vec3(borderWidth, borderWidth, borderWidth);
    vec4 bc = borderColor;


    vec3 adjPos = FragPos.xyz - worldPos;

    if (showPlaceQuad) {
        thresh = -worldSize / 2.f + 0.003f;
        thresh *= FragPos.w * FragPos.w * FragPos.w;
        bc = vec4(1, 1, 1, 1.f);

        if (
        (adjPos.x < thresh.x || adjPos.x > -thresh.x || adjPos.y < thresh.y || adjPos.y > -thresh.y) &&
        (adjPos.x < thresh.x || adjPos.x > -thresh.x || adjPos.z < thresh.z || adjPos.z > -thresh.z) &&
        (adjPos.y < thresh.y || adjPos.y > -thresh.y || adjPos.z < thresh.z || adjPos.z > -thresh.z)
        ){
            float f = mouseDown ? 0.7f : 1.f;
            FragColor = vec4(bc.xyz * f, 1.f);
        }
    } else if (borderWidth > 1.f){
        thresh = vec3(worldSize) / 2.f - vec3(borderWidth);
        if (
        (adjPos.x < -thresh.x|| adjPos.x > thresh.x || adjPos.y < -thresh.y || adjPos.y > thresh.y)
        ){
            float f = mouseDown ? 0.7f : 1.f;
            FragColor = vec4(bc.xyz * f, bc.a);
        }

    }



    float subGridStep = 0.1f;
    float gridStep = 0.5f;
    float gridWidth = 0.003f;

    if (showGrid) {
        if (mod(FragPos.x, gridStep) < gridWidth || mod(FragPos.z, gridStep) < gridWidth) {
            FragColor = color;
        } else if (mod(FragPos.x, subGridStep) < gridWidth || mod(FragPos.z, subGridStep) < gridWidth) {
            FragColor = color * 1.3f;
        } else {
            FragColor = vec4(0, 0, 0, 0);
        }
    }

    vec3 normWorldPos = FragPos.xyz - worldPos + vec3(placeQuadPos, placeQuadPos.y);



    if (showPlaceQuad) {
        if (
        (normWorldPos.x < placeQuadSize.x / 2.f &&
        normWorldPos.x > -placeQuadSize.x / 2.f &&
        normWorldPos.z < placeQuadSize.y / 2.f &&
        normWorldPos.z > -placeQuadSize.y / 2.f)
        ) {
            FragColor = mix(FragColor, vec4(0.6, 0.6, 0.6, 1), 0.4f);
        }

        //        if (
        //        (ModelPos.x < -0.45 || ModelPos.x > 0.45) ||
        //        (ModelPos)
        //        (ModelPos.z < -0.45 || ModelPos.z > 0.45)) {
        //            FragColor = mix(FragColor, vec4(0, 1, 1, 1), 0.7f);
        //        }
    }

    if (radius > 0.0001f) {
        float Pi = 6.28318530718;// Pi*2

        // GAUSSIAN BLUR SETTINGS {{{
        float Directions = 16.0;// BLUR DIRECTIONS (Default 16.0 - More is better but slower)
        float Quality = 3.0;// BLUR QUALITY (Default 4.0 - More is better but slower)
        float Size = radius;// BLUR SIZE (Radius)
        // GAUSSIAN BLUR SETTINGS }}}

        vec2 Radius = Size/worldSize.xy;

        // Normalized pixel coordinates (from 0 to 1)
        vec2 uv = TexCoord;
        // Pixel colour
        vec4 Color = texture(tex1, uv);

        // Blur calculations
        for (float d=0.0; d<Pi; d+=Pi/Directions)
        {
            for (float i=1.0/Quality; i<=1.001; i+=1.0/Quality)
            {
                Color += texture(tex1, uv+vec2(cos(d), sin(d))*Radius*i);
            }
        }

        // Output to screen
        Color /= Quality * Directions +1.0;
        FragColor = Color;
    }


    float c = (FragColor.x + FragColor.y + FragColor.z) / 3.f;
    vec4 gray = vec4(c, c, c, FragColor.a);
    FragColor = mix(FragColor, gray, desaturate);


    return;
    //
    //    float distance    = length(posLight.position - FragPos);
    //    float attenuation = 1.0 / (posLight.constant + posLight.linear * distance + posLight.quadratic * (distance * distance));
    //
    //    vec3 ambient = flashlight.ambient * color;
    //    //ambient *= attenuation;
    //
    //    // Diffuse lightinge
    //    vec3 norm = normalize(Normal);
    //    //vec3 lightDir = normalize(-dirLight.direction);
    //    vec3 lightDir = normalize(flashlight.position - FragPos);
    //
    //    float theta = dot(lightDir, normalize(-flashlight.direction));
    //    float epsilon   = flashlight.cutoff - flashlight.outerCutoff;
    //    float intensity = clamp((theta - flashlight.outerCutoff) / epsilon, 0.0, 1.0);
    //
    //    //if (theta > flashlight.outerCutoff) {
    //
    //        float diff = max(dot(norm, lightDir), 0.0);
    //        vec3 diffuse = flashlight.diffuse * (diff * color);
    //        //diffuse *= attenuation;
    //        diffuse *= intensity;
    //
    //        // Specular lighting
    //        vec3 viewDir = normalize(viewPos - FragPos);
    //        vec3 reflectDir = reflect(-lightDir, norm);
    //        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //        vec3 specular = flashlight.specular * (material.specular * spec);
    //        //specular *= attenuation;
    //        specular *= intensity;
    //
    //        FragColor = vec4((ambient + diffuse + specular), 1.0);
    //
    ////    } else {
    ////
    ////        FragColor = vec4(ambient * color, 1.0);
    ////    }
}