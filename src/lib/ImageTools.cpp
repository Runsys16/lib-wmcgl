#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include "ImageTools.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <assert.h>
#include <mutex>
#include <string>

#undef _UNICODE
#include <IL/il.h>




#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
//#define DEBUG
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool isFileExist( char* pathname )
{
    struct stat   buffer;   
    if ( stat( pathname, &buffer) == 0 )
    {
        if ( (buffer.st_mode & S_IFMT) ==  S_IFREG )
        {
            return true;
        }
    }

	return false;

}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

std::mutex lock_load_imageDEVIL;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
namespace _ImageTools {

GLubyte* OpenImagePPM(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d)
{
#ifdef DEBUG
	std::cout <<"  _ImageTools::OpenImagePPM("<< filename <<" w="<< w <<" h="<< h <<" d="<< d <<")"<< std::endl;
#endif

	char head[70];
	int i,j;
	GLubyte * img = NULL;

	FILE * f = fopen(filename.c_str(), "rb");

	if(f==NULL){
		return 0;
	}
	fgets(head,70,f);

	if(!strncmp(head, "P6", 2)){
		i=0;
		j=0;
		while(i<3){
			fgets(head,70,f);

			if(head[0] == '#'){
				continue;
			}

			if(i==0)
				i += sscanf(head, "%d %d %d", &w, &h, &d);
			else if(i==1)
				i += sscanf(head, "%d %d", &h, &d);
			else if(i==2)
				i += sscanf(head, "%d", &d);
		}

		img = new GLubyte[(size_t)(w) * (size_t)(h) * 3];
		if(img==NULL) {
			fclose(f);
			return 0; 
		}

		fread(img, sizeof(GLubyte), (size_t)w*(size_t)h*3,f);
		fclose(f);
	}
	else{
		fclose(f);
	}
	
	return img;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
GLubyte* OpenImageDevIL(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d)
{
#ifdef DEBUG
	std::cout <<"_ImageTools::OpenImageDevIL("<< filename <<" w="<< w <<" h="<< h <<" d="<< d <<")"<< std::endl;
#endif
    lock_load_imageDEVIL.lock();
    
    
	static bool first = true;
	if(first) {
		first = false;

		// Initalisation de DevIL
		ilInit();

		// On indique que l'origine des images se trouve sur le coin haut-gauche
		ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		ilEnable(IL_ORIGIN_SET);

		ilEnable(IL_TYPE_SET);
		ilTypeFunc(IL_UNSIGNED_BYTE);
		

	}

    // Génération d'une nouvelle texture
    ILuint ilTexture = 0;
    ilGenImages(1, &ilTexture);
    ilBindImage(ilTexture);

    // Chargement de l'image
	if (!ilLoadImage((char*)filename.c_str()))
	{
    	std::cout <<"  [Erreur]_Texture::OpenImageDevIL('...')"<< std::endl;
        lock_load_imageDEVIL.unlock();
		return NULL;
    }


    // Récupération de la taille de l'image
	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	d = ilGetInteger(IL_IMAGE_BPP);

	int w0 = w;
	int h0 = h;
	int	d0 = d;
	//std::cout <<  "width:"<< w0 <<" height:"<< h0 <<" bpp:"<< d0 << std::endl;
    ILboolean success;
    
	if(d==4)
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	else
	if(d==3)
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	

    //d = 4;
    // Récupération des pixels de l'image
    const unsigned char* Pixels = ilGetData();

	GLubyte* img = new GLubyte[(size_t)(w) * (size_t)(h) * (size_t)(d) + 8*4];
	memcpy(img, Pixels, (size_t)(w) * (size_t)(h) * (size_t)(d));

    // Suppression de la texture
    ilBindImage(0);
    ilDeleteImages(1, &ilTexture);

    lock_load_imageDEVIL.unlock();
	return img;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
GLubyte* OpenImage(const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d)
{
#ifdef DEBUG
	std::cout <<"_ImageTools::OpenImage("<< filename <<" w="<< w <<" h="<< h <<" d="<< d <<")"<< std::endl;
#endif
	GLubyte* ptr = NULL;
	
	if(filename.find(".ppm") != std::string::npos){
		//ptr = _ImageTools::OpenImagePPM("./textures/"+filename, w, h, d);
		//if ( !ptr ) ptr = _ImageTools::OpenImagePPM(filename, w, h, d);

		std::string f = "./textures/"+filename;
        if( isFileExist((char*)f.c_str()) )
    		ptr =  _ImageTools::OpenImagePPM(f, w, h, d);
        else
    		ptr =  _ImageTools::OpenImagePPM(filename, w, h, d);
	}
	else {
		std::string f = "./textures/"+filename;
        if( isFileExist((char*)f.c_str()) )
    		ptr =  _ImageTools::OpenImageDevIL(f, w, h, d);
        else
    		ptr =  _ImageTools::OpenImageDevIL(filename, w, h, d);
	}

	if ( ptr == NULL )	{ std::cout << "Erreur de chargement de l'image " << filename << std::endl; }

#ifdef DEBUG
	std::cout <<"_ImageTools::OpenImage(...) OK  w="<< w <<" h="<< h <<" d="<< d << std::endl;
#endif
	return ptr;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void OpenImage(const std::string& filename, _ImageData& img)
{
	img.data = OpenImage(filename, img.w, img.h, img.d);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void _ImageData::Destroy()
{
	if(data) {
		delete [] data;
		data = NULL;
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
ivec3 _ImageData::getColor(unsigned int x, unsigned int y) const
{
	int idx = (y * w + x) * d;
	return ivec3( data[idx+0], data[idx+1], data[idx+2]);
}
/*
ivec3 _ImageData::getColor(float x, float y)
{
	vec2  posF(	x, y );
	ivec2 posI(	(int)(posF.x), (int)(posF.y) );
	vec2  posD(	posF.x - posI.x, posF.y - posI.y );

	if(posI.x >= (int)w-1)	posI.x = w-2;
	if(posI.y >= (int)h-1)	posI.y = h-2;

	assert(posI.x>=0 && posI.x<(int)w-1 && posI.y>=0 && posI.y<(int)h-1);

	vec3 c00 = getColor((unsigned int)posI.x + 0, (unsigned int)posI.y + 0);
	vec3 c10 = getColor((unsigned int)posI.x + 1, (unsigned int)posI.y + 0);
	vec3 c01 = getColor((unsigned int)posI.x + 0, (unsigned int)posI.y + 1);
	vec3 c11 = getColor((unsigned int)posI.x + 1, (unsigned int)posI.y + 1);

	vec3 c =  vec3(c00  * (1.0f-posD.x) * (1.0f-posD.y))
			+ vec3(c10  *       posD.x  * (1.0f-posD.y))
			+ vec3(c01  * (1.0f-posD.x) *       posD.y)
			+ vec3(c11  *       posD.x  *       posD.y);
}
*/

}

