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

float len(vec3 p) {
    return max(abs(p.x)*0.5+abs(p.z)*0.5,max(abs(p.y)*0.5+abs(p.x)*0.5,abs(p.z)*0.5+abs(p.y)*0.5));
}
                                                        
void main()
{
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    
    
    // first row is frequency data (48Khz/4 in 512 texels, meaning 23 Hz per texel)
    float fft = float1;
    
    vec2 R = iResolution.xy;
    vec2 uv = (fragCoord - .5*R) / iResolution.y;
    
    
    // first row is frequency data (48Khz/4 in 512 texels, meaning 23 Hz per texel)
    
    
    
    vec3 rp = vec3(0.,0.,iTime);
    vec3 rd = normalize(vec3(uv,1.));
    
    vec3 c = vec3(0.);
    float s = 0.;
    
    float viewVary = cos(iTime*0.05)*.15;
    
    for (int i = 0; i < 74; i++) {
        vec3 hp = rp+rd*s;
        float d = len(cos(hp*.6+
                          cos(hp*.3+iTime*.5)))-.75;
        float cc = min(1.,pow(max(0., 1.-abs(d)*10.25),1.))/(float(i)*1.+10.);//clamp(1.-(d*.5+(d*5.)/s),-1.,1.);
        
        c += (cos(vec3(hp.xy,s))*.5+.5 + cos(vec3(s+iTime*fft,hp.yx)*.1)*.5+.5 + 1.)/3.
        *cc;
        
        s += max(abs(d),0.35+viewVary);
        rd = normalize(rd+vec3(sin(s*0.5*fft),cos(s*0.5),0.)*d*0.05*clamp(s-1.,0.,1.));
    }
    
    fragColor = vec4(pow(c,vec3(1.2)),1.);
    
    gl_FragColor = fragColor;
}
);

