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


float hash(vec2 p)  // replace this by something better
{
    p  = 50.0*fract( p*0.34768375633183 + vec2(0.71,0.113));
    return -1.0+2.0*fract( p.x*p.y*(p.x+p.y) );
}
                                                        
float noise( in vec2 p )
{
    vec2 i = floor( p );
    vec2 f = fract( p );
    
    vec2 u = f*f*(3.0-2.0*f);
    
    return mix( mix( hash( i + vec2(0.0,0.0) ),
                    hash( i + vec2(1.0,0.0) ), u.x),
               mix( hash( i + vec2(0.0,1.0) ),
                   hash( i + vec2(1.0,1.0) ), u.x), u.y);
}


void main()
{
    
    
        // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    
    
    vec2 p = fragCoord.xy / iResolution.xy;
    
    vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0);
    uv.y -= .9;
    
//        vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
//        uv.y += 0.5;
        //vec2 uv = fragCoord.xy/iResolution.xy;
        vec2 st = vec2(atan(uv.x,uv.y),length(uv));
        st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
        // origin on -y axis
    
    

    
    
    vec3 col;

    
    float number = 180.0;// pressure test
    
    float noise = noise(vec2(uv.y*1.,uv.x*1.1+1./number*sin(iTime*20.)));
    noise = pow(noise,2.);
    float r = noise * 1.;
    
    for(float i=0.0;i<number;i++){

        col += 1. - smoothstep(0.0,0.005,abs(uv.y +  i/30. - r*noise));
        
        
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
