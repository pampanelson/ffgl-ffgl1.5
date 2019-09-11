#pragma once

#include "FFGLShader.h"
#include "FFGLPluginSDK.h"
#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <sstream>

// split string combine with "," or other mark seperator as vector
std::vector<std::string> MySplitS1(std::string input){
    std::vector<std::string> result;
    if(input.size()>0){
        
        std::istringstream iss(input);
        for(std::string s; iss >> s; )
            result.push_back(s);
    }
    
    return result;
}



std::vector<float> MyConvertStingToFloatVector(std::string input){
    
    std::vector<float> vect;
    
    std::stringstream ss(input);
    
    float i;
    
    while (ss >> i)
    {
        vect.push_back(i);
        
        if (ss.peek() == ',')
            ss.ignore();
    }
    
    
    
    return vect;
}





class AddSubtract : public CFreeFrameGLPlugin
{
public:
    AddSubtract();
    ~AddSubtract();
    
    ///////////////////////////////////////////////////
    // FreeFrame plugin methods
    ///////////////////////////////////////////////////
    FFResult SetTextParameter(unsigned int dwIndex, const char *value) override;
    char* GetTextParameter(unsigned int index) override;
    
    FFResult SetFloatParameter(unsigned int dwIndex, float value) override;
    float GetFloatParameter(unsigned int index) override;
    FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
    FFResult InitGL(const FFGLViewportStruct *vp) override;
    FFResult DeInitGL() override;
    
    ///////////////////////////////////////////////////
    // Factory method
    ///////////////////////////////////////////////////
    
    static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
    {
        *ppOutInstance = new AddSubtract();
        if (*ppOutInstance != NULL)
            return FF_SUCCESS;
        return FF_FAIL;
    }
    
    
protected:
    // Parameters
    
    int m_initResources;
    //    bool  m_SwitchTex;
    //    float m_Float1;
    
    
    
    bool bLineRipple;
    bool bLineTracking;
    
    float lineNum;
    float lineWidth;
    float lineOffset;
    float lineMiRippleSize;
    float lineMiRippleSpeed;
    float lineMaRippleSize;
    float lineMaRippleSpeed;
    
    
    float waveScale;
    
    
    float bwLine;
    // control wave shape and change rate
    float waveDelta;
    float waveMax;
    
    std::string rawOscTextData = "hello";
    
    
    int kTrackingDataSize = 16;
    GLfloat trackingData[16]; // for 8 group data wave position and amplitute for each
    std::vector<float> trackingDataVec[16];
    
    
    
    GLint bLineRippleLoc;
    GLint bLineTrackingLoc;
    
    GLint lineNumLoc;
    GLint lineWidthLoc;
    GLint lineOffsetLoc;
    GLint lineMiRippleSizeLoc;
    GLint lineMiRippleSpeedLoc;
    GLint lineMaRippleSizeLoc;
    GLint lineMaRippleSpeedLoc;
    
    
    GLint trackingDataLoc;
    
    
    GLint waveScaleLoc;
    
    GLint bwLineLoc;
    
    FFGLShader m_shader;
    GLint m_inputTextureLocation;
    GLint m_TicksLocation;
    
    GLint m_WidthLocation;
    GLint m_HeightLocation;
    
    
    GLint m_SwitchTexLocation;
    GLint m_Float1Location;
    
    double ticks;
    
};
