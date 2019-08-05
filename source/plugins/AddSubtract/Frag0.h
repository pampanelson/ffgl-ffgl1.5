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

const float TWO_PI = 6.28318530718;
const int steps = 36;
const float brighten = 1.;

float map(float value, float low1, float high1, float low2, float high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

float triangle(vec2 p, float size) {
    vec2 q = abs(p);
    return max(q.x * 0.866025 + p.y * 0.5, -p.y * 0.5) - size * 0.5;
}

float hexagon(vec2 p, float radius) {
    vec2 q = abs(p);
    return max(abs(q.y), q.x * 0.866025 + q.y * 0.5) - radius;
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

    
    float time = iTime * 2.;
    vec2 st = (2. * fragCoord.xy - iResolution.xy) / iResolution.y;
    st -= vec2(0, 0.3); // offset overall y a bit for more "floor"
    
    // start white and head towards black as triangles shrink
    float col = 1.;
    float sizeStart = 5. + cos(time);
    float sizeEnd = 0.;
    float stepSize = sizeStart / float(steps);
    for(int i = 0; i < steps; i++) {
        float curStepSize = float(i) * stepSize;
        float stepColor = map(curStepSize, sizeStart, sizeEnd, 1., 0.05);
        float yCompensate = float(i) * -0.22; // triangle isn't centered, so we can offset for better concentricity
        float offset = fft * sin(iTime);
        vec2 stMoved = offset + st + 0.2 * vec2(0, yCompensate + sin(float(i) * 0.25 + time * 3.)); // offset wobble y down the tunnel, 3x faster than main oscillation
        if(hexagon(stMoved, curStepSize) > 0.) {
            col = stepColor;
        }
    }
    fragColor = vec4(vec3(col * brighten), 1.0);
    
    
    
//    fragColor = vec4(1.0);
    gl_FragColor = fragColor;
}
);




