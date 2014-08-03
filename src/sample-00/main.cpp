#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <string>


#include <WindowsManager.h>

using namespace std;




GLenum err;



//WindowsManager & wm = WindowsManager::getInstance();

int width  = 1500;
int height = 800;
GLdouble fov = 60.0;

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

	WindowsManager& wm = WindowsManager::getInstance();
	wm.setScreenSize( width, height );

	glViewport(0, 0, (GLint)newWidth, (GLint)newHeight);

	change_fov();
}



static void idleGL(void)
{
	WindowsManager::getInstance().idleGL();


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
	switch(key){ 
	case 27: quit(); break;
	default :
		WindowsManager::getInstance().swapVisible();
		std::cout << (int)key << std::endl;
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







static void CreateAllWindows()	{
	WindowsManager& wm = WindowsManager::getInstance();
	wm.setScreenSize( width-100, height-100 );

	PanelSimple  * ps;
	PanelText* pt;
	
	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+0, 50+50, 200, wm.getHeight()-100);
	wm.add( ps );
	int y = 100;
	int dy = 15;
	ps->add( new PanelText( "Fichier",		PanelText::LARGE_FONT, 5, y + 0*dy ) );
	ps->add( new PanelText( "Edition",		PanelText::LARGE_FONT, 5, y + 1*dy ) );
	ps->add( new PanelText( "Affichage",	PanelText::LARGE_FONT, 5, y + 2*dy ) );
	ps->add( new PanelText( "Recherche",	PanelText::LARGE_FONT, 5, y + 3*dy ) );
	ps->add( new PanelText( "Outils",		PanelText::LARGE_FONT, 5, y + 4*dy ) );
	ps->add( new PanelText( "Aide",			PanelText::LARGE_FONT, 5, y + 5*dy ) );
	
	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+wm.getWidth()-200, 50+50, 200, wm.getHeight()-100);
	wm.add( ps );
	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+0, 50+wm.getHeight()-50-2, wm.getWidth(), 50+2);
	wm.add( ps );
	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+0, 50+0, wm.getWidth(), 50+2);
	wm.add( ps );
	
	
	ps = new PanelSimple();
	ps->setPosAndSize( 50+200, 50+200, 400, 600);
	wm.add( ps );
	
	pt = new PanelText( "1 Essai de panelText", PanelText::SMALL_FONT );
	pt->setPos( 40, 100 );
	ps->add( pt );
	
	pt = new PanelText( "2 Essai de panelText", PanelText::SMALL_FONT );
	pt->setPos( 60, 120 );
	ps->add( pt );
	
	pt = new PanelText( "3 Essai de panelText", PanelText::SMALL_FONT );
	pt->setPos( 80, 140 );
	ps->add( pt );
	
	
	wm.setScreenSize( width, height );
}














int main(int argc, 
	 char **argv)
{

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize( width, height );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-width)/2, (glutGet(GLUT_SCREEN_HEIGHT)-height)/2 );

	if (glutCreateWindow("Sample-00") == 0){ 
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


