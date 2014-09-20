#ifndef IMAGETOOLS_H
#define IMAGETOOLS_H

#include <GL/gl.h>
#include <iostream>
#include "Mathlib.h"

// -------------------------------
// Chargement d'images
// retourne les données brutes
// -------------------------------

namespace _ImageTools
{
	class _ImageData {
	public:
		GLubyte*	data;
		unsigned int w, h, d;

		ivec3	getColor(unsigned int x, unsigned int y) const;
	//	ivec3	getColor(float x, float y);

		_ImageData() {w = h = d = 0; data = NULL;}
		~_ImageData() {Destroy();}
		void Destroy();
	};

	void     OpenImage(const std::string& filename, _ImageData& img);
	GLubyte* OpenImage(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d);

	GLubyte* OpenImagePPM(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d);
	GLubyte* OpenImageDevIL(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d);

	

}

#endif

