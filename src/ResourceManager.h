#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Singleton.h"
#include <iostream>
#include <map>
#include <string>

#include "Texture2D.h"
#include "Shader.h"
#include <assert.h>

SINGLETON_BEGIN( ResourceManager )
public:
	enum RES_TYPE {TEXTURE2D, SHADER};

	void* LoadResource(RES_TYPE type, const std::string& name);
	void* LoadResourceM(RES_TYPE type, const std::string& name);
	void* NewResource(void* data, const std::string& name);
	void  List(RES_TYPE);
	void  ListShaders(void);
	void  ListTextures(void);
	

	inline void*			getResource(const std::string& name)		{assert(m_ResDB.find(name)!=m_ResDB.end()); return m_ResDB.find(name)->second;}
	inline Texture2D*		getTexture2D(const std::string& name)		{return (Texture2D*)getResource(name);}
	inline Shader*			getShader(const std::string& name)			{return (Shader*)getResource(name);}

	bool Delete( void * );
	void Destroy();
	
	void DeleteTexture2D(Texture2D * ptr)								{ delete ptr;}
	void DeleteShader(Shader * ptr)										{ delete ptr;}

protected:
	~ResourceManager();

private:
	std::map<std::string, void*>	m_ResDB;
	std::map<std::string, RES_TYPE>	m_TypeDB;
	std::map<std::string, int>		m_UseDB;

SINGLETON_END()

#endif

