#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>


#include <iostream>
#include <string>
#include <typeinfo>
#include <string>
#include <vector>

#include <WindowsManager.h>


using namespace std;




GLenum err;
//PanelConsole console(100);

float prevTime = -1.0;

WindowsManager & wm = WindowsManager::getInstance();

int width  = 1500;
int height = 800;
GLdouble fov = 60.0;
PanelConsole *pPC;
std::vector<PanelWindow *>	pw;

void change_fov( void )
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	GLdouble ratio = (GLdouble)width / (GLdouble)height;
	
	gluPerspective((GLdouble)fov, (GLdouble)ratio, (GLdouble)0.1, (GLdouble) 1000.0);
}






static void displayGL(void)
{
	WindowsManager::getInstance().clearBufferGL(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	WindowsManager::getInstance().displayGL();

	glutSwapBuffers();


}





static void reshapeGL(int newWidth, int newHeight)
{

	width  = newWidth;
	height = newHeight;

	//WindowsManager& wm = WindowsManager::getInstance();
	wm.setScreenSize( width, height );

	glViewport(0, 0, (GLint)newWidth, (GLint)newHeight);

	change_fov();
}



static void idleGL(void)
{
	float time = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	
	if ( prevTime < 0 )	{
		prevTime = time;
	}
	else 	{
		float elapsedTime = time - prevTime;
		prevTime = time;
		WindowsManager::getInstance().idleGL( elapsedTime );
	}


	glutPostRedisplay();
}









static void quit(void)
{
	exit(0);
}



static void initGL(int argc, 
		   char **argv)
{
	glClearColor(.8f,.8f,.8f,1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glDisable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



}


static void glutKeyboardFunc(unsigned char key, int x, int y) {
	PanelConsole* console = (PanelConsole*)WindowsManager::getInstance().getByID(5);
	//cout <<"Console addr : "<< console <<"typeof "<< typeid(console).name() << endl;
	WindowsManager::getInstance().keyboardFunc( key, x, y);

	static int n = 0;
	
	
	switch(key){ 
	case 27: 
		{
		quit();
		}
		break;
	case 178: 
		{
			for (int i=0; i<pw.size(); i++ )	{
				int b = pw[i]->getBorderSize();
				if ( b < 0 )	b = 128;
			
				b -=2;
				pw[i]->setBorderSize(b);
			}
			
		}
		break;
	case 9: 
		{
			for (int i=0; i<pw.size(); i++ )	{
				int b = pw[i]->getBorderSize();
				if ( b >= 128 )	b = 0;
			
				b +=2;
				pw[i]->setBorderSize(b);
			}
			
		}
		break;
	}
}

static void glutKeyboardUpFunc(unsigned char key, int x, int y)	{
	WindowsManager::getInstance().keyboardUpFunc( key, x, y);
}

static void glutSpecialFunc(int key, int x, int y)	{
	WindowsManager::getInstance().keyboardSpecialFunc( key, x, y);
}
static void glutSpecialUpFunc(int key, int x, int y)	{
	WindowsManager::getInstance().keyboardSpecialUpFunc( key, x, y);
}

static void glutMouseFunc(int button, int state, int x, int y)	{
	WindowsManager::getInstance().mouseFunc(button, state, x, y);
}

static void glutMotionFunc(int x, int y)	{	
	WindowsManager::getInstance().motionFunc(x, y);
}

static void glutPassiveMotionFunc(int x, int y)	{
	WindowsManager::getInstance().passiveMotionFunc(x, y);
}



void exec_cmd( string cmd  )	{
	printf( "C standard :: exec_cmd( ) \"%s\"\n", cmd.c_str() );
	if ( cmd.compare( "clear" ) == 0 )	{
		pPC->clear();
		return;
	}
	else if ( cmd.compare( "debug" ) == 0 )	{
		//WindowsManager& wm = WindowsManager::getInstance();
		wm.setDebug( !wm.getDebug() );
		return;
	}
	pPC->affiche( new string("Exec command ..") );
}





class CallBack : public PanelConsoleCallBack	{
	public :
	virtual void					callback_cmd(std::string cmd)	{
		cout << "Class Callback : " << cmd << endl;
		cout << "  CallBack::callback_cmd( \""<< cmd <<"\" )"<< endl;
	};
};


static void CreateOneWindow()	{
	//WindowsManager& wm = WindowsManager::getInstance();
	string * pStr;
	PanelWindow  * PW;
	PanelSimple  * ps;
	PanelText* pt;
	
	char *texture[] = { (char*)"bouton-jaune.png", 
						(char*)"bouton-orange.png",
						(char*)"bouton-rouge.png",
						(char*)"bouton-vert.png", 
						(char*)"bouton-bleu.png", 
						(char*)"bouton-noir.png" };
	
	PW = new PanelWindow();
	pw.push_back( PW );
	
	int x, y, dx, dy;
	x =  10+rand()%1000;
	y =  10+rand()%450;
	dx = 200+rand()%400;
	dy = 200+rand()%200;
	dx = 220;
	dy = 6 * 42-10;

	PW->setPosAndSize( x, y, dx, dy );
	if ( pw.size()%3 == 0 )	PW->loadSkin( PanelWindow::RED );
	if ( pw.size()%3 == 1 )	PW->loadSkin( PanelWindow::BLACK );

	ps = new PanelSimple();
	ps->setPosAndSize( x, y, dx, dy );
	ps->setPosAndSize( 0, 0, dx, dy );
	PW->add( ps );


	char Buff[255];
	/*
	sprintf( Buff, "PanelWindow no %d", (int)pw.size() );
	PanelText* pT = new PanelText( Buff,	PanelText::NORMAL_FONT, 0, 32/2-10, 0xffFF0000 );
	pT->setAlign( PanelText::RIGHT );
	ps->add( pT );
	*/

	/********************************************************************************************
		Insertion des bouton
	*/
	PanelSimple* pButton;
	PanelText* pText;
	for ( int i=0; i<6		; i++ )	{
		pButton = new PanelSimple();
		pButton ->setPosAndSize( 0, 0+42*i, 220, 32 );
		pButton ->setBackground( (char*) texture[i%6] );

		sprintf( Buff, "New MENU %d", (int)i );
		
		cout << "----- premier -------------------" << endl;
		pText = new PanelText( Buff,	PanelText::NORMAL_FONT, 1, 32/2-10+1, 0xff000000);
		pText->setAlign( PanelText::CENTER );
		pButton->add( pText );
		
		cout << "----- deuxieme -------------------" << endl;
		pText = new PanelText( Buff,	PanelText::NORMAL_FONT, 0, 32/2-10, 0xFFffFFff );
		pText->setAlign( PanelText::CENTER );
		pButton->add( pText );
		
		

		ps->add( pButton );
	}	
	
	ps->setBackground( (char*) "images/background.tga" );
	//ps->setBackground( (_Texture2D*)NULL );

	wm.add(PW);
	delete pStr;
}

static void CreateAllWindows()	{
	//WindowsManager& wm = WindowsManager::getInstance();
	wm.setScreenSize( width-100, height-100 );

	PanelSimple  * ps;
	PanelText* pt;
	
	
	//---------------------------------------------------------------------------------	
	ps = new PanelSimple();
	ps->setBackground( (char*) "images/background.tga" );
	ps->setPosAndSize( 50+0, 50+50, 200, wm.getHeight()-100);
	int y = 10;
	int dy = 15;
	ps->add( new PanelText( (char*)"Fichier",		PanelText::LARGE_FONT, 5, y + 0*dy ) );
	ps->add( new PanelText( (char*)"Edition",		PanelText::UBUNTU_B, 5, y + 1*dy ) );
	ps->add( new PanelText( (char*)"Affichage",		PanelText::LARGE_FONT, 5, y + 2*dy ) );
	ps->add( new PanelText( (char*)"Recherche",		PanelText::LARGE_FONT, 5, y + 3*dy ) );
	ps->add( new PanelText( (char*)"Outils",		PanelText::LARGE_FONT, 5, y + 4*dy ) );
	ps->add( new PanelText( (char*)"Aide",			PanelText::LARGE_FONT, 5, y + 5*dy ) );
	
	wm.add( ps );
	//---------------------------------------------------------------------------------	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+wm.getWidth()-200, 50+50, 200, wm.getHeight()-100);
	ps->setBackground( (char*)"images/background.tga" );
	wm.add( ps );
	
	//---------------------------------------------------------------------------------	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+0, 50+wm.getHeight()-50-2, wm.getWidth(), 50+2);
	ps->setBackground( (char*)"images/background.tga" );
	wm.add( ps );

	PanelText* pT = new PanelText( (char*)"Press ESC to quit",	PanelText::NORMAL_FONT, 0, 20 );
	pT->setAlign( PanelText::CENTER );
	ps->add( pT );
	
	//---------------------------------------------------------------------------------	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+0, 100-(3*14+20)+2, wm.getWidth(), 3*14+20);
	ps->setBackground( (char*) "images/background.tga" );
	/*
	Induit une erreur de segmentation
	-----------------------------------
	pStr = new string("Press ESC to quit");
	ps->add( new PanelText( *pStr,	PanelText::NORMAL_FONT, 10, 5, 0xffffffff ) );
	pStr = new string("Or key to test console, enter word \"debug\" for debug mode on/off");
	ps->add( new PanelText( *pStr,	PanelText::NORMAL_FONT, 10, 20 ) );
	*/
	ps->add( new PanelText( (char*)"Press ESC to quit",	
							PanelText::NORMAL_FONT, 10, 10, 0xffffffff ) );
	ps->add( new PanelText( (char*)"Or key to test console, enter word \"debug\" for debug mode on/off",
							PanelText::NORMAL_FONT, 10, 24 ) );
	ps->add( new PanelText( (char*)"Touche TAB : agrandi la largeur, touche ² inverse",
							PanelText::NORMAL_FONT, 10, 38 ) );
	
	wm.add( ps );
	//---------------------------------------------------------------------------------	
	ps = new PanelSimple();
	ps->setBackground( (char*) "images/background.tga" );
	ps->setPosAndSize( 650-4, 50+50+0, 400, 600);

	ps->add( new PanelText( (char*)"1 Essai de panelText SMALL_FONT", PanelText::SMALL_FONT, 40, 100 ) );
	ps->add( new PanelText( (char*)"2 Essai de panelText NORMAL_FONT", PanelText::NORMAL_FONT, 60, 120 ) );
	ps->add( new PanelText( (char*)"3 Essai de panelText LARGE_FONT", PanelText::LARGE_FONT, 80, 140 ) );

	wm.add( ps );
	//---------------------------------------------------------------------------------	
	PanelConsole* pc;
	pc = new PanelConsole( 100, 10 );
	pc->setBackground( (char*) "images/background.tga" );
	pc->setPosAndSize( 250-2, 100, 400, wm.getHeight()-100);
	pc->setPrompt( "console1> " );
	pc->setCallBackCmd( exec_cmd );
	pPC = pc;
	
	wm.add( pc );
	//---------------------------------------------------------------------------------	
	pc = new PanelConsole( 100, 5 );
	pc->setPosAndSize( 250-2-4+800, 100, 200+8, wm.getHeight()-100);
	pc->setBackground( (char*) "images/background.tga" );
	//pc->setPrompt( "rene@poste-002:/home/rene$ " );
	pc->setPrompt( "console2> " );
	pc->setCallBackCmd( new CallBack() );
	wm.add( pc );
	/*
	*/
	for (int i=0; i<3; i++ )		CreateOneWindow();
	CreateOneWindow();

	wm.setScreenSize( width, height );
}














int main(int argc, 
	 char **argv)
{

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize( width, height );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-width)/2, (glutGet(GLUT_SCREEN_HEIGHT)-height)/2 );

	if (glutCreateWindow("Sample-01") == 0){ 
		return 1;
	}

	glutReshapeFunc(reshapeGL);
	glutDisplayFunc(displayGL);
	glutIdleFunc(idleGL);

	
	cout << "wxh " << width << "x" << height << endl;

	glutKeyboardFunc(glutKeyboardFunc);
	glutKeyboardUpFunc(glutKeyboardUpFunc);
	glutSpecialFunc(glutSpecialFunc);
	glutSpecialUpFunc(glutSpecialUpFunc);
	glutMouseFunc(glutMouseFunc);
	glutMotionFunc(glutMotionFunc);
	glutPassiveMotionFunc(glutPassiveMotionFunc);

	initGL(argc, argv);
    glewInit();

	CreateAllWindows();
	//WindowsManager::getInstance().swapVisible();

	/*
	std::cout<<"SIZE: "<< "size of char: " << sizeof (char) << std::endl;
	std::cout<<"SIZE: "<< "size of short: " << sizeof (short) << std::endl;
	std::cout<<"SIZE: "<< "size of int: " << sizeof (int) << std::endl;
	std::cout<<"SIZE: "<< "size of long: " << sizeof (long) << std::endl;
	std::cout<<"SIZE: "<< "size of long long: " << sizeof (long long) << std::endl;

	std::cout<<"SIZE: "<< "size of float: " << sizeof (float) << std::endl;
	std::cout<<"SIZE: "<< "size of double: " << sizeof (double) << std::endl;

	std::cout<<"SIZE: "<< "size of pointer: " << sizeof (int *) << std::endl;
	*/

	glutMainLoop();

	  return 0;
}


