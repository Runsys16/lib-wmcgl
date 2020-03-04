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






//#define DEBUG 1

bool _Texture::s_bGenerateMipmaps = true;

bool _Texture::LoadFile(GLenum target, const std::string& name)
{
	unsigned int w, h, d;


	GLubyte* ptr = NULL;
	ptr = _ImageTools::OpenImage(name, w, h, d);
	
#ifdef DEBUG
	std::cout << "  _Texture::LoadFile()  w=" << w << " h=" << h << " d=" << d << std::endl;
#endif
	if(ptr == NULL) {
    	std::cout << "  _Texture::LoadFile()  w=" << w << " h=" << h << " d=" << d << std::endl;
		std::cout << "  [Error] WM - Impossible de charger la _Texture \""<< name <<"\""<< std::endl;
		std::cout << "  [Error] WM - Pointeur : NULL"<<  std::endl;
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
	dx = w;
	dy = h;
#ifdef DEBUG
	std::cout << "  _Texture::LoadFile()  w=" << w << " h=" << h << " d=" << d << std::endl;
#endif

	delete[] ptr;
	return true;
}

void _Texture::LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d)
{
	glTexImage2D(target, 0, d==3?GL_RGB:GL_RGBA, w, h, 0, d==3?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, ptr);
#ifdef DEBUG
	std::cout << "_Texture::LoadData()  glTextImage2D" << std::endl;
#endif
}



void _Texture::Gen()
{
#ifdef DEBUG
	std::cout << "_Texture::Gen()" << std::endl;
#endif
	Destroy();
	glGenTextures(1, &m_nHandle);
}



bool _Texture::Load(const std::string& name)
{
#ifdef DEBUG
	std::cout <<"_Texture::Load(name='"<< name <<"')  GenTexture ..." << std::endl;
#endif
	Gen();

	if(m_nHandle == 0){
		std::cout << "  [Erreur] Identifiant de texture incorrect" << std::endl;
		
		int err = glGetError();
		switch(err)
		{
		case GL_NO_ERROR:                       std::cout << "  GL_NO_ERROR" << std::endl;   break;
		case GL_INVALID_ENUM:                   std::cout << "  GL_INVALID_ENUM" << std::endl;   break;
		case GL_INVALID_VALUE:                  std::cout << "  GL_INVALID_VALUE" << std::endl;   break;
		case GL_INVALID_OPERATION:              std::cout << "  GL_INVALID_OPERATION" << std::endl;   break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  std::cout << "  GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;   break;
		case GL_STACK_UNDERFLOW:                std::cout << "  GL_STACK_UNDERFLOW" << std::endl;   break;
		case GL_STACK_OVERFLOW:                 std::cout << "  GL_STACK_OVERFLOW" << std::endl;   break;
		}
		
		return true;
		return false;
	}
	else
	{
#ifdef DEBUG
	    std::cout <<"  m_nHandle = "<< m_nHandle << std::endl;
#endif
	}

	return true;
}


void _Texture::Destroy()
{
#ifdef DEBUG
	std::cout << "_Texture::Destroy()   m_nHandle = "<< m_nHandle << std::endl;
#endif
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


