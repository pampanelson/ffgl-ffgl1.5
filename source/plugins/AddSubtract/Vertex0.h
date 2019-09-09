//
//  Vert.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/8/24.
//

#ifndef Vert_h
#define Vert_h

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string vertexShaderCode = STRINGIFY(
                                                      void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    
}
);


#endif /* Vert_h */
