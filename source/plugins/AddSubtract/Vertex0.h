//
//  Header.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/7/30.
//

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string vertexShaderCode = STRINGIFY(
varying float gray;
varying float a[4];

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    gray = 1.0;
    a[0] = 1.0;
    a[1] = 0.0;
    a[2] = 0.0;
    a[3] = 1.0;
}
                                                      
);
