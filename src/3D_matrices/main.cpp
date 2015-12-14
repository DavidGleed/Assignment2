// tag::C++11check[]
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if __cplusplus < 201103L
	#pragma message("WARNING: the compiler may not be C++11 compliant. __cplusplus version is : " STRING(__cplusplus))
#endif
// end::C++11check[]

// tag::includes[]
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <SDL.h>

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/ext.hpp"
// end::includes[]

// tag::using[]
// see https://isocpp.org/wiki/faq/Coding-standards#using-namespace-std
// don't use the whole namespace, either use the specific ones you want, or just type std::
using std::cout;
using std::cerr;
using std::endl;
using std::max;
using std::string;
// end::using[]


// tag::globalVariables[]
std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext
int frameCount = 0;
std::string frameLine = "";

GLint uniform_mvp;
// end::globalVariables[]

// tag::loadShader[]
std::string loadShader(const string filePath) {
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		string fileData( (std::istreambuf_iterator<char>(fileStream)),
		                 (std::istreambuf_iterator<char>()          ));

		cout << "Shader Loaded from " << filePath << endl;
		return fileData;
	}
	else
	{
        cerr << "Shader could not be loaded - cannot read file " << filePath << ". File does not exist." << endl;
        return "";
	}
}
// end::loadShader[]

//our variables
bool done = false;

GLfloat PaddleXZ = 0.1f;
GLfloat PaddleY = 1.0f;


GLfloat BallXYZ = 0.1f;
// tag::vertexData[]
//the data about our geometry
const GLfloat PaddleData[] = {

	//Cuboid
	//	 X      Y      Z	  R     G     B      A

	//Bottom
	-PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// top
	-PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// front
	-PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,
	PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,
	-PaddleXZ, PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,
	PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,
	PaddleXZ, PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,
	-PaddleXZ, PaddleY, PaddleXZ,   0.0f, 0.0f, 1.0f,  1.0f,

	// back
	-PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// left
	-PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ,-PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ,-PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY, PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-PaddleXZ, PaddleY,-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// right
	PaddleXZ,-PaddleY,	 PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,	-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,	-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ,-PaddleY,	 PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,	-PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,
	PaddleXZ, PaddleY,	 PaddleXZ,   0.0f, 1.0f, 0.0f,  1.0f,

};
// end::vertexData[]

const GLfloat BallData[] = {
	//	 X      Y      Z	  R     G     B      A

	// bottom
	-BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// top
	-BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// front
	-BallXYZ,-BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,
	BallXYZ,-BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,
	-BallXYZ, BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,
	BallXYZ,-BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,
	BallXYZ, BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,
	-BallXYZ, BallXYZ, BallXYZ,   1.0f, 1.0f, 1.0f,  1.0f,

	// back
	-BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// left
	-BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ,-BallXYZ,   .0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	-BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,

	// right
	BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ,-BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ,-BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
	BallXYZ, BallXYZ, BallXYZ,   0.0f, 1.0f, 0.0f,  1.0f,
};

// tag::gameState[]
//the translation vector we'll pass to our GLSL program

//glm::vec3 position1 = {-1.4f, 0.0f, 0.0f};
//glm::vec3 velocity1 = { 0.0f, 0.5f, 0.0f};
//
//glm::vec3 position2 = { 1.4f, 0.0f, 0.0f};
//glm::vec3 velocity2 = { 0.0f,-0.5f, 0.0f};
//
//glm::vec3 ballPosition = { 0.0f, 0.0f, 0.0f };
//glm::vec3 ballVelocity = { 0.0f, 0.0f, 0.0f };



//--------------------------------------------------------------------------------------


GLfloat Pos1[] = { -0.9f, 0.0f };
GLfloat Vel1[] = { 0.0f, 0.3f };

GLfloat Pos2[] = { 0.9f, 0.0f };
GLfloat Vel2[] = { 0.0f, -0.3f };

GLfloat ballPos[] = { 0.0f, 0.0f };
GLfloat ballVel[] = { -0.4f, 0.3f };
// end::gameState[]

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

//attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL
GLint translationVectorLocation;

//uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;

GLuint vertexDataBufferObject[2];
GLuint vertexArrayObject[2];
// end::GLVariables[]



// end Global Variables
/////////////////////////

// tag::initialise[]
void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}
// end::initialise[]



// tag::createWindow[]
void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL); //same height and width makes the window square ...


	//error handling
	if (win == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}
// end::createWindow[]

// tag::setGLAttributes[]
void setGLAttributes()
{
	int major = 3;
	int minor = 3;
	cout << "Built for OpenGL Version " << major << "." << minor << endl; //ahttps://en.wikipedia.org/wiki/OpenGL_Shading_Language#Versions
	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to versicreate remote branchon " << major << "." << minor << " OK!\n";
}
// tag::setGLAttributes[]

// tag::createContext[]
void createContext()
{
	setGLAttributes();

	context = SDL_GL_CreateContext(win);
	if (context == nullptr){
		SDL_DestroyWindow(win);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}
// end::createContext[]

// tag::initGlew[]
void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cerr << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
}
// end::initGlew[]

// tag::createShader[]
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}
// end::createProgram[]

// tag::initializeProgram[]
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader("vertexShader.glsl")));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader("fragmentShader.glsl")));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cerr << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
	}

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(theProgram, "position");
	vertexColorLocation = glGetAttribLocation(theProgram, "vertexColor");
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	translationVectorLocation = glGetUniformLocation(theProgram, "Pos1");
	translationVectorLocation = glGetUniformLocation(theProgram, "Pos2");
	translationVectorLocation = glGetUniformLocation(theProgram, "ballPos");

	//only generates runtime code in debug mode
	assert( modelMatrixLocation != -1);
	assert( viewMatrixLocation != -1);
	assert( projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}
// end::initializeProgram[]

// tag::initializeVertexArrayObject[]
//setup a GL object (a VertexArrayObject) that stores how to access data and from where
void initializeVertexArrayObject()
{
	glGenVertexArrays(2, &vertexArrayObject[0]); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << vertexArrayObject << std::endl;

	glBindVertexArray(vertexArrayObject[0]); //make the just created vertexArrayObject the active one

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject[0]); //bind vertexDataBufferObject

		glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
		glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

		// tag::glVertexAttribPointer[]
		glVertexAttribPointer(positionLocation,    3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *) (0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
		glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *) (3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
		// end::glVertexAttribPointer[]

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it


	//----------------------------------------------------------------------------


	glBindVertexArray(vertexArrayObject[1]); //make the just created vertexArrayObject the active one

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject[1]); //bind vertexDataBufferObject

	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

	// tag::glVertexAttribPointer[]
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
	glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
	// end::glVertexAttribPointer[]

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it




	//cleanup
	glDisableVertexAttribArray(positionLocation); //disable vertex attribute at index positionLocation
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer

}
// end::initializeVertexArrayObject[]

// tag::initializeVertexBuffer[]
void initializeVertexBuffer()
{
	glGenBuffers(2, vertexDataBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PaddleData), PaddleData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PaddleData), PaddleData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BallData), BallData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << vertexDataBufferObject << std::endl;

	initializeVertexArrayObject();
}
// end::initializeVertexBuffer[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	initializeVertexBuffer(); //load data into a vertex buffer

	cout << "Loaded Assets OK!\n";
}
// end::loadAssets[]

// tag::handleInput[]
void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//hit escape to exit

					//case SDLK_ESCAPE: done = true;
					//	// use "a" and "d" to invert Paddle velocity
					//case SDLK_a: velocity1 *= -1.0f;
					//	break;
					//case SDLK_d: velocity2 *= -1.0f;
					//	break;


				case SDLK_ESCAPE: done = true;
					// use "a" and "d" to invert Paddle velocity
				case SDLK_a: Vel1[1] *= -1.0f;
					break;
				case SDLK_d: Vel2[1] *= -1.0f;
					break;

				}
			break;
		}
	}
}
// end::handleInput[]

// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//WARNING - we should calculate an appropriate amount of time to simulate - not always use a constant amount of time
			// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html

	/*position1 += float(simLength) * velocity1;
	position2 += float(simLength) * velocity2;
	ballPosition += float(simLength) * ballVelocity;*/



	Pos1[0] += (float)simLength * Vel1[0];
	Pos1[1] += (float)simLength * Vel1[1];

	Pos2[0] += (float)simLength * Vel2[0];
	Pos2[1] += (float)simLength * Vel2[1];

	ballPos[0] += (float)simLength * ballVel[0];
	ballPos[1] += (float)simLength * ballVel[1];


	if (Pos1[1] >= 0.7f)
	{
		//reverse velocity on the collision axis, assuming X
		Vel1[1] *= -1;
	}

}
// end::updateSimulation[]

// tag::preRender[]
void preRender()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 600, 600); //set viewpoint
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	glBindVertexArray(vertexArrayObject[0]);

	
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*600 / 600, 0.1f, 10.0f);


	//set projectionMatrix - how we go from 3D to 2D
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(projection));
	
	//set viewMatrix - how we control the view (viewpoint, view direction, etc)
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view));

	
	modelMatrix = glm::translate(glm::mat4(1.0f), Pos1);	
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));

	glUniform2f(translationVectorLocation, Pos1[0], Pos1[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);


	modelMatrix = glm::translate(glm::mat4(1.0f), Pos2);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));

	glUniform2f(translationVectorLocation, Pos2[0], Pos2[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);



	glBindVertexArray(vertexArrayObject[1]);

	modelMatrix = glm::translate(glm::mat4(1.0f), ballPos);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));

	glUniform2f(translationVectorLocation, ballPos[0], ballPos[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);




	glBindVertexArray(0);

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
	frameLine += "Frame: " + std::to_string(frameCount++);
	cout << "\r" << frameLine << std::flush;
	frameLine = "";
}
// end::postRender[]

// tag::cleanUp[]
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}
// end::cleanUp[]

// tag::main[]
int main( int argc, char* args[] )
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();

	createContext();

	initGlew();

	glViewport(0,0,600,600); //should check what the actual window res is?

	SDL_GL_SwapWindow(win); //force a swap, to make the trace clearer


	//do stuff that only needs to happen once
	//- create shaders
	//- load vertex data
	loadAssets();

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		preRender();

		render(); // this should render the world state according to VARIABLES -

		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]
