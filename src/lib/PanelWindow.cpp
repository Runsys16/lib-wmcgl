#include <typeinfo>

#include "PanelWindow.h"
#include "PanelText.h"
#include "ResourceManager.h"


using namespace std;

//--------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------
PanelWindow::PanelWindow( void )	{
	PanelSimple();

	_ResourceManager& res = _ResourceManager::getInstance();
	
	loadSkin( "bfs" );

	borderSize = 8;
}



void PanelWindow::loadSkin( string name )	{

	_ResourceManager& res = _ResourceManager::getInstance();
	
	cout <<"PanelWindow::loadSkin(\""<< name <<"\" ) m_text_tl : "<< m_tex_tl << endl;
	string nameFile = "/usr/share/wmcgl/images/" + name;
	m_tex_tl = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_ul.png") );
	m_tex_tr = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_ur.png") );
	m_tex_bl = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_dl.png") );
	m_tex_br = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_dr.png") );
	m_tex_t = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_u.png") );
	m_tex_b = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_d.png") );
	m_tex_l = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_l.png") );
	m_tex_r = ((_Texture2D*)res.LoadResource(_ResourceManager::TEXTURE2D, nameFile + "_r.png") );
	cout <<"  m_text_tl : "<< m_tex_tl << endl;
	
	
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
	/*
	cout <<"ID : "<< getID() <<" nb childs : "<< childs.size() << endl;
	if ( getID() == 14 || getID() == 15 )	{
		Panel* ps = childs[0];
		cout <<"  typeID : "<< typeid(*ps).name() << endl;
		
		if (ps )	{
			if ( ps->getChilds().size() == 1 && typeid(*ps->getChilds()[0]) == typeid(PanelText) )	{
				cout <<"  typeID : "<< typeid(*ps->getChilds()[0]).name() << endl;
				PanelText* pt = dynamic_cast<PanelText*>(ps->getChilds()[0]);
				cout <<"  text : "<< pt->getText() << endl;
				cout <<"  visible : "<< boolalpha << pt->getVisible() << endl;
				cout <<"  change : "<< boolalpha << pt->bChange << endl;
				cout <<"  pTextGL : "<< pt->pTextGL << endl;
				cout <<"  font : "<< pt->strFont() << endl;
			}
		}
	}
	*/
}


//--------------------------------------------------------------
//	Opengl
//--------------------------------------------------------------

void PanelWindow::displayGLtex( _Texture2D* pTex, float X, float Y, float DX, float DY )	{
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
	//cout << "PanelWindow::displayGL() m_tex_bl : " << m_tex_bl << endl;
	float X, Y, DX, DY;
	
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	
	
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

