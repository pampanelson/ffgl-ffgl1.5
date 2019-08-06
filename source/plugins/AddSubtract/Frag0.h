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
vec2 iResolution;
vec2 fragCoord;
uniform float switchTex;

uniform float float1;


void main()
{
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    vec4 O;
    float t = iTime;
    float v;
    float g = 0.;
    vec2 U = fragCoord;
    mat2  R = mat2( sin(t+vec4(0,33,11,0)) );
    vec3  q = vec3(iResolution.x,iResolution.y,1.0+float1*5.0);
    vec3 D = vec3(.3*(U+U-q.xy)/q.y, -1);    // ray direction
    vec3 p = 30./q;
    vec3 a;                       // marching point along ray
    O-=O;
    // step forward = dist to obj
//
    for ( O++; O.x > 0. && t > .01 ; O-=.015 ){
        q = p;
        q.xz *= R;
        q.yz *= R;                              // rotation
        a = abs(q); // v = C - 5.,            // bounding cube
        a = abs(a-3.);
        t = max(max(a.x,a.y),a.z);              // 8 small cubes
        t = min(t, ( a = abs(q), a.x/=5., max(max(a.x,a.y),a.z) ) );
        t = min(t, ( a = abs(q), a.y/=5., max(max(a.x,a.y),a.z) ) );
        t = min(t, ( a = abs(q), a.z/=5., max(max(a.x,a.y),a.z) ) );
        t -= 1.;
        // 3 boxes
        p += t*D;
    }

    if(switchTex > 0.0){
        O *= texture2D(inputTexture,p.xy*0.1+0.5);
    }

    fragColor = O;
//    fragColor = vec4(1.0);
    //    fragColor = vec4(1.0,0.0,0.0,1.0);
    // finish ---------------
    gl_FragColor = fragColor;
    
    
}
);



    
