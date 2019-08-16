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
const float kArcLen = 0.0004;
const int num = 100;
float radius = 10000.;
const float squareWidith = 0.01;

float lineNum = 80.0;
float lineGap = 0.03;
float lineWidith = 0.3;
float offsetY = 0.2;
float lineSaturation = 4.0;
float cheapstep(float x)
{
    x = 1.0 - x*x;    // MAD
    x = 1.0 - x*x;    // MAD
    return x;
}
                                                        

float smtLine(float lineWidith,float f){
    float res;
    res = smoothstep(lineWidith,0.0,f);
    res *= smoothstep(0.0,lineWidith,f)*lineSaturation;
    return res;
}

                                                        


void main()
{
    
    
        // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    

//    vec2 uv = fragCoord.xy / iResolution.xy;
    
    vec3 col;
    
    vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
    uv.y += 0.5;
    //vec2 uv = fragCoord.xy/iResolution.xy;
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    
    
    // if(st1.x >= 0.25 && st.x <= 0.75){
    
    vec2 st1 = st;
    
    //float wave = -(sin(st.y*0.05*iTime) + 1.0)*0.5;
    float index = floor(st1.y * lineNum);
    //float wave = sin(0.0008*iTime*index)*sin(index);
    
    
    //st1.y += wave;
    
    float f = fract(st1.y * lineNum);
    
    // ripples -----------------------------
    float wave = sin(0.02*iTime*index*4.0*PI);
    //f += 0.06*clamp(0.2,0.8,wave);
    //wave = max(0.1,wave);
    f += 0.05 * wave;
    
    //------------------------------------------
    
    
    
    float line = smtLine(lineWidith,f);
    col = vec3(line);
    
    
    //}
    
    
    if(st.y <= offsetY){
        col *= 0.0;
    }
    


    // Output to screen
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);


// ================================    2.1

//vec3 col;
//
//vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
//uv.y += 0.5;
////vec2 uv = fragCoord.xy/iResolution.xy;
//vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
//
//
//// if(st1.x >= 0.25 && st.x <= 0.75){
//
//vec2 st1 = st;
//
////float wave = -(sin(st.y*0.05*iTime) + 1.0)*0.5;
//float index = floor(st1.y * lineNum);
//
//// not use yet ---------------------------------------------------
////float wave = sin(0.0008*iTime*index)*sin(index);
////st1.y += wave;
//
//float f = fract(st1.y * lineNum);
//f += 0.03*sin(0.0001*iTime*index*index*index);
//float line = smtLine(lineWidith,f);
//col = vec3(line);
//
//
////}
//
//
//if(st.y <= offsetY){
//    col *= 0.0;
//}
//
//



// ================================    2.0
//vec3 col;
//
//vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
//uv.y += 0.5;
////vec2 uv = fragCoord.xy/iResolution.xy;
//vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
//
//st.y += sin(iTime);
//if(st.x >= 0.25 && st.x <= 0.75){
//    float f = fract(st.y * lineNum);
//    float line = smtLine(lineWidith,f);
//    col = vec3(line);
//
//
//}
//
//
//if(st.y <= offsetY){
//    col *= 0.0;
//}
//
//
//
//// Output to screen
//fragColor = vec4(col,1.0);
//






// =======================================================
//    float wave = (sin(iTime)+1.0)*0.5;
//    float wave = fract(iTime);
//    radius *= wave;
//    uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
//    //vec2 uv = fragCoord.xy/iResolution.xy;
//    //vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//    //st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
//    //0.0 and 1.0 locate on -y axis direction
//
//    vec3 col = vec3(0.0); // white as default , multiply by later on
//    //col += ball(uv,vec2(0.0,0.0),.1);
//    //col += square(uv,vec2(0.0,0.0),.05,45.);
//
//    uv.y += radius - 0.5;// move down along y axis
//
//    for(int j = 0;j < letterCol ;j++){
//        uv.y += colGap;// move down along y axis by a gap between columns
//        float theta = 360.0 * kArcLen/(radius * 2.0 * PI);
//        for(int i = 0;i<num;i++){
//            float theta1 = theta * float(i);
//            float x = sin(theta1)*radius;
//            float y = cos(theta1)*radius;
//
////            float x = 0.0001*radius;
////            float y = 0.0001*radius;
//
//
//            vec2 pos = vec2(x,y);
//            col += square(uv,pos,squareWidith,theta1);
//            pos.x *= -1.0;
//            col += square(uv,pos,squareWidith,theta1);
//
//        }
//
//
//    }
//


