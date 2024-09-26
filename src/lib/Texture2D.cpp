#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include "Texture2D.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
//#include "FrameBufferObject.h"

//#define DEBUG

bool _Texture2D::Load(const std::string& name)
{
#ifdef DEBUG
	std::cout << "_Texture2D::Load('"<< name <<"')"<< std::endl;
#endif
	if(!_Texture::Load(name))
		return false;

#ifdef DEBUG
	std::cout << "_Texture2D::Load() ... Bind()"<< std::endl;
#endif
	Bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	if(!_Texture::s_bGenerateMipmaps) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	if(!LoadFile(GL_TEXTURE_2D, name))
		return false;

	Unbind();

	return true;
}

bool _Texture2D::Load(GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d)
{
#ifdef DEBUG
	std::cout << "_Texture2D::Load('"<< w <<","<< h <<","<< d <<"')"<< std::endl;
#endif
	if(!_Texture::Load(""))
		return false;

	Bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	if(!s_bGenerateMipmaps) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	LoadData(GL_TEXTURE_2D, ptr, w, h, d);

	Unbind();

	return true;
}




