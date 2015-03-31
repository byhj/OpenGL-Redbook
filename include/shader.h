
#include <vector>
#include <string>
using namespace std;

char *textFileRead(const char *fn);  
// GL ERROR CHECK
#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)
int CheckGLError(char *file, int line);
//GLuint loadTexture(const char* theFileName);

class Shader {
public:
	Shader(std::string shaderName):name(shaderName) {}
	void init();
	void attach(int type, char *filename);
	void link();
	void setUniformMatrix4fv(char* varname, GLsizei count, GLboolean transpose, GLfloat *value);
	void use(void) {
		glUseProgram(program);
		CHECK_GL_ERROR();
	}
	void end(void) {
		glUseProgram(0);
		CHECK_GL_ERROR();
	}
	vector<GLuint> handles; //shader handle
	GLuint program;   //shader program
	string name;   //shader class name
};

char *textFileRead(const char *fn) {  //read the shader code
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  
		fopen_s(&fp , fn, "rt");

		if (fp != NULL) {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
		else 
			cout << "Fail to open the shader file" << endl;
	}  
	return content;  
}  

int CheckGLError(char *file, int line)
{
	//return 0;
	GLenum glErr, glErr2;
	int retCode = 0;

	glErr = glErr2 = glGetError();
	while (glErr != GL_NO_ERROR) 
	{
	   char* str1 = (char*)gluErrorString(glErr);
	   if (str1)
			cout << "GL Error #" << glErr << "(" << str1 << ") " << " in File " << file << " at line: " << line << endl;
	   else
			cout << "GL Error #" << glErr << " in File " << file << " at line: " << line << endl;
		retCode = 1;
		glErr = glGetError();
	}
	if (glErr2 != GL_NO_ERROR) while(1) Sleep(100);;

	return 0;
}
void Shader::init()
{
		//1、查看GLSL和OpenGL的版本  
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion =   
	glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	cout << "GL Vendor    :" << vendor << endl;  
	cout << "GL Renderer  : " << renderer << endl;  
	cout << "GL Version (string)  : " << version << endl;  
	cout << "GL Version (integer) : " << major << "." << minor << endl;  
	cout << "GLSL Version : " << glslVersion << endl;   
	cout << "-----------------------------------------------------------------------" << endl;
}

void Shader::attach(int type, char* filename) //连接不同种类shader
{
		char* mem = textFileRead(filename);
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
		CHECK_GL_ERROR();

		glCompileShader(handle);
		CHECK_GL_ERROR();
		GLint compileSuccess=0;
		GLchar compilerSpew[256];

		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
		CHECK_GL_ERROR();
		if (!compileSuccess) {
			glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader %s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);
			CHECK_GL_ERROR();
			while(1);;
		}
		handles.push_back(handle); //存储创建的shader handle以供连接所用
}

void Shader::link()
{
    program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) {
		glAttachShader(program, handles[i]); //将前面创建的shader添加到program
		CHECK_GL_ERROR();
	}
	glLinkProgram(program);  //连接shader program
	CHECK_GL_ERROR();

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) {
			glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
			CHECK_GL_ERROR();
			while(1);;
	}
		printf("%s linked successful\n",name.c_str());
	CHECK_GL_ERROR();
}

void Shader::setUniformMatrix4fv(char* varname, GLsizei count, GLboolean transpose, GLfloat *value)
{
		GLint loc = glGetUniformLocation(program, varname);
		if (loc==-1) {
			printf("Variable \"%s\" in shader \"%s\" not found\n",varname,name.c_str());
			while(1);;
		};
		glUniformMatrix4fv(loc, count, transpose, value);
		CHECK_GL_ERROR();
}
