#include "Shader.h"
#include "Mesh.h"

Shader::Shader(string vFile, string fFile, string gFile, string tcsFile, string tesFile)	{
	program		= glCreateProgram();
	objects[SHADER_VERTEX]		= GenerateShader(vFile	 ,GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT]	= GenerateShader(fFile,GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY]	= 0;
	objects[SHADER_TCS]			= 0;
	objects[SHADER_TES]			= 0;

	glAttachShader(program,objects[SHADER_VERTEX]);
	glAttachShader(program,objects[SHADER_FRAGMENT]);

	if(!gFile.empty()) {
		objects[SHADER_GEOMETRY]	= GenerateShader(gFile,GL_GEOMETRY_SHADER);
		glAttachShader(program,objects[SHADER_GEOMETRY]);
	}
	if(!tcsFile.empty()) {
		objects[SHADER_TCS]	= GenerateShader(tcsFile,GL_TESS_CONTROL_SHADER);
		glAttachShader(program,objects[SHADER_TCS]);
	}
	if(!tesFile.empty()) {
		objects[SHADER_TES]	= GenerateShader(tesFile,GL_TESS_EVALUATION_SHADER);
		glAttachShader(program,objects[SHADER_TES]);
	}

	SetDefaultAttributes();
	loadFailed = !LinkProgram();
}

Shader::~Shader(void)	{
	for(int i = 0; i < SHADER_MAX; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

bool	Shader::LoadShaderFile(string from, string &into)	{
	ifstream	file;
	string		temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if(!file.is_open()){
		cout << "File does not exist!" << endl;
		return false;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}

	cout << into << endl << endl;

	file.close();
	cout << "Loaded shader text!" << endl << endl;
	return true;
}

GLuint	Shader::GenerateShader(string from, GLenum type)	{
	cout << "Compiling Shader..." << endl;

	string load;
	if(!LoadShaderFile(from,load)) {
		cout << "Compiling failed!" << endl;
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	{
		cout << "Compiling failed!" << endl;
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "Compiling success!" << endl << endl;
	loadFailed = false;
	return shader;
}

bool Shader::LinkProgram()	{
	if(loadFailed) {
		return false;
	}
	glLinkProgram(program); 

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	if (code != GL_TRUE)	{
		char error[2048];

		glGetProgramInfoLog(program, 2048, NULL, error);

		cout << string(error) << endl;
	}

	return code == GL_TRUE ?  true : false;
}

void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}