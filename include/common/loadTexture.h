#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H

#include <GL/glew.h>
#include <IL/il.h>
#include <iostream>

GLuint loadTexture(const  char* theFileName)
{
	GLuint textureID;			// Create a texture ID as a GLuint
	ILuint imageID;				// Create an image ID as a ULuint
	ilInit();   //初始化IL
	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image
	ILboolean success = ilLoadImage(theFileName); 	// Load the image file

	if (success) 
	{
		glGenTextures(1, &textureID); //创建Opengl纹理接口
		glBindTexture(GL_TEXTURE_2D, textureID);

		//设置纹理的过滤和环绕模式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//将加载的纹理数据转化为OpenGL格式
 	    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		//将数据传入纹理对象中
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
					 0,				// Pyramid level (for mip-mapping) - 0 is the top level
					 ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
					 ilGetInteger(IL_IMAGE_WIDTH),	// Image width
					 ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
					 0,				// Border width in pixels (can either be 1 or 0)
					 ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
					 GL_UNSIGNED_BYTE,		// Image data type
					 ilGetData());			// The actual image data itself
 	}
	else 
		std::cout << "Fail to load the texture!" << std::endl;
 
 	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
	std::cout << "Texture creation successful." << std::endl;
	return textureID; // 返回加载纹理索引
}



#endif