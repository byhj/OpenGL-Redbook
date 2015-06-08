
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

char *textFileRead(const char *fn);  

#define WINDOW_PLATFORM

class Shader {
public:
	Shader(){}
	~Shader(){}
	Shader(std::string shaderName):name(shaderName) {}
	void init();
	void attach(int type, char *filename);
	void link();
	void interfaceInfo();
	void use(void) 
	{
		glUseProgram(program);
	}
	void end(void) 
	{
		glUseProgram(0);
	}

	GLuint GetProgram()
	{
        return program;
	}

private:
	std::vector<GLuint> handles; //shader handle
	GLuint program;   //shader program
	std::string name;   //shader class name
};

char *textFileRead( char *fn) {  //read the shader code
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

}

void Shader::attach(int type, char* filename) //连接不同种类shader
{
		char* mem = textFileRead(filename);
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
		

		glCompileShader(handle);
		
		GLint compileSuccess=0;
		GLchar compilerSpew[256];

		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
		
		if (!compileSuccess) {
			glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader %s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);
			
			while(1);;
		}
		handles.push_back(handle); //存储创建的shader handle以供连接所用
}

void Shader::link()
{
    program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) {
		glAttachShader(program, handles[i]); //将前面创建的shader添加到program
		
	}
	glLinkProgram(program);  //连接shader program
	

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) {
			glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
			
			while(1);;
	}
		printf("%s linked successful\n",name.c_str());
	
}

void Shader::interfaceInfo()
{
	std::cout << "--------------------" << name << " Interface------------------------" << std::endl; 
	GLint outputs = 0;
	glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT,  GL_ACTIVE_RESOURCES, &outputs);
	static const GLenum props[] = {GL_TYPE, GL_LOCATION};
	GLint params[2];
	GLchar name[64];
	const char *type_name;

    if (outputs > 0)
	   std::cout << "----------Input-----------" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, params);
		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

    glGetProgramInterfaceiv(program, GL_PROGRAM_OUTPUT,  GL_ACTIVE_RESOURCES, &outputs);
    if (outputs > 0)
	   std::cout << "----------Onput-----------" << std::endl;
	std::cout << std::endl;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_PROGRAM_OUTPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_PROGRAM_OUTPUT, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}
	
	glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
	  std::cout << "------Uniform Block-------" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_UNIFORM_BLOCK, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}


	glGetProgramInterfaceiv(program, GL_UNIFORM,  GL_ACTIVE_RESOURCES, &outputs);
	if (outputs > 0)
		std::cout << "----------Uniform---------" << std::endl;

	if (outputs > 10)
		return ;
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_UNIFORM, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_UNIFORM, i, 2, props, 2, NULL, params);

		type_name = name;
		//std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin: " << params[1] << std::endl;
	}

}


#endif

