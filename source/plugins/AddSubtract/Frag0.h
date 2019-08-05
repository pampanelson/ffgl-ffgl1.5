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


uniform float switchTex;
                                                        
uniform float float1;
void main() {
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    
    if(switchTex>0.0){
        vec4 tex= texture2D(inputTexture,gl_TexCoord[0].st);
        fragColor = vec4(1.0,0.0,0.0,1.0)*tex;


    }else{

        fragColor = vec4(vec3(sin(iTime)*float1),1.0);
    }

    

    // finish ---------------
    gl_FragColor = fragColor;
    
}
);
