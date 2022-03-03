#include <iostream>
#include <epoxy/gl.h>
#include <epoxy/egl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Percussa.h"

class TestPluginEditorInterface : public Percussa::SSP::PluginEditorInterface {
public: 	
	GLuint VAO = 0; 
	enum VboIndex { 
		kVertices = 0, 
		kColors, 
		numVBOs, 
	}; 
	GLuint VBOs[numVBOs] = {}; 
	unsigned int shaderProgram = 0; 

	// vertices for an equilateral triangle around the origin (0, 0)
	// see https://math.stackexchange.com/questions/2385147/how-do-i-represent-an-equilateral-triangle-in-cartesian-coordinates-centered-aro 
	// for the z coordinate, use 1.0f so we place the triangle closest 
	// to the viewer in front of any other geometry (see orthogonal 
	// projection matrix below).  

	float w = 1.25f;
	float h = std::sqrt(3)/2*w; 

	static const int numVertices = 3; 
	static const int numCompsPerVert = 4; 

	float vertices[numVertices][numCompsPerVert] = {
		{    0,  h/2,	1.0f, 	1.0f, }, 
		{ -w/2,	-h/2,	1.0f,	1.0f, },
		{  w/2,	-h/2,	1.0f, 	1.0f, },
	}; 

	// use colors red, green, blue with alpha = 1.0f for vertices 
	float colors[numVertices][numCompsPerVert] = { 
		{ 1.0f, 0.0f, 0.0f, 1.0f, }, 
	       	{ 0.0f, 1.0f, 0.0f, 1.0f, }, 
		{ 0.0f, 0.0f, 1.0f, 1.0f, }, 	
	}; 

	glm::mat4 proj = glm::mat4(1.0f);  
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	GLint mLoc = 0; 
	GLint vLoc = 0; 
	GLint pLoc = 0; 

public:
	TestPluginEditorInterface() {

		static const char* vertexShaderSource = R"(
			#version 300 es
			layout (location = 0) in vec4 aPos;
			layout (location = 1) in vec4 inColor; 
			out vec4 outColor;
			uniform mat4 model; 
			uniform mat4 view;
			uniform mat4 proj; 
			void main() {
				gl_Position = proj*view*model*aPos;
				outColor = inColor; 
			}
		)";

		static const char* fragmentShaderSource = R"(
			#version 300 es
			precision highp float; 
			out vec4 FragColor;
			in vec4 outColor; 
			void main() { 
				FragColor = outColor;
			}
		)";

		// based on code from 
		// https://learnopengl.com/Getting-started/Hello-Triangle
		// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp 

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) { 
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
				<< infoLog << std::endl;
			throw std::runtime_error("error compiling vertex shader"); 
		}

		// fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) { 
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
				<< infoLog << std::endl;
			throw std::runtime_error("error compiling fragment shader"); 
		}

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" 
				<< infoLog << std::endl;
			throw std::runtime_error("error linking shader program"); 
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		std::clog << __FUNCTION__ << ": shaders compiled and linked" << std::endl; 

		// set initial proj/view/model matrices in shader 
		glUseProgram(shaderProgram);
		mLoc = glGetUniformLocation(shaderProgram, "model");
		vLoc = glGetUniformLocation(shaderProgram, "view");
		pLoc = glGetUniformLocation(shaderProgram, "proj");
		glUniformMatrix4fv(mLoc, 1, false, glm::value_ptr(model)); 
		glUniformMatrix4fv(vLoc, 1, false, glm::value_ptr(view)); 
		glUniformMatrix4fv(pLoc, 1, false, glm::value_ptr(proj)); 
		std::clog << __FUNCTION__ << ": set proj/view/model matrices" << std::endl; 

		glGenVertexArrays(1, &VAO);
		glGenBuffers(numVBOs, VBOs);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), 
		// and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		int v = 0; 
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v); 
		std::clog << __FUNCTION__ << ": max vertex attribs: " << v << std::endl; 

		// allocate VBOs, enable vertex attrib pointers, etc  
		for (int i=0; i<numVBOs; i++) { 
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, numCompsPerVert, 
				GL_FLOAT, GL_FALSE, numCompsPerVert*sizeof(float), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0); 
		}
		std::clog << __FUNCTION__ << ": vertex buffers created" << std::endl; 

		// You can unbind the VAO afterwards so other VAO calls won't accidentally 
		// modify this VAO, but this rarely happens. Modifying other VAOs requires 
		// a call to glBindVertexArray anyways so we generally don't unbind VAOs 
		// (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0); 
	}

	~TestPluginEditorInterface() {
	
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(numVBOs, VBOs);
		glDeleteProgram(shaderProgram);
	}

	void frameStart() override {}
	void visibilityChanged(bool b) override {}
	void renderToImage(unsigned char* buffer, int width, int height) override {}

	// called after renderToImage, when all OpenGLES geometry is being drawn. 
	void draw(int width, int height) override {

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); 

		// update vertices  
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[kVertices]); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// update colors 
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[kColors]); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// rotate model matrix by 1 degrees around z axis and update in shader
		model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(mLoc, 1, false, glm::value_ptr(model)); 

		// set orthogonal projection matrix taking into account aspect ratio of screen
		float ar = (float)(width) / height;
		proj = glm::ortho(-ar, ar, -1.0f, 1.0f, -1.0f, 1.0f); 
		glUniformMatrix4fv(pLoc, 1, false, glm::value_ptr(proj)); 

		// draw triangle 
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glBindVertexArray(0); 
	}
};

class TestPluginInterface: public Percussa::SSP::PluginInterface {
public:
    TestPluginInterface(): editor_(nullptr) {
    }

    ~TestPluginInterface() {}

    Percussa::SSP::PluginEditorInterface* getEditor() override {
	if (editor_ == nullptr) {
            editor_ = new TestPluginEditorInterface();
        }
        return editor_;
    }

    void buttonPressed(int n, bool val) override {}

    void encoderPressed(int n, bool val) override {}

    void encoderTurned(int n, int val) override {}

    void inputEnabled(int n, bool val) override {}

    void outputEnabled(int n, bool val) override {}

    void getState(void** buffer, size_t* size) override {}

    void setState(void* buffer, size_t size) override {}

    void prepare(double sampleRate, int samplesPerBlock) override {}

    void process(float** channelData, int numChannels, int numSamples) override {} 

private:
    Percussa::SSP::PluginEditorInterface* editor_ = nullptr;
};

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor* createDescriptor() {
    static std::vector<std::string> inNames = {"In 1", "In 2"};
    static std::vector<std::string> outNames = {"Out 1", "Out 2"};
    auto desc = new Percussa::SSP::PluginDescriptor;
    desc->name = "TEST";
    desc->descriptiveName = "Test";
    desc->manufacturerName = "Perc";
    desc->version = "1.0.1";
    desc->fileOrIdentifier = "TEST";
    desc->uid = 0x54455354;
    desc->inputChannelNames = inNames;
    desc->outputChannelNames = outNames;
    return desc;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface* createInstance() {
    return new TestPluginInterface();
}

extern "C" __attribute__ ((visibility("default")))
void getApiVersion(unsigned& major, unsigned& minor) {
    major = Percussa::SSP::API_MAJOR_VERSION;
    minor = Percussa::SSP::API_MINOR_VERSION;
}
