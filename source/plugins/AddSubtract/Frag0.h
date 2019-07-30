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

varying float gray;
varying float a[4];
void main() {
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    vec4 tex= texture2D(inputTexture,gl_TexCoord[0].st);
    
    
    // ---------------
    fragColor = vec4(1.0,0.0,0.0,1.0)*tex;
    
    fragColor = vec4(gray);
    fragColor = vec4(a[0],a[1],a[2],a[3]);
    fragColor = vec4(vec3(sin(iTime)),1.0);
    // finish ---------------
    gl_FragColor = fragColor;
    
}
);
