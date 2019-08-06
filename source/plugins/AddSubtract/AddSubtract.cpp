#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vertex0.h"
#include "Frag0.h"
#include "../../lib/ffgl/utilities/utilities.h"



#define FFPARAM_SwitchTex   (0)
#define FFPARAM_Float1      (1)
////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"P10Demo",								// Plugin unique ID
	"P10Demo",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"P10Demo",			// Plugin description
	"by Pampa -- lohosoft.com"				// About
);




AddSubtract::AddSubtract()
:CFreeFrameGLPlugin(),
 m_initResources(1),
 m_inputTextureLocation(-1){
	// Input properties
	SetMinInputs(1);
	SetMaxInputs(1);

	// Parameters
	SetParamInfo(FFPARAM_SwitchTex, "Switch Tex", FF_TYPE_BOOLEAN, false);
	
    m_SwitchTex = false;
	
    SetParamInfo(FFPARAM_Float1,"Float 1",FF_TYPE_STANDARD,0.0f);
    m_Float1 = 0.0f;

}

AddSubtract::~AddSubtract()
{
	
}

FFResult AddSubtract::InitGL(const FFGLViewportStruct *vp)
{

	m_initResources = 0;


	//initialize gl shader
	m_shader.Compile(vertexShaderCode,fragmentShaderCode);

	//activate our shader
	m_shader.BindShader();

	//to assign values to parameters in the shader, we have to lookup
	//the "location" of each value.. then call one of the glUniform* methods
	//to assign a value
	m_inputTextureLocation = m_shader.FindUniform("inputTexture");
    m_TicksLocation = m_shader.FindUniform("ticks");
    m_WidthLocation = m_shader.FindUniform("width");
    m_HeightLocation = m_shader.FindUniform("height");
    m_SwitchTexLocation = m_shader.FindUniform("switchTex");
    m_Float1Location = m_shader.FindUniform("float1");
    
	//the 0 means that the 'inputTexture' in
	//the shader will use the texture bound to GL texture unit 0
	glUniform1i(m_inputTextureLocation, 0);
	
	m_shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult AddSubtract::DeInitGL()
{
  m_shader.FreeGLResources();


  return FF_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Methods
////////////////////////////////////////////////////////////////////////////////////////////////////



FFResult AddSubtract::ProcessOpenGL(ProcessOpenGLStruct *pGL)
{
	if (pGL->numInputTextures<1)
		return FF_FAIL;

	if (pGL->inputTextures[0]==NULL)
		return FF_FAIL;

    
    ticks = getTicks();
	//activate our shader
	m_shader.BindShader();

	FFGLTextureStruct &Texture = *(pGL->inputTextures[0]);

	//get the max s,t that correspond to the 
	//width,height of the used portion of the allocated texture space
	FFGLTexCoords maxCoords = GetMaxGLTexCoords(Texture);

    
    //get the width of the viewport
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    


    // assign ticks in millisecond
    glUniform1f(m_TicksLocation,ticks);
    
    // assign width and height
    glUniform1f(m_WidthLocation, (float)viewport[2]);
    glUniform1f(m_HeightLocation, (float)viewport[3]);

    glUniform1f(m_Float1Location, m_Float1);
    
    if(m_SwitchTex){
        glUniform1f(m_SwitchTexLocation,1.0);

    }else{
        glUniform1f(m_SwitchTexLocation,0.0);

    }
    
    
    
	//activate texture unit 1 and bind the input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture.Handle);
    
	//draw the quad that will be painted by the shader/textures
	//note that we are sending texture coordinates to texture unit 1..
	//the vertex shader and fragment shader refer to this when querying for
	//texture coordinates of the inputTexture
	glBegin(GL_QUADS);

	//lower left
	glMultiTexCoord2f(GL_TEXTURE0, 0,0);
	glVertex2f(-1,-1);

	//upper left
	glMultiTexCoord2f(GL_TEXTURE0, 0, maxCoords.t);
	glVertex2f(-1,1);

	//upper right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, maxCoords.t);
	glVertex2f(1,1);

	//lower right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, 0);
	glVertex2f(1,-1);
	glEnd();

	//unbind the input texture
	glBindTexture(GL_TEXTURE_2D,0);


	//unbind the shader
	m_shader.UnbindShader();

	return FF_SUCCESS;
}

float AddSubtract::GetFloatParameter(unsigned int dwIndex)
{
	float retValue = 0.0;

	switch (dwIndex)
	{
    case FFPARAM_SwitchTex:
        retValue = m_SwitchTex;
        return retValue;
    case FFPARAM_Float1:
        retValue = m_Float1;
        return retValue;
	default:
		return retValue;
	}
}

FFResult AddSubtract::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
    case FFPARAM_Float1:
        m_Float1 = value;
        break;
    case FFPARAM_SwitchTex:
        m_SwitchTex = value > 0.5;
        break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
