#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <iostream>

// -------------------------------
// Textures abstraites
// -------------------------------

class _Texture
{
public:
	virtual GLenum getTextureType() const = 0;

	void Gen();
	virtual bool Load(const std::string& name);
	void Destroy();

	void Bind(GLuint slot) const;
	void Unbind(GLuint slot) const;

	GLuint getHandle() const {return m_nHandle;} 

	static void EnableGenerateMipmaps(bool b) {s_bGenerateMipmaps=b;}
	void Bind() const;
	void Unbind() const;

	_Texture() {m_nHandle=0;}
	~_Texture() {Destroy();}
	
	unsigned	dx;
	unsigned	dy;
	

protected:
	//void Bind() const;
	//void Unbind() const;
	bool LoadFile(GLenum target, const std::string& name);
	void LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d);
	
protected:
	GLuint	m_nHandle;				// ID de la texture
	static bool s_bGenerateMipmaps;	// Etat pour le chargement : génération ou non des mipmaps
};

#endif

