#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "../../lib/ffgl/utilities/utilities.h"

#define FFPARAM_BrightnessR  (0)
#define FFPARAM_BrightnessG	 (1)
#define FFPARAM_BrightnessB	 (2)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"PFragTest0",								// Plugin unique ID
	"PFragTest0",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"demo",			// Plugin description
	"by Pampa -- lohosoft.com"				// About
);

static const std::string vertexShaderCode = STRINGIFY(
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
);


static const std::string fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform vec3 brightness;
uniform float ticks;
uniform float width;
uniform float height;



void main() {
    
    // name convert ---------------
    vec4 fragColor;
    float iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;

    vec4 tex= texture2D(inputTexture,gl_TexCoord[0].st);

    
    // ---------------
    fragColor = vec4(1.0,0.0,0.0,1.0)*tex;
    
    // finish ---------------
    gl_FragColor = fragColor;
    
}
);

AddSubtract::AddSubtract()
:CFreeFrameGLPlugin(),
 m_initResources(1),
 m_inputTextureLocation(-1),
 m_BrightnessLocation(-1)
{
	// Input properties
	SetMinInputs(1);
	SetMaxInputs(1);

	// Parameters
	SetParamInfo(FFPARAM_BrightnessR, "R", FF_TYPE_RED, 0.5f);
	m_BrightnessR = 0.5f;

	SetParamInfo(FFPARAM_BrightnessG, "G", FF_TYPE_GREEN, 0.5f);
	m_BrightnessG = 0.5f;
	
	SetParamInfo(FFPARAM_BrightnessB, "B", FF_TYPE_BLUE, 0.5f);
	m_BrightnessB = 0.5f;

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
	m_BrightnessLocation = m_shader.FindUniform("brightness");
    m_TicksLocation = m_shader.FindUniform("ticks");
    m_WidthLocation = m_shader.FindUniform("width");
    m_HeightLocation = m_shader.FindUniform("height");
    
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
    
    
	//assign the Brightness
	glUniform3f(m_BrightnessLocation,
				-1.0f + (m_BrightnessR * 2.0f),
				-1.0f + (m_BrightnessG * 2.0f),
				-1.0f + (m_BrightnessB * 2.0f)
				);
	

    // assign ticks in millisecond
    glUniform1f(m_TicksLocation,ticks);
    
    // assign width and height
    glUniform1f(m_WidthLocation, (float)viewport[2]);
    glUniform1f(m_HeightLocation, (float)viewport[3]);

    
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
	case FFPARAM_BrightnessR:
		retValue = m_BrightnessR;
		return retValue;
	case FFPARAM_BrightnessG:
		retValue = m_BrightnessG;
		return retValue;
	case FFPARAM_BrightnessB:
		retValue = m_BrightnessB;
		return retValue;
	default:
		return retValue;
	}
}

FFResult AddSubtract::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
	case FFPARAM_BrightnessR:
		m_BrightnessR = value;
		break;
	case FFPARAM_BrightnessG:
		m_BrightnessG = value;
		break;
	case FFPARAM_BrightnessB:
		m_BrightnessB = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
