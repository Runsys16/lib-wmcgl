#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include "Texture.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "ImageTools.h"

bool _Texture::s_bGenerateMipmaps = true;

bool _Texture::LoadFile(GLenum target, const std::string& name)
{
	unsigned int w, h, d;
	//std::cout << std::endl;
	GLubyte* ptr = ImageTools::OpenImage(name, w, h, d);
	if(!ptr) {
		std::cout<<std::endl;
		std::cerr << "[Error] WM -Impossible de charger la _Texture \""<< name <<"\""<< std::endl;
		return false;
	} 
	//std::cout << "Profondeur "<< d << std::endl;
	/*
	else
	{
		static std::string voids = "void.tga";
		ptr = ImageTools::OpenImage( voids, w, h, d);
		if (!ptr) return false;
	}
	*/
	LoadData(target, ptr, w, h, d);

	delete[] ptr;
	return true;
}

void _Texture::LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d)
{
	//std::cout << "_Texture::LoadData()  w=" << w << " h=" << h << " d=" << d << std::endl;
	glTexImage2D(target, 0, d==3?GL_RGB:GL_RGBA, w, h, 0, d==3?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, ptr);
	//std::cout << "_Texture::LoadData()  glTextImage2D" << std::endl;
}



void _Texture::Gen()
{
	Destroy();
	glGenTextures(1, &m_nHandle);
}



bool _Texture::Load(const std::string& name)
{
	Gen();

	if(m_nHandle == 0){
		std::cerr << "Identifiant de texture incorrect" << std::endl;
		return false;
	}

	return true;
}


void _Texture::Destroy()
{
	glDeleteTextures(1, &m_nHandle);
}





void _Texture::Bind() const {
	glBindTexture(getTextureType(), m_nHandle);
}

void _Texture::Bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0+slot);
	glEnable(getTextureType());
	glBindTexture(getTextureType(), m_nHandle);
}

void _Texture::Unbind() const {
	glBindTexture(getTextureType(), 0);
}

void _Texture::Unbind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(getTextureType(), 0);
}


