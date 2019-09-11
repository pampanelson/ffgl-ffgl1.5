//
//  Frag.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/8/24.
//

#ifndef Frag_h
#define Frag_h

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform float ticks;
uniform float width;
uniform float height;
float iTime;
vec2 iResolution;
vec2 fragCoord;
// uniform float switchTex;


uniform float bLineRipple;
uniform float bLineTracking;


uniform float lineNum;
uniform float lineWidth;
uniform float lineOffset;
uniform float lineMiRippleSize;
uniform float lineMiRippleSpeed;
uniform float lineMaRippleSize;
uniform float lineMaRippleSpeed;

uniform float bwLine;
uniform float globalWaveAmp;// smaller means bigger wave peak to the lower wave bottom;
                                                        // 0.06 ~ 0.6
uniform float trackingData[8]; // 12 size()


int kTrackingDataSize = 8;
float kTrackingDataSizeF = 8.0;
float PI = 3.1415926535;
float aPI = acos(-1.);

float rand(float n){return fract(sin(n) * 43758.5453123);}


// Polynomial smooth min (for copying and pasting into your shaders)
float smin(float a, float b, float k) {
    float h = clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
    return mix(a, b, h) - k*h*(1.0-h);
}

float smax(float a,float b,float k){
    return smin(a,b,-k);
}

float wave1(float x){
    return x;
}
float wave2(float x,float amplitude,float freq){
    float res = 0.0;
    
    res = amplitude - pow(x*freq,2.0);
    return res;
}
// peak is highest point , narrow is wave wide
float wave3(float x,float peak,float narrow){
    float res = 0.0;
    res = peak - narrow*abs(x*x*x);
    return res;
}



float wave_distort11(bool use,vec2 st,float angle,float factor){
    // distort =========================================
    
    
    vec2 st2 = st;
    //st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    
    float x = st2.x;
    x *= factor; // wave smooth factor
    // x -= fract(iTime*0.1);
    //x += 0.5;
    //x = -x;
    
    // =+++++++++++++ IMPORTANT ++++++++++++++++++++++++++++
    if(!use){
        
        x += 1.; // this value makes no distort ================ . TODO
    }
    else{
        x += -.0; // 0.0 is up direct , -0.5 is right direct , 0.5 is left direct
        x += 0.5 - angle;// angle is 0. ~ 1, 1 is right direct , 0 is left direct
        
    }
    
    // float x = uv.x;
    float y = 0.0;
    float a1 = -.2*sin(iTime*5.0);
    
    float f1 = 3.5 + sin(iTime) ;
    float y1 = wave2(x,a1,f1);
    float a2 = 0.0;//
    a2 = sin(iTime*10.)*0.1;
    float f2 = 9.0; // power ===============================
    float y2 = wave2(x+0.1,a2,f2);
    y = smax(y,y1,0.1);
    y = smax(y,y2,0.2);
    //y = smax(y,wave1(x*0.01),-0.9);
    float peak3 = 0.1;//
    float narrow3 = 5.0 + 2.*sin(iTime*2.);
    float y3 = wave3(x+0.1*sin(iTime*5.),peak3,narrow3);
    
    y = smax(y,y3,0.8);
    //y = smax(y,0.2,0.9);
    
    //y *= 1.2;// whole scale =======================
    
    return y;
}



float wave_distort1(float use,vec2 st,float angle,float amp){



    float y = 0.0;
    if(use > 0.0){
        bool bUseWaveDistort = true;
        y = wave_distort11(bUseWaveDistort,st,angle,0.2+ rand(iTime*0.000001));
        float y1 = wave_distort11(bUseWaveDistort,st,angle,0.3 + rand(iTime*0.000001));
        y = mix(y,y1,0.9 + rand(iTime*0.000001) );
        float y2 = wave_distort11(bUseWaveDistort,st,angle,0.9 + rand(iTime*0.000001));
        y = mix(y,y2,0.9+ rand(iTime*0.000001));
        
        float y3 = wave_distort11(bUseWaveDistort,st,angle,1.9);
        
        y = mix(y,y3,0.1+ rand(iTime*0.000001));
        
        float waveBottom = amp;// smaller means bigger wave peak to the lower wave bottom;
        // 0.05 ~ 0.6
        y = smax(y,waveBottom,0.9);
        
    }

    
    
    return y;
}


float line_wave(vec2 st,float lineNum,float lineWidth,float lineOffset,float distort,float bLineRipple){
    float line;
    float sty = st.y;
    if(bLineRipple > 0.5){
        float maRipple = sin(iTime) * lineMaRippleSpeed;
        //line = 1. - smoothstep(0.0,lineWidth,abs(sty + ripple - 0.1 ));
        
        sty += maRipple * lineMaRippleSize;
    }
    
    // not tracking ,input value is 0.0, not good
    if(distort > 0.0){
        sty /= distort;

    }
    sty *= lineNum;
    
    
    float index = floor(sty);
    if(index > lineOffset){
        
        if(bLineRipple > 0.5){
            float miRipple = sin(index*iTime) * lineMiRippleSpeed;
            
            sty += miRipple * lineMiRippleSize;
        }
        
        sty = fract(sty);
        
        line = 1. - smoothstep(0.0,lineWidth,abs(sty - 0.1));
        
        
    }
    
    return line;
}

void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    // --------------------------
    
    
    vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
    
    
    // prepare uv for st **********************************
    uv *= 2.0; // -1. ~ 1.
    uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
    
    // uv.y -= wordOffset;// whole screen offset ======================
    // uv *= 0.5;// 0 ~ 1
    
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    //st.x += PI;// 0 ~ 2PI on -y axis
    
    
    float y = 0.0;
    
    // y += wave_distort(bWordTracking,st,0.5,0.8);
    
    if(bLineTracking>0.0){
        for (int i = 0; i < 2; i++)
        {
            float angle = sin(iTime*float(i));
            float amp = globalWaveAmp;// smaller means bigger wave peak to the lower wave bottom;
            // 0.05 ~ 0.6
            
            y = smax(y,wave_distort1(bLineTracking,st,angle,amp),0.1);
            
        }
    }else{
        y = globalWaveAmp;
    }

    
    
    
    vec3 col;
    
    
    col = vec3(line_wave(st,lineNum,lineWidth,lineOffset,y,bLineRipple));
    
    
    vec3 col1 = 1. - col;
    col = mix(col1,col,bwLine);
    // debug distort wave =============
    bool bWaveDistortDebug = true;
    //     if(st.y < y && bWaveDistortDebug){
    //         col = vec3(1.0);
    //     }
    
    // Output to screen
    
//    col = vec3(sin(iTime));
    fragColor = vec4(col,1.0);
    
    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
