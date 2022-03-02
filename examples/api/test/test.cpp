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
	GLuint VBO = 0; 
	unsigned int shaderProgram = 0; 

	static constexpr float projXmin   =  0.0f;
	static constexpr float projXmax   =  1.0f;
	static constexpr float projYmin   =  0.0f;
	static constexpr float projYmax   =  1.0f;
	static constexpr float projZmin   = -1.0f;
	static constexpr float projZmax   =  1.0f;

	float z = projZmax; 
	float w = (projXmax-projXmin)*0.1f; 
	float h = (projYmax-projYmin)*0.1f;
	float midx = (projXmax-projXmin)*0.5f;
	float midy = (projYmax-projYmin)*0.5f;
	float x = 0;
	float y = 0;

	glm::mat4 proj = glm::ortho(
		projXmin-0.25f,
		projXmax+0.25f,
		projYmin,
		projYmax,
		projZmin,
		projZmax
	); 

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	GLint mLoc = 0; 
	GLint vLoc = 0; 
	GLint pLoc = 0; 

public:
	TestPluginEditorInterface() {

		static const char* vertexShaderSource = R"(
			#version 300 es
			layout (location = 0) in vec3 aPos;
			uniform mat4 model; 
			uniform mat4 view;
			uniform mat4 proj; 
			void main() {
				gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
			}
		)";

		static const char* fragmentShaderSource = R"(
			#version 300 es
			precision highp float; 
			out vec4 FragColor;
			void main() { 
				FragColor = vec4(1.0, 1.0, 1.0, 1.0);
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

		// set proj/view/model matrices in shader 
		glUseProgram(shaderProgram);

		mLoc = glGetUniformLocation(shaderProgram, "model");
		vLoc = glGetUniformLocation(shaderProgram, "view");
		pLoc = glGetUniformLocation(shaderProgram, "proj");

		view = glm::translate(view, glm::vec3(0.5f, 0.5f, 0.0f)); 

		glUniformMatrix4fv(mLoc, 1, false, glm::value_ptr(model)); 
		glUniformMatrix4fv(vLoc, 1, false, glm::value_ptr(view)); 
		glUniformMatrix4fv(pLoc, 1, false, glm::value_ptr(proj)); 

		std::clog << __FUNCTION__ << ": set proj/view/model matrices" << std::endl; 

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), 
		// and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		int v = 0; 
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v); 
		std::clog << __FUNCTION__ << ": max vertex attribs: " << v << std::endl; 

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(float), NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// note that this is allowed, the call to glVertexAttribPointer registered 
		// VBO as the vertex attribute's bound vertex buffer object so afterwards 
		// we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// You can unbind the VAO afterwards so other VAO calls won't accidentally 
		// modify this VAO, but this rarely happens. Modifying other VAOs requires 
		// a call to glBindVertexArray anyways so we generally don't unbind VAOs 
		// (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0); 

		std::clog << __FUNCTION__ << ": vertex buffers written" << std::endl; 
	}

	~TestPluginEditorInterface() {
	
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
	}

	void frameStart() override {}
	void visibilityChanged(bool b) override {}
	void renderToImage(unsigned char *buffer, int width, int height) override {}

	float counter = 0.0f; 

	// called after renderToImage, when all OpenGLES geometry is being drawn. 
	void draw(int width, int height) override {

		float ar = width;
		ar /= height; 

		//glClearColor(0.0f, 0.0f, counter, 1.0f);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); 

		x = 0; 
		y = 0; 
		w = 0.25f; 
		h = std::sqrt(3)/2*w; 
		z = 1.0f; 

		float vertices[3][3] = {
			{ x,		y,		z },
			{ x + w,	y,		z },
			{ x + w*0.5f,	y + h,		z },
		}; 

		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// rotate model matrix by 1 degrees around z axis and update in shader
		model = glm::rotate(
				glm::mat4(1.0f), 
				glm::radians(counter), 
				glm::vec3(0.0f, 0.0f, 1.0f)); 

		glUniformMatrix4fv(mLoc, 1, false, glm::value_ptr(model)); 

		proj = glm::ortho(
			-ar, ar,
			-1.0f, 1.0f, 
			-1.0f, 1.0f 
		); 

		glUniformMatrix4fv(pLoc, 1, false, glm::value_ptr(proj)); 

		// draw triangle 
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); 
/*
		x += 0.001f; 
		y += 0.001f; 
		if (x > projXmax) x -= projXmax; 
		if (y > projYmax) y -= projYmax; 
*/
		counter += 1.0f; 
		if (counter > 360.0f) 
			counter -= 360.0f; 
		//std::clog << __FUNCTION__ << ": counter: " << counter << std::endl; 
		//throw std::runtime_error("stop"); 
	}
};

class TestPluginInterface : public Percussa::SSP::PluginInterface {
public:
    TestPluginInterface() : editor_(nullptr) {

    }

    ~TestPluginInterface() {}

    Percussa::SSP::PluginEditorInterface *getEditor() override {
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

    void getState(void **buffer, size_t *size) override {}

    void setState(void *buffer, size_t size) override {}

    void prepare(double sampleRate, int samplesPerBlock) override {}

    void process(float **channelData, int numChannels, int numSamples) override {

    }

private:
    Percussa::SSP::PluginEditorInterface *editor_ = nullptr;
};


extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
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
Percussa::SSP::PluginInterface *createInstance() {
    return new TestPluginInterface();
}

extern "C" __attribute__ ((visibility("default")))
void getApiVersion(unsigned& major, unsigned& minor) {
    major = Percussa::SSP::API_MAJOR_VERSION;
    minor = Percussa::SSP::API_MINOR_VERSION;
}
