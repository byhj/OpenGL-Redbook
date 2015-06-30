#ifndef GLDEBUG_H
#define GLDEBUG_H

#include <GL/glew.h>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

void DebugOutputToFile(unsigned int source, unsigned int type, unsigned int id,
									unsigned int severity, const char* message)
{
	FILE* f;
	f = fopen("Debug.txt","a");
	if(f)
	{
		char debSource[16], debType[20], debSev[5];
		if(source == GL_DEBUG_SOURCE_API_ARB)
			strcpy(debSource, "OpenGL");
		else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
			strcpy(debSource, "Windows");
		else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
			strcpy(debSource, "Shader Compiler");
		else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
			strcpy(debSource, "Third Party");
		else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
			strcpy(debSource, "Application");
		else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
			strcpy(debSource, "Other");

		if(type == GL_DEBUG_TYPE_ERROR_ARB)
			strcpy(debType, "Error");
		else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
			strcpy(debType, "Deprecated behavior");
		else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			strcpy(debType, "Undefined behavior");
		else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
			strcpy(debType, "Portability");
		else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
			strcpy(debType, "Performance");
		else if(type == GL_DEBUG_TYPE_OTHER_ARB)
			strcpy(debType, "Other");

		if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
			strcpy(debSev, "High");
		else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
			strcpy(debSev, "Medium");
		else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
			strcpy(debSev, "Low");

		fprintf(f,"Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
			debSource,debType,id,debSev,message);
		fclose(f);
	}
}

void DebugOutput(unsigned int source, unsigned int type, unsigned int id,
					   unsigned int severity, const char* message)
{

		char debSource[16], debType[20], debSev[10];
		if(source == GL_DEBUG_SOURCE_API_ARB)
			strcpy(debSource, "OpenGL");
		else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
			strcpy(debSource, "Windows");
		else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
			strcpy(debSource, "Shader Compiler");
		else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
			strcpy(debSource, "Third Party");
		else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
			strcpy(debSource, "Application");
		else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
			strcpy(debSource, "Other");

		if(type == GL_DEBUG_TYPE_ERROR_ARB)
			strcpy(debType, "Error");
		else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
			strcpy(debType, "Deprecated behavior");
		else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			strcpy(debType, "Undefined behavior");
		else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
			strcpy(debType, "Portability");
		else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
			strcpy(debType, "Performance");
		else if(type == GL_DEBUG_TYPE_OTHER_ARB)
			strcpy(debType, "Other");

		if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
			strcpy(debSev, "High");
		else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
			strcpy(debSev, "Medium");
		else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
			strcpy(debSev, "Low");
		else 
			strcpy(debSev, "Other");

		if (strcmp(debSev, "Other") != 0 )
		{
		   printf("--------------------------------------------------------------------------------");
#ifdef IN32
		   MessageBox(NULL, message, "GL Error", MB_OK);
		   exit(0);
#else
		   printf("Source:%s\tType:%s\tID:%d\tSeverity:%s\nMessage:%s\n",
			   debSource,debType,id,debSev,message);
#endif
		}

}

void CheckDebugLog()
{
	unsigned int count = 10; // max. num. of messages that will be read from the log
	int bufsize = 2048;

	unsigned int* sources      = new unsigned int[count];
	unsigned int* types        = new unsigned int[count];
	unsigned int* ids          = new unsigned int[count];
	unsigned int* severities   = new unsigned int[count];
	int* lengths = new int[count];

	char* messageLog = new char[bufsize];

	unsigned int retVal = glGetDebugMessageLogARB(count, bufsize, sources, types, ids,
		                                          severities, lengths, messageLog);
	if(retVal > 0)
	{
		unsigned int pos = 0;
		for(unsigned int i=0; i<retVal; i++)
		{
			DebugOutput(sources[i], types[i], ids[i], severities[i], &messageLog[pos]);
			pos += lengths[i];
		}
	}
	delete [] sources;
	delete [] types;
	delete [] ids;
	delete [] severities;
	delete [] lengths;
	delete [] messageLog;
}

void CALLBACK DebugCallback(unsigned int source, unsigned int type,
								   unsigned int id, unsigned int severity,
								   int length, const char* message, const void* userParam)
{
	     DebugOutput(source, type, id, severity, message);
}


void  EmitGLError(unsigned int id, char* message, int length)
{
	glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_ERROR_ARB, id, 
		                    GL_DEBUG_SEVERITY_HIGH_ARB, length, message);
}
#endif