#include "PanelWindow.h"
#include "ResourceManager.h"

using namespace std;

//--------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------
PanelWindow::PanelWindow( void )	{
	PanelSimple();

	ResourceManager& res = ResourceManager::getInstance();
	
	loadSkin( "bfs" );

	borderSize = 8;
}



void PanelWindow::loadSkin( string name )	{

	ResourceManager& res = ResourceManager::getInstance();
	
	string nameFile = "/usr/share/wmcgl/images/" + name;
	m_tex_tl = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_ul.png") );
	m_tex_tr = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_ur.png") );
	m_tex_bl = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_dl.png") );
	m_tex_br = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_dr.png") );
	m_tex_t = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_u.png") );
	m_tex_b = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_d.png") );
	m_tex_l = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_l.png") );
	m_tex_r = ((Texture2D*)res.LoadResource(ResourceManager::TEXTURE2D, nameFile + "_r.png") );
	
	borderSize = 8;
}

void PanelWindow::loadSkin( SKIN s )	{
	switch( s )	{
		case STD:	loadSkin( "bfs" );		break;
		case RED:	loadSkin( "rouge" );	break;
		case BLACK:	loadSkin( "fen-2" );	break;
	}
}


void PanelWindow::setPosAndSize(int x0, int y0, int dx0, int dy0)	{
	x=x0; y=y0; dx=dx0 ;dy=dy0;
};


void PanelWindow::setPos(int x0, int y0) {
	x=x0; y=y0;
};


void PanelWindow::setdSize(int dx0, int dy0)	{
	dx=dx0 ;dy=dy0;
};


void PanelWindow::updatePos() {
	Panel::updatePos();
}


//--------------------------------------------------------------
//	Opengl
//--------------------------------------------------------------

void PanelWindow::displayGLtex( Texture2D* pTex, float X, float Y, float DX, float DY )	{
	pTex->Bind(0);
	
	glBegin(GL_QUADS);

	glTexCoord2f(0.05, 0.05);
	glVertex2f(X, Y);

	glTexCoord2f(0.05, 0.95);
	glVertex2f(X, Y + DY);

	glTexCoord2f(0.95, 0.95);
	glVertex2f(X + DX, Y + DY);

	glTexCoord2f(0.95, 0.05);
	glVertex2f(X + DX, Y);

	glEnd();

	pTex->Unbind(0);

}





void PanelWindow::displayGL( void )	{
	if ( !visible )			return;
	
	Panel::displayGL();
	
	float X, Y, DX, DY;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/*
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	*/
	
	// corners
	X = getX() - borderSize;
	Y = getY() - borderSize;
	DX = borderSize;
	DY = borderSize;
	displayGLtex( m_tex_tl, X, Y, DX, DY );

	X = getX() + getDX();
	Y = getY() - borderSize;
	DX = borderSize;
	DY = borderSize;
	displayGLtex( m_tex_tr, X, Y, DX, DY );
	
	X = getX() - borderSize;
	Y = getY() + getDY();
	DX = borderSize;
	DY = borderSize;
	displayGLtex( m_tex_bl, X, Y, DX, DY );

	X = getX() + getDX();
	Y = getY() + getDY();
	DX = borderSize;
	DY = borderSize;
	displayGLtex( m_tex_br, X, Y, DX, DY );
	
	// border
	X = getX() - borderSize;
	Y = getY();
	DX = borderSize;
	DY = getDY();
	displayGLtex( m_tex_l, X, Y, DX, DY );

	X = getX() + getDX();
	Y = getY();
	DX = borderSize;
	DY = getDY();
	displayGLtex( m_tex_r, X, Y, DX, DY );
	
	X = getX();
	Y = getY() - borderSize;
	DX = getDX();
	DY = borderSize;
	displayGLtex( m_tex_t, X, Y, DX, DY );

	X = getX();
	Y = getY() + getDY();
	DX = getDX();
	DY = borderSize;
	displayGLtex( m_tex_b, X, Y, DX, DY );
	/*
	*/
}

