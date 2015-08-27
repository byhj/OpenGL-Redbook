#include "Shader.h"
#include <fstream>

namespace byhj
{

namespace ogl
{

//read the Shadercode
char * Shader::textFileRead( char *fn) {  //read the shader code
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  

		fopen_s(&fp , fn, "rt");
		if (!fp){

#ifdef WINDOW_PLATFORM
			MessageBox(NULL, "Can not open the shader file", "Error",  MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "Can not open the shader file:" << fn << std::endl;

#endif
			return NULL;
		}
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
			std::cout << "Fail to open the shader file" << std::endl;
	}  
	return content;  
}  



void Shader::init()
{
	m_Program = glCreateProgram();
}

void Shader::attach(int type, char* filename) //连接不同种类Shader
{
	auto mem = textFileRead(filename);
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar**)(&mem), 0);

	glCompileShader(handle);

	GLint compileSuccess=0;
	GLchar compilerSpew[256];

	glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess) 
	{
		glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
		printf("Shader%s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);

		while(1);;
	}
	glAttachShader(m_Program, handle); 

}

void Shader::link()
{
	glLinkProgram(m_Program);  

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(m_Program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) 
	{
		glGetProgramInfoLog(m_Program, sizeof(compilerSpew), 0, compilerSpew);
		printf("ShaderLinker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
		while(1);;
	}

	std::cout << m_Name << " linked successful" << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
}

void Shader::info()
{
	std::cout << "------------------------------" << m_Name << " Interface-------------------------" 
		      << std::endl;

	GLint outputs = 0;
	glGetProgramInterfaceiv(m_Program, GL_PROGRAM_INPUT,  GL_ACTIVE_RESOURCES, &outputs);
	static const GLenum props[] = {GL_TYPE, GL_LOCATION};
	GLint params[2];
	GLchar name[64];
	const char *type_name;

	if (outputs > 0)
		std::cout << "***Input***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_Program, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(m_Program, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, params);
		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_Program, GL_PROGRAM_OUTPUT,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Onput***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_Program, GL_PROGRAM_OUTPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_Program, GL_PROGRAM_OUTPUT, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_Program, GL_UNIFORM_BLOCK,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Uniform Block***" << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_Program, GL_UNIFORM_BLOCK, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_Program, GL_UNIFORM_BLOCK, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(m_Program, GL_UNIFORM,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "***Uniform***" << std::endl;
	if (outputs > 100)
		return ;
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(m_Program, GL_UNIFORM, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(  m_Program, GL_UNIFORM, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
}

void Shader::use() const
{
	glUseProgram(m_Program);
}
void Shader::end() const
{
	glUseProgram(0);
}

GLuint Shader::GetProgram() const
{
	return m_Program;
}

}

}