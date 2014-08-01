#include <stdio.h>
#include "ResourceManager.h"
//#include "Console.h"

using namespace std;


ResourceManager::~ResourceManager()
{
	cout << "Destructeur ResourceManager ... " << endl;
	Destroy();
}





void ResourceManager::Destroy()
{
	for(std::map<std::string, void*>::iterator it = m_ResDB.begin(); it != m_ResDB.end(); it++) {
		string 	key = it->first;
		switch( m_TypeDB[it->first] )
		{
			case TEXTURE2D: {
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				delete (Texture2D*)(*it).second;
				break;}
			case SHADER: {
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				delete (Shader*)(*it).second;
				break;}
			/*
			case TEXTURECUBEMAP: {
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				delete (TextureCubemap*)(*it).second;
				break;}
			case MESH: {
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				delete (Mesh*)(*it).second;
				break;}
			*/
		}

		//delete (*it).second;
		(*it).second = NULL;
	}
	m_ResDB.clear();
	m_UseDB.clear();
}




bool ResourceManager::Delete( void * Res )	
{
	string key;
	
	for(std::map<std::string, void*>::iterator it = m_ResDB.begin(); it != m_ResDB.end(); it++)
	{
		if ( (*it).second == Res )
		{
			key = it->first;
			m_UseDB[key]--;
			if (  m_UseDB[key] == 0 )
			{
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< Res << std::endl;
				
				switch( m_TypeDB[key] )
				{
					case TEXTURE2D: 		DeleteTexture2D( (Texture2D*)Res );			break;
					case SHADER:			DeleteShader( (Shader*)Res );				break;
					//case TEXTURECUBEMAP:	DeleteCubeMap( (TextureCubemap*)Res );		break;
					//case MESH:				DeleteMesh( (Mesh*)Res );					break;
				}
				
				(*it).second = NULL;
				m_ResDB.erase( it );
		
				for(map<string, RES_TYPE>::iterator it_type = m_TypeDB.begin(); it_type != m_TypeDB.end(); it_type++) {
					if ( it_type->first == key )	{
						m_TypeDB.erase( it_type );
					}
				}
			
				for(map<string, int>::iterator it_use = m_UseDB.begin(); it_use != m_UseDB.end(); it_use++) {
					if ( it_use->first == key )	{
						m_UseDB.erase( it_use );
					}
				}
				return true;
			}
			else
			{
				cout << key << "  utilise " << m_UseDB[key] << " fois " << Res << endl;
			}
			return false;
		}
	}
	cout << "Ressource non trouve : " << Res << endl;
	return false;
	//m_ResDB.clear();
}

void* ResourceManager::LoadResource(RES_TYPE type, const std::string& name)
{
	std::cout << "Loading resource " << name << "   --  ";//  << std::endl;
	// La ressource est déja chargée, on ne la recharge pas :
	if(m_ResDB.find(name) != m_ResDB.end()) {
		if ( name.find( ".obj" ) != std::string::npos )	{
			/*
			if ( name.find( "plam" ) == std::string::npos ) { 
				Mesh* pMesh;
				pMesh = (Mesh*)m_ResDB.find(name)->second;
				int nb = pMesh->getFace().size();
				//Console::getInstance().AddNbFace(nb);
				//std::cout << "Objet : " << name.c_str();
				std::cout << " " << nb << " faces" ;// << std::endl;
			}
			*/
		}
		std::cout << "  deja charge   --  " << m_ResDB.find(name)->second << std::endl;;
		m_UseDB[name] += 1;
		return m_ResDB.find(name)->second;
	}
	

	// on charge la ressource
	void* ptr = NULL;
	switch(type) {
	case TEXTURE2D: {
		std::cout << "Chargement ....";//<< std::endl;
		ptr = new Texture2D();
		((Texture2D*)ptr)->Load(name);
		break;}
	case SHADER: {
		//std::cout << std::endl;
		ptr = new Shader();
		((Shader*)ptr)->Load(name);
		break;}
	/*
	case TEXTURECUBEMAP: {
		//std::cout << std::endl;
		ptr = new TextureCubemap();
		((TextureCubemap*)ptr)->Load(name);
		break;}
	case MESH: {
		ptr = new Mesh();
		((Mesh*)ptr)->Load(name);
		int nb = ((Mesh*)ptr)->getFace().size();
		//Console::getInstance().AddNbFace(nb);
		break;}
		*/
	}

	if(!ptr) { std::cout<<std::endl; return NULL;}

	m_ResDB[name] = ptr;
	m_TypeDB[name] = type;
	m_UseDB[name] = 1;
	std::cout << "  --  " << ptr << std::endl;;
	return ptr;
}

void* ResourceManager::LoadResourceM(RES_TYPE type, const std::string& name)
{
	// La ressource est déja chargée, on ne la recharge pas :
	/*
	if(m_ResDB.find(name) != m_ResDB.end())
		return m_ResDB.find(name)->second;
	*/

	std::cout << "Loading resource " << name ;

	// on charge la ressource
	void* ptr = NULL;
	/*
	switch(type) {
	case MESH: {
		ptr = new Mesh();
		((Mesh*)ptr)->Load(name);
		int nb = ((Mesh*)ptr)->getFace().size();
		//Console::getInstance().AddNbFace(nb);
		break;}
	}
	*/

	if(!ptr) {
		std::cout << "Impossible de charger :" << name.c_str() << std::endl;
		return NULL;
	}

	//std::string newName = name + "001";
	//m_ResDB[newName] = ptr;
	m_ResDB[name] = ptr;
	std::cout << " " << ptr ;
	return ptr;
}

void* ResourceManager::NewResource(void* data, const std::string& name)
{
	if(!data) return NULL;

	// La ressource est déja chargée, on ne la recharge pas :
	if(m_ResDB.find(name) != m_ResDB.end())
		return m_ResDB.find(name)->second;

	std::cout << name.c_str() << std::endl;

	m_ResDB[name] = data;
	return data;
}


/*
void ResourceManager::List(void)
{
	for(std::map<std::string, void*>::iterator it = m_ResDB.begin(); it != m_ResDB.end(); it++) {
		std::string key = (*it).first;
		Console::getInstance().AfficheLigne( key.c_str() );
		std::cout << key.c_str() << std::endl;
	}
}

*/

void ResourceManager::List( RES_TYPE TypeRecherche )
{
	char			text[80];
	for(std::map<std::string, RES_TYPE>::iterator it = m_TypeDB.begin(); it != m_TypeDB.end(); it++) {
		if ( (*it).second == TypeRecherche )
		{
			std::string key = (*it).first;
			sprintf( text, "  %s", key.c_str() );
			//Console::getInstance().AfficheLigne( text );
			std::cout << text << std::endl;
		}
	}
}







void ResourceManager::ListShaders(void)
{
	char			text[80];
	
	sprintf( text, "Shaders" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( SHADER );
}



/*
void ResourceManager::ListObjs(void)
{
	char			text[80];
	
	sprintf( text, "Meshs" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( MESH );
}


void ResourceManager::ListCubeMap(void)
{
	char			text[80];
	
	sprintf( text, "CubeMap" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( TEXTURECUBEMAP );
}

*/


void ResourceManager::ListTextures(void)
{
	char			text[80];
	
	sprintf( text, "Textures2D" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( TEXTURE2D );
}


















