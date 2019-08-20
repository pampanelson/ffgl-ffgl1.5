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
//float lineNumF = 80.0;
float lineWidith = 0.003;
float offsetY = 0.2;



// Polynomial smooth min (for copying and pasting into your shaders)
float smin(float a, float b, float k) {
    float h = clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
    return mix(a, b, h) - k*h*(1.0-h);
}

float smax(float a,float b,float k){
    return smin(a,b,-k);
}

float hash1(float s){
    return sin(s * 42.6520981210954362);
}

float hash2(float s){
    return fract(s * 24.57608761367653 + 0.4146145118678);
}

float wave1(float stx,float scale,float t){    // float angle = abs(sin(iTime))* 1.5;
    
    
    //float angle = .3 + 0.4 * abs(sin(iTime)) - 0.12;// 0.3~0.5
    //float angle = .3 + 0.4 * a - 0.12;// 0.3~0.5
    
    // stx += angle;
    float hash1 = hash1(t);
    // float hash2 = hash2(iTime);
    float r;
    float r1 = (0.1 + 0.0001 * hash1) * abs(sin((stx + 0.1*t) * 4.1));
    float r2 = (0.12 - 0.001 * hash1) * sin(stx*14. + t);
    float r3 = (.35 + 0.0001 * hash1) * cos(stx*18. + t)-0.6;
    
    
    r = smax(r,r1,0.6);
    r = smax(r,r2,.2);
    r = smax(r,r3,.6);
    r *= scale;
    r = max(0.0,r);
    return r;
}

float wave2(float stx,float scale){
    float res;
    
    stx = 4.*(stx-0.5);// input stx is 0.25~0.75 after uv.y += 0.5
    // now convert stx to -1 ~ 1 for local point up curve
    res = 1. - abs(stx) - 0.1; // move sharp peak lower bit
    float res1 = 1. - pow(stx,2.0);
    res = smax(res,res1,0.2); // linear make very sharp peak,mix with powered smooth peak
    res *= scale; // scale down
    res = max(0.0,res);
    
    return res;
}

float wave(float stx,float t,float scale){
    // st.x += 0.25 * sin(iTime);
    // a = abs(sin(iTime));
    float w1 = wave1(stx,0.7,t);
    float w2 = wave2(stx,0.15);
    
    // for debug
    // if(st.y < w1){
    //     col += vec3(1.0,0.0,0.0);
    // }
    
    // if(st.y < w2){
    //     col += vec3(0.0,1.0,0.0);
    // }
    
    float w = smax(w1,w2,0.02);
    w *= scale;
    return w;
}

                                                        
                                                        

void main()
{
    
    
        // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    
    vec3 col;
    
    vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
    uv.y += 0.5;
    //vec2 uv = fragCoord.xy/iResolution.xy;
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    // origin on -y axis
    
    



//    col = vec3(1.0);
    float lineNumF = 50.;
    for(float i = 0.;i<lineNumF;i++){
        float scale = 0.5;
        float w = wave1(st.x,scale*i*0.03,iTime*2.0);
        
       // if(st.y < w){
        //    col += 1.0;
       // }
        
        float line = 1.0 - smoothstep(0.0,lineWidith,abs(st.y - w));
        
        col += line;
    }

    
    
    // Output to screen
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);



// gpu press test

//for(float i = 0.;i<400.;i++){
//    //        col += 0.1 * sin(iTime*i + 3.1415926535 * i);
//    //        uv.x += sin(iTime) * 0.01;
//    col += iTime * pow(uv.x,3.0) + 4.1 * pow(uv.x,2.0) + 5.0 * pow(uv.x,2.0);
//    
//}
//
//col *= 0.1;
//

// ======================  backup










//
//vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
//uv *= 2.0; // -1. ~ 1.
//uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
//// uv *= 0.5;// 0 ~ 1
//vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//st.x += PI;// 0 ~ 2PI on -y axis
//int tryLoop = 6;
//for(int i = 0;i < tryLoop;i++ ){
//    float factor = 0.04 + 0.01 * sin(iTime*4.0);
//    st.y += factor * sin(0.0007*iTime*(st.x+iTime*0.1));
//
//    float index = floor(st.y  * lineNumF);
//
//    float gap = 1.0 / lineNumF;
//    float r = index * gap;
//    float line = 1.0 - smoothstep(0.0,lineWidith,abs(st.y - r));
//
//    col += line;
//}
//
//
//if(st.y < .15){
//    col += 1.0;
//}
//
//
//
//
//vec3 col;
//
//vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
//uv *= 2.0; // -1. ~ 1.
//uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
//// uv *= 0.5;// 0 ~ 1
//vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//st.x += PI;// 0 ~ 2PI on -y axis
//
//vec2 st1 = st;
//
//float index = floor(st.y  * lineNumF);
//st1.y += .02 * sin(0.01*iTime*st.x*16.);
//float gap = 1.0 / lineNumF;
//float r = index * gap;
//float line = 1.0 - smoothstep(0.0,lineWidith,abs(st1.y - r));
//
//col += line;








////    vec2 uv = fragCoord.xy / iResolution.xy;
//
//vec3 col;
//
//vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
//uv *= 2.0; // -1. ~ 1.
//uv.y += 1.0;
//
//vec2 st = vec2(atan(uv.x,uv.y),length(uv));
//st.y += 0.15;
////    st.x *= 1.5;
////st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
//
//
//
//
//
//for (int i = 0;i<4;i++){
//    float x = st.x;
//    x *= .2;
//    x -= fract(iTime*0.1);
//    // float x = uv.x;
//    float y = 0.0;
//    float a1 = -.2*sin(iTime*5.0);
//    float f1 = 12.5;
//    float y1 = wave2(x,a1,f1);
//    float a2 = 0.0;//
//    a2 = sin(iTime*10.)*0.1;
//    float f2 = 8.0;
//    float y2 = wave2(x+0.1,a2,f2);
//    y = smax(y,y1,0.9);
//    y = smax(y,y2,0.8);
//    // y = smax(y,wave1(x*0.01),-0.9);
//    float peak3 = 0.2;
//    float narrow3 = 1.0;//*sin(iTime*10.);
//    float y3 = wave3(x+0.2,peak3,narrow3);
//
//    y = smax(y,y3,0.8);
//    y = smax(y,0.2,0.9);
//    //        y *= 1.0;
//    ////        for(float j = 0.0;j<100.0;j++){
//    //            float index = floor(st.y  * lineNumF);
//    //            // index = 0.0;
//    //            float gap = 1.0 / lineNumF;
//    //            // for(float i = 0.0; i < lineNumF; i++){
//    //            float r = index * gap;
//    //            r += y * 0.02;
//    //            float line = 1.0 - smoothstep(0.0,lineWidith,abs(st.y - r));
//    //            col += vec3(line);
//    //
//    ////        }
//    //
//
//
//    ////    float index = floor(st1.y * lineNumF);
//    float gap = 4.2/lineNumF;
//
//    for(float i = 0.0; i < lineNumF; i++){
//        float r = y + i * gap;
//        //float r = y;
//        float line = 1.0 - smoothstep(0.0,lineWidith,abs(st.y - r));
//        col += vec3(line);
//    }
//
//
//}
//
