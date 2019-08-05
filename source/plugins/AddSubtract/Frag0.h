//
//  Frag0.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/7/30.
//

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform float ticks;
uniform float width;
uniform float height;
float iTime;
vec2 iResolution;
vec2 fragCoord;
uniform float switchTex;
                                                        
uniform float float1;

                                                        
mat2 myrotate(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat2(
                vec2(c, -s),
                vec2(s, c)
                );
}


void main()
{
    
    
        // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    
    // first row is frequency data (48Khz/4 in 512 texels, meaning 23 Hz per texel)
    float fft = float1;

    
    // set position
    vec2 v = iResolution.xy;
    vec2 p = fragCoord.xy;
    p = (p-v*.5) / v.y;
    p *= myrotate(iTime);
    float r = length(p);
     //breathing effect
    p += p * sin(dot(p, p)*20.-iTime) * .04;
    
     //accumulate color
    vec4 c = vec4(0.);
    for (float i = .5 ; i < 8. ; i++){

        // fractal formula and rotation
        p = abs(2.*fract(p-.5)-1.)*mat2(cos(.01*(iTime * 2.7 + fft*10.)*i*i + .78*vec4(1,7,3,1)));

        // coloration
        c += exp(-abs(p.y)*5.) * (cos(vec4(20.,30.,2.,20.)*i)*0.9+0.15) * .5;
        c += exp(-abs(p.x)*5.) * (cos(vec4(2,3,1,0)*i)*.5+.5) * .5;

    };

    // palette
    c.rg *= .5;

    
    
    fragColor = c;
    
//    fragColor = vec4(1.0);
    gl_FragColor = fragColor;
}
);




