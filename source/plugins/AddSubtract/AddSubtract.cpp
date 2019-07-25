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
	"PDemo190725",								// Plugin unique ID
	"PP3D",					// Plugin name
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

const int samples = 16;
const int num_steps = 6;
const float PI = 3.141592;
const float radius = 10.;
vec2 iResolution;
float r2D(vec2 p)
{
    return fract(sin(dot(p, vec2(32.91, 54.28)))*96516.4172);
}
                                                        
float pattern(vec2 p, float s)
{
    vec2 i = floor(p*s);
    return r2D(i);
}
                                                        
vec3 normals(vec2 p, float s, float h)
{
    float pixel = 1./iResolution.y;
    vec2 e = vec2(pixel, 0.);
    return normalize(
                     vec3(
                          (pattern(p-e.xy, s)-h)/e.x,
                          (pattern(p-e.yx, s)-h)/e.x,
                          1.));
}
                                                        



float ambient_occlusion(vec2 p, float h, vec3 n, float s)
{
    float ao = 0.;
    float pixel = 1./iResolution.y;
    vec3 origin = vec3(p, h);
    for (int i = 0; i < samples; i++)
    {
        float angle = float(i)*PI/float(samples);
        vec2 dir = vec2(cos(angle), sin(angle));
        for (int j = 1; j <= num_steps; j++)
        {
            vec2 point = p+float(j)*pixel*dir*radius;
            float r = pattern(point, s);
            vec3 current = vec3(point, r);
            vec3 dir_curr = current - origin;
            float dir = dot(normalize(dir_curr), n);
            if (dir < 0.)
                break;
            if (dir > 0.)
            {
                ao += length(dir_curr)/float(j);
                break;
            }
        }
    }
    ao /= float(samples);
    return ao;
}
                                                        

void main() {
    
    // name convert ---------------
    vec4 fragColor;
    float iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    vec2 fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;

    // ---------------

    fragColor = vec4(1.0,0.0,0.0,1.0);
    
    vec2 uv = fragCoord/iResolution.y;
    
    float s = 6.;
    float h = pattern(uv, s);
    vec3 n = normals(uv, s, h);
    float ao = 1.-ambient_occlusion(uv, h, n, s);
    
    float t = iTime;
    vec3 ld = normalize(vec3(cos(t), sin(t), 1.)*3.-vec3(uv, h));
    float diff = max(dot(n, ld), 0.);
    float l = diff*ao;
    
    vec3 col = vec3(0.);
    col += l*mix(vec3(1.), vec3(0.0, .6, .8), h);
    
    fragColor = vec4(col,1.0);
    
    // finish ---------------
    gl_FragColor = fragColor;
    
}
//void main()
//{
//    //vec4 color = texture2D(inputTexture,gl_TexCoord[0].st);
//    vec4 color = vec4(0.0,0.0,sin(ticks),1.0);
//    gl_FragColor  =  color;
//}
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
