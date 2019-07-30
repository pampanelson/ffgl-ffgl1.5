//
//  Frag0.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/7/30.
//

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform vec3 brightness;
uniform float ticks;
uniform float width;
uniform float height;
float iTime;

                                                        
                                                        

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;
const vec3 cubePos = vec3(1.0,2.0,-5.0); // for text only
const float cubeHalfWidth = 0.5;// fot test only




mat3 rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
                vec3(1, 0, 0),
                vec3(0, c, -s),
                vec3(0, s, c)
                );
}



mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
                vec3(c, 0, s),
                vec3(0, 1, 0),
                vec3(-s, 0, c)
                );
}


mat3 rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
                vec3(c, -s, 0),
                vec3(s, c, 0),
                vec3(0, 0, 1)
                );
}



float cubeSDF(vec3 p) {
    // If d.x < 0, then -1 < p.x < 1, and same logic applies to p.y, p.z
    // So if all components of d are negative, then p is inside the unit cube

    vec3 d = abs(p-cubePos) - vec3(cubeHalfWidth);

    // Assuming p is inside the cube, how far is it from the surface?
    // Result will be negative or zero.
    float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);

    // Assuming p is outside the cube, how far is it from the surface?
    // Result will be positive or zero.
    float outsideDistance = length(max(d, 0.0));

    return insideDistance + outsideDistance;
}


float sphereSDF(vec3 p) {

    return length(p) - 1.0;
}


float sceneSDF(vec3 samplePoint) {
    samplePoint -= cubePos;
    samplePoint = rotateY(iTime / 2.0) * samplePoint;
    samplePoint += cubePos;
    return cubeSDF(samplePoint);
}


float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(eye + depth * marchingDirection);
        if (dist < EPSILON) {
            return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}



vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}


vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
                          sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
                          sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
                          sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
                          ));
}


vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye,
                          vec3 lightPos, vec3 lightIntensity) {
    vec3 N = estimateNormal(p);
    vec3 L = normalize(lightPos - p);
    vec3 V = normalize(eye - p);
    vec3 R = normalize(reflect(-L, N));

    float dotLN = dot(L, N);
    float dotRV = dot(R, V);

    if (dotLN < 0.0) {
        // Light not visible from this point on the surface
        return vec3(0.0, 0.0, 0.0);
    }

    if (dotRV < 0.0) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        return lightIntensity * (k_d * dotLN);
    }
    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}



mat4 myViewMatrix(vec3 eye, vec3 center, vec3 up) {
    // Based on gluLookAt man page
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat4(
                vec4(s, 0.0),
                vec4(u, 0.0),
                vec4(-f, 0.0),
                vec4(0.0, 0.0, 0.0, 1)
                );
}


vec3 myCubColor(vec3 p,vec3 eye){
    vec3 c = vec3(0.0,0.0,0.0);
    vec3 p1 = p;
    p1 -= cubePos;
    p1 *= rotateY(-iTime / 2.0);

    if(abs(p1.z) > abs(cubeHalfWidth - EPSILON)){

        c = texture2D(inputTexture,(p1.xy+.5)).xyz;

    }
    if(abs(p1.x) > abs(cubeHalfWidth - EPSILON)){

        c = texture2D(inputTexture,(p1.yz+.5)).xyz;

    }
    if(abs(p1.y) > abs(cubeHalfWidth - EPSILON)){

        c = texture2D(inputTexture,(p1.xz + .5)).xyz;

    }

    //p1+=cubePos;

    return c;
}


vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {
    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);
    vec3 color = ambientLight * k_a;

    vec3 light1Pos = vec3(4.0 * sin(iTime),
                          2.0,
                          4.0 * cos(iTime));
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light1Pos,
                                  light1Intensity);

    vec3 light2Pos = vec3(2.0 * sin(0.37 * iTime),
                          2.0 * cos(0.37 * iTime),
                          2.0);
    vec3 light2Intensity = vec3(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
                                  light2Pos,
                                  light2Intensity);
    return color;
}


void main()
{

    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;

//    vec4 tex= texture2D(inputTexture,gl_TexCoord[0].st);




    vec3 viewDir = rayDirection(45.0, iResolution.xy, fragCoord);
    vec3 eye = vec3(0.0,0.0,10.);

    mat4 viewToWorld = myViewMatrix(eye, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

    vec3 worldDir = (viewToWorld * vec4(viewDir, 0.0)).xyz;
    float dist = shortestDistanceToSurface(eye, worldDir, MIN_DIST, MAX_DIST);

    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    // The closest point on the surface to the eyepoint along the view ray
    vec3 p = eye + dist * worldDir;

    vec3 K_a = vec3(1., 1., 1.);
    vec3 K_d = vec3(0.7, 0.2, 0.2);
    vec3 K_s = vec3(1.0, 1.0, 1.0);
    float shininess = 10.0;


    vec3 color = myCubColor(p,eye);

    color *= phongIllumination(K_a, K_d, K_s, shininess, p, eye);
    fragColor = vec4(color, 1.0);

//    fragColor = vec4(1.0,0.0,0.0,1.0);
    // finish ---------------
    gl_FragColor = fragColor;


}

                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
//varying float gray;
//varying float a[4];
//void main() {
//
//    // name convert ---------------
//    vec4 fragColor;
//    iTime = ticks/1000.0;
//    vec2 iResolution = vec2(width,height);
//    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
//
//    vec4 tex= texture2D(inputTexture,gl_TexCoord[0].st);
//
//
//    // ---------------
//    fragColor = vec4(1.0,0.0,0.0,1.0)*tex;
//
//    fragColor = vec4(gray);
//    fragColor = vec4(a[0],a[1],a[2],a[3]);
//    fragColor = vec4(vec3(sin(iTime)),1.0);
//    // finish ---------------
//    gl_FragColor = fragColor;
//
//}
);
