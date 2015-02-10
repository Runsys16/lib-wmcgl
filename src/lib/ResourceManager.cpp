#include <stdio.h>
#include "ResourceManager.h"
//#include "Console.h"

//#define DEBUG
//#define DEBUG_CONST

#ifdef DEBUG_WM
#	define DEBUG
#endif


#ifdef DEBUG
#	define DEBUG_CONST
#endif


using namespace std;


_ResourceManager::~_ResourceManager()
{
	#ifdef DEBUG_CONST
	cout << "Destructeur _ResourceManager ... " << endl;
	#endif
	Destroy();
}





void _ResourceManager::Destroy()
{
	for(std::map<std::string, void*>::iterator it = m_ResDB.begin(); it != m_ResDB.end(); it++) {
		string 	key = it->first;
		switch( m_TypeDB[it->first] )
		{
			case TEXTURE2D: {
				#ifdef DEBUG
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				#endif
				delete (_Texture2D*)(*it).second;
				break;}
			case SHADER: {
				#ifdef DEBUG
				std::cout << "Efface la ressource : " << (*it).first <<"    "<< (*it).second << std::endl;
				#endif
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




bool _ResourceManager::Delete( void * Res )	
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
				#ifdef DEBUG
				std::cout << "WM - Efface la ressource : " << (*it).first <<"    "<< Res << std::endl;
				#endif
				
				switch( m_TypeDB[key] )
				{
					case TEXTURE2D: 		DeleteTexture2D( (_Texture2D*)Res );			break;
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
				#ifdef DEBUG
				cout << key << "  utilise " << m_UseDB[key] << " fois " << Res << endl;
				#endif
			}
			return false;
		}
	}
	#ifdef DEBUG
	cout << "WM - Ressource non trouve : " << Res << endl;
	#endif
	return false;
	//m_ResDB.clear();
}

void* _ResourceManager::LoadResource(RES_TYPE type, const std::string& name)
{
	#ifdef DEBUG
	std::cout << "WM - Loading resource " << name << "   --  " << std::endl;
	#endif
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
	#ifdef DEBUG
		std::cout << "  deja charge   --  " << m_ResDB.find(name)->second << std::endl;;
	#endif
		m_UseDB[name] += 1;
		return m_ResDB.find(name)->second;
	}
	

	// on charge la ressource
	void* ptr = NULL;
	switch(type) {
	case TEXTURE2D: {
	#ifdef DEBUG
		std::cout << "  Chargement ...." << std::endl;
	#endif
		ptr = new _Texture2D();
		((_Texture2D*)ptr)->Load(name);
	#ifdef DEBUG
		std::cout << "  pointeur : "<< ptr << std::endl;
	#endif
		break;}
	case SHADER: {
		//std::cout << std::endl;
		ptr = new Shader();
		((Shader*)ptr)->Load(name);
		break;}
	}

	if(!ptr) { std::cout<<std::endl; return NULL;}

	m_ResDB[name] = ptr;
	m_TypeDB[name] = type;
	m_UseDB[name] = 1;
	#ifdef DEBUG
	std::cout << "  --  " << ptr << std::endl;;
	#endif
	return ptr;
}

void* _ResourceManager::LoadResourceM(RES_TYPE type, const std::string& name)
{
	// La ressource est déja chargée, on ne la recharge pas :
	/*
	if(m_ResDB.find(name) != m_ResDB.end())
		return m_ResDB.find(name)->second;
	*/

	#ifdef DEBUG
	std::cout << "Loading resource " << name ;
	#endif

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
	#ifdef DEBUG
		std::cout << "WM - Impossible de charger :" << name.c_str() << std::endl;
	#endif
		return NULL;
	}

	//std::string newName = name + "001";
	//m_ResDB[newName] = ptr;
	m_ResDB[name] = ptr;
	std::cout << " " << ptr ;
	return ptr;
}

void* _ResourceManager::NewResource(void* data, const std::string& name)
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
void _ResourceManager::List(void)
{
	for(std::map<std::string, void*>::iterator it = m_ResDB.begin(); it != m_ResDB.end(); it++) {
		std::string key = (*it).first;
		Console::getInstance().AfficheLigne( key.c_str() );
		std::cout << key.c_str() << std::endl;
	}
}

*/

void _ResourceManager::List( RES_TYPE TypeRecherche )
{
	char			text[80];
	for(std::map<std::string, RES_TYPE>::iterator it = m_TypeDB.begin(); it != m_TypeDB.end(); it++) {
		if ( (*it).second == TypeRecherche )
		{
			std::string key = (*it).first;
			sprintf( text, "  %s", key.c_str() );
			//Console::getInstance().AfficheLigne( text );
	#ifdef DEBUG
			std::cout << text << std::endl;
	#endif
		}
	}
}







void _ResourceManager::ListShaders(void)
{
	char			text[80];
	
	sprintf( text, "Shaders" );
	//Console::getInstance().AfficheLigne( text );
	#ifdef DEBUG
	std::cout << text << std::endl;
	#endif
	List( SHADER );
}



/*
void _ResourceManager::ListObjs(void)
{
	char			text[80];
	
	sprintf( text, "Meshs" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( MESH );
}


void _ResourceManager::ListCubeMap(void)
{
	char			text[80];
	
	sprintf( text, "CubeMap" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( TEXTURECUBEMAP );
}

*/


void _ResourceManager::ListTextures(void)
{
	char			text[80];
	
	sprintf( text, "Textures2D" );
	//Console::getInstance().AfficheLigne( text );
	std::cout << text << std::endl;
	List( TEXTURE2D );
}


















