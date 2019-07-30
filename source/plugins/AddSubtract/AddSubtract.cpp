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
	"PDemo190730",								// Plugin unique ID
	"PampaDemo0",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"demo",			// Plugin description
	"by Pampa -- lohosoft.com"				// About
);

static const std::string vertexShaderCode = STRINGIFY(

//uniform sampler2D inputTexture;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aCoord;
//varying vec4 vertColor;
//varying vec2 vertCoord;
//uniform float width;
//uniform float height;
void main()
{

//    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//    gl_Position = vec4(aCoord.x, aCoord.y, aPos.z, 1.0);
//    gl_TexCoord[0] = gl_MultiTexCoord0;
//    vertCoord = aCoord;
//    vertColor = texture2D(inputTexture,aCoord.xy);
//    vertCoord = aCoord;

}
);


static const std::string fragmentShaderCode = STRINGIFY(

uniform sampler2D inputTexture;
uniform vec3 brightness;
uniform float ticks;
uniform float width;
uniform float height;
//varying vec2 vertCoord;
//varying vec4 vertColor;

void main() {
    
    // name convert ---------------
    vec4 fragColor;
    float iTime = ticks/1000.0;
    vec2 iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;

    
    // ---------------
//    fragColor = vec4(sin(iTime)*vec2(fragCoord.xy/iResolution.xy),0.0,1.0);
//    fragColor = texture2D(inputTexture,gl_TexCoord[0].st);
//    fragColor = texture2D(inputTexture,gl_FragCoord.xy/iResolution.xy);
//    fragColor = gl_FrontColor;
//    fragColor = texture2D(inputTexture,vertCoord);
//    fragColor = texture2D(inputTexture,vertCood.xy);
    fragColor = vec4(1.0,0.0,0.0,1.0);
    
//    fragColor = vertColor;

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

    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture.Handle);
    
    

    //draw the quad that will be painted by the shader/textures
    //note that we are sending texture coordinates to texture unit 1..
    //the vertex shader and fragment shader refer to this when querying for
    //texture coordinates of the inputTexture
//    glBegin(GL_QUADS);
////
////    //lower left
//    glMultiTexCoord2f(GL_TEXTURE0, 0,0);
//    glVertex2f(-1,-1);
//
//    //upper left
//    glMultiTexCoord2f(GL_TEXTURE0, 0, maxCoords.t);
//    glVertex2f(-1,1);
//
//    //upper right
//    glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, maxCoords.t);
//    glVertex2f(1,1);
//
//    //lower right
//    glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, 0);
//    glVertex2f(1,-1);
//    glEnd();
////
//    //unbind the input texture
//    glBindTexture(GL_TEXTURE_2D,0);

    // TODO .................. enable and disable once is ok ????

    // ----------------  full opengl function try -------------------------------
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, 0.0f, 0.0f,0.0f,.5f,
        1.0f, -.5f, 0.0f,-1.0f,-0.5f,
        0.0f,  1.0f, 1.0f,0.5f,1.0f
    };
//
//    static const GLfloat g_vertex_buffer_data[] = {
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        0.0f,  1.0f, 1.0f
//    };
//
    
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);

    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    
    
    glBindVertexArrayAPPLE(vao);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // need detail memo --------------------  TODO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
//
//
    

    // finaly draw --------------
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    
    
    // clean up
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArrayAPPLE(0);
    
    
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
