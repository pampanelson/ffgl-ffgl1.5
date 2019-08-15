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

const float PI = 3.1415926535;
const float kArcLen = 0.0006;
const int num = 60;
float radius = 10000.;
const float squareWidith = 0.01;
const int  letterCol = 6;
const float colGap = 0.1;
                                                        
mat2 rotate(float rot){
    rot = (rot/360.0) * 2.0 * PI;
    return mat2(cos(rot), -sin(rot), sin(rot), cos(rot));
}
float ball(vec2 uv,vec2 p,float r){
    if(length(uv-p)<r){
        return 1.0;
    }
    return 0.0;
    
}

float square(vec2 uv,vec2 p,float width,float rot){
    uv *= rotate(rot);
    if(abs(uv.x-p.x)<width && abs(uv.y - p.y)<width){
        return 1.0;
    }
    return 0.0;
}



void main()
{
    
    
        // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    

    vec2 uv = fragCoord.xy / iResolution.xy;
    
    float wave = (sin(iTime)+1.0)*0.5;
    radius *= wave;
    uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
    //vec2 uv = fragCoord.xy/iResolution.xy;
    //vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    //st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    //0.0 and 1.0 locate on -y axis direction
    
    vec3 col = vec3(0.0); // white as default , multiply by later on
    //col += ball(uv,vec2(0.0,0.0),.1);
    //col += square(uv,vec2(0.0,0.0),.05,45.);
    
    uv.y += radius;// move down along y axis

    for(int j = 0;j < letterCol ;j++){
        uv.y += float(j) * colGap;// move down along y axis by a gap between columns
        float theta = 360.0 * kArcLen/(radius * 2.0 * PI);
        for(int i = 0;i<num;i++){
            float theta1 = theta * float(i);
            float x = sin(theta1)*radius;
            float y = cos(theta1)*radius;
            vec2 pos = vec2(x,y);
            col += square(uv,pos,squareWidith,theta1);
            pos.x *= -1.0;
            col += square(uv,pos,squareWidith,theta1);
            
        }
        
        
    }


    
    
    
    // Output to screen
    fragColor = vec4(col,1.0);
    
    

    
    gl_FragColor = fragColor;
}
);
