#include "PanelSpinEditText.h"
#include "wm.h"

#define ZONE_MORTE		20.0
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PanelSpinEditText::PanelSpinEditText()
{
	//string nameFile = "/usr/share/wmcgl/images/" + name;


    PanelEditText();
    this->setExtraString("SpinEditText");

    pCadran = new PanelSimple();
    pCadran->setExtraString("PanelSpinEditText:Cadran");
    pCadran->setBackground((char*)"/usr/share/wmcgl/images/cadran.tga");
    pCadran->setPosAndSize( -100+20, -100+8, 200, 200 );

    pBoule = new PanelSimple();
    pBoule->setBackground((char*)"/usr/share/wmcgl/images/boule.tga");
    pBoule->setPosAndSize( 55, 55, 20, 20 );

	PanelSimple* pEditScissor;
	pEditScissor = new PanelSimple();
    pEditScissor->setBackground((char*)"/usr/share/wmcgl/images/black.png");
	pEditScissor->setPos( 100-30, 100-8 );
	pEditScissor->setSize( 65, 20 );
	pEditCopy = new PanelEditText();
	pEditCopy->setPos( 100-20, 100-8 );
	pEditCopy->setPos( 1, 1 );
	pEditCopy->hideCursor();
	
 	WindowsManager&     wm  = WindowsManager::getInstance();
    pCadran->add( pBoule );
    pEditScissor->add( pEditCopy );
	pEditScissor->setScissor( true );
    pCadran->add( pEditScissor );
    wm.add( pCadran );
    

	pCadran->setVisible( false );
	pCadran->setScissor( false );

    delta_x = delta_y = nDecimal = 0;
    pVal = NULL;
    cb_motion = NULL;
    hideCursor();
    
    pChangeValue = NULL;
    /*
    pClick = new Panel();
    pClick->setPosAndSize(10,10,10,10);
    pClick->setPanelClickLeft(this);
    */
    //setVisible( false );
    //setExtraString( "PanelSpinEditText" );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_pVal( double*  p )
{
	pVal	= p;
	val		= *pVal;
	
    char s[50];
    
    switch( nDecimal )
    {
        case 0:
            sprintf( s, "%0.0f", val );
            break;
        case 1 :
            sprintf( s, "%0.1f", val );
            break;
        case 2:
            sprintf( s, "%0.2f", val );
            break;
        case 6:
            sprintf( s, "%0.6f", val );
            break;
        case 14:
            sprintf( s, "%0.14f", val );
            break;
        default:
            sprintf( s, "%0.0f", val );
            break;
    }
    //logf( (char*)"val=%0.2f max=%0.2f", val, max );
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_val( double  p )
{
	if ( pVal )		*pVal	= p;
	val		= p;
	
    char s[50];
    
    switch( nDecimal )
    {
        case 0:
            sprintf( s, "%0.0f", val );
            break;
        case 1 :
            sprintf( s, "%0.1f", val );
            break;
        case 2:
            sprintf( s, "%0.2f", val );
            break;
        case 6:
            sprintf( s, "%0.6f", val );
            break;
        case 14:
            sprintf( s, "%0.14f", val );
            break;
        default:
            sprintf( s, "%0.0f", val );
            break;
    }
    //logf( (char*)"val=%0.2f max=%0.2f", val, max );
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_enum( vector<double> t )
{
    t_val = t;
    nb = t.size();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::boule_pos( int xm, int ym )
{
    vec2 vm = vec2( xm, ym );
    vec2 v = vm - vCentre;
    v.y *= -1.0;
    double norme = v.length();
    
    angle = RAD2DEG( acos( v.y / norme ) );
    if ( v.x <0.0 )         angle = 360.0 - angle;
    
    vec2 vBoule = v / norme * 45.0 + vec2( 100.0-10.0, -100.0+10.0 );
    vBoule.y *= -1;
    pBoule->setPos( vBoule.x, vBoule.y );

}
//--------------------------------------------------------------------------------------------------------------------
// Calcul l'angle par rapport au centre
// et la distance
// Sauvegarde le vecteur position
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::computeRef( int xm, int ym )
{
    vec3 ptm	= vec3( xm, ym, 0.0 );
    vec3 ptc	= vec3( vCentre.x, vCentre.y, 0.0 );
    
   	vRef 		= ptm - ptc; 

    vRef.normalize();
}
//--------------------------------------------------------------------------------------------------------------------
// Calcul l'angle par rapport au centre
// et la distance
// Sauvegarde le vecteur position
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::computeAngle( int xm, int ym )
{
    vec3 ptm	= vec3( xm, ym, 0.0 );
    vec3 ptc	= vec3( vCentre.x, vCentre.y, 0.0 );
    vec3 v 		= ptm - ptc; 
    vec3 r;

    if ( v.length() <= ZONE_MORTE )		{
	    vRef = v;
    	vRef.normalize();
    	return;
	}
	
    boule_pos(xm, ym);
    
    v.normalize();
    r.cross( vRef, v );
    double norm = r.length();
    
    angle = RAD2DEG( asin(norm) );

    if ( r.z < 0.0 )    angle = -angle;

    vRef = v;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::compute_pos_relatif( int xm, int ym )
{
//#define DEBUG
	
	computeAngle( xm, ym );
	
    //--------------------------------------
    if ( t_val.size() == 0 )
    {
        val_angle = (step) / (nb*360.0) * angle;
#ifdef DEBUG
        logf( (char*)"compute_pos_relatif()  angle=%0.2f val_angle=%0.8f", angle, val_angle ); 
        logf( (char*)"                       min=%0.8f max=%0.8f", min, max ); 
#endif
        
        val += val_angle;
        clampVal();
    }    
    //--------------------------------------
    else
    {
#ifdef DEBUG
	    logf( (char*)"compute_pos_relatif()  angle=%0.2f", angle ); 
#endif
        val_angle += nb / 360.0 * angle;
        if ( val_angle <  0.0 )         val_angle = 0.0;
        if ( val_angle >= (nb-1 ))      val_angle = nb-1;
        int i = val_angle;
        val = t_val[i];
    }    
    //--------------------------------------
    //logf( (char*)"compute_pos_relatif()  angle=%0.2f norm=%0.2f val=%0.2f", val_angle, norm, val ); 
    if ( pVal!= NULL )          	*pVal = val;
    if ( pChangeValue )				pChangeValue->changeValueDouble( val, pID );
#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::ajusteDelta( int ws, int hs )
{
    int dx = pCadran->getDX();
    int dy = pCadran->getDY();
    int dx2 = dx/2;
    int dy2 = dy/2;

	delta_x = 0;
	delta_y = 0;
	
	int X=20, Y=8;
	
	//delta_x = delta_y = 0;
	
	if ( (x_raw+dx2) > ws )	{
		X += -dx2;
	}
	if ( (x_raw-dx2) < 0 )	{
		X += +dx2;
	}
	if ( (y_raw+dy2) > hs )	{
		Y += -dy2;
	}
	if ( (y_raw-dy2) < 0 )	{
		Y += +dy2;
	}
	
	delta_x += X;
	delta_y += Y;
	
	pCadran->setPos( getX() + delta_x - dx2, getY() + delta_y - dy2 );
	pCadran->updatePos();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::clampVal()
{
    if ( val > max )        val = max;
    if ( val < min )        val = min;
    if ( pVal != NULL )     *pVal = val;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::clickLeft( int xm, int ym )
{
    logf( (char*)"PanelSpinEditText::clickLeft(%d, %d)", xm, ym );

    log_tab(true);
    logf( (char*)"delta old (%d, %d)", delta_x, delta_y );

	// Affiche le cadran et efface le panel 
	setVisible( false );
    pCadran->setVisible( true );
    pCadran->setCanMove(false);

	// Calcul la position de du cadran
 	WindowsManager&     wm  = WindowsManager::getInstance();
	int ws = wm.getWidth();
	int hs = wm.getHeight();
    int dx2 = pCadran->getDX() / 2;
    int dy2 = pCadran->getDY() / 2;
    int xScreen = getX();
    int yScreen = getY();
    x_click = getPosX();
    y_click = getPosY();
    
    char s[20];
    strncpy( s, getText().c_str(), sizeof(s)-1 );
    pEditCopy->changeText( (char*)s );

    
    logf( (char*)"position (%d, %d)", xScreen, yScreen );
    pCadran->setPos(xScreen, yScreen);
    pCadran->updatePos();
    
	ajusteDelta( ws, hs );
    logf( (char*)"delta new (%d, %d)", delta_x, delta_y );
	
    logf( (char*)"pos_raw old (%d, %d)", pEditCopy->getX(), pEditCopy->getY() );
    
    logf( (char*)"cadran (%d, %d)  boule (%d,%d)", pCadran->getX(), pCadran->getY(), pBoule->getX(), pBoule->getY() );
    
    computeRef( xm, ym );
    clampVal();

    wm.onTop(pCadran);
    if ( click_left_cb != NULL )        (*click_left_cb)( xm, ym);
    

    log_tab(false);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::motionLeft( int xm, int ym )
{
    //logf( (char*)"PanelSpinEditText::motionLeft(%d, %d)", xm, ym );
    
    
    compute_pos_relatif( xm, ym );
    clampVal();
    
    char s[50];
    switch( nDecimal )
    {
        case 0:
            sprintf( s, "%0.0f", val );
            break;
        case 1 :
            sprintf( s, "%0.1f", val );
            break;
        case 2:
            sprintf( s, "%0.2f", val );
            break;
        case 6:
            sprintf( s, "%0.6f", val );
            break;
        case 14:
            sprintf( s, "%0.14f", val );
            break;
        default:
            sprintf( s, "%0.0f", val );
            break;
    }
    //logf( (char*)"val=%0.2f max=%0.2f", val, max );
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );

 	WindowsManager&     wm  = WindowsManager::getInstance();
    wm.onTop(pEditCopy);

	if ( cb_motion != NULL )			(*cb_motion)(xm, ym);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::releaseLeft( int xm, int ym )
{
    logf( (char*)"PanelSpinEditText::releaseLeft(%d, %d)", xm, ym );
    setVisible( true );
    pCadran->setVisible( false );
    pCadran->setCanMove(true);

    clampVal();
    
    if ( pVal!= NULL )          		*pVal = val;
    if ( pChangeValue )					pChangeValue->changeValueDouble( val, pID );
    if ( release_left_cb != NULL )		(*release_left_cb)( xm, ym);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::clickRight( int xm, int ym )
{
    logf( (char*)"PanelSpinEditText::clickRight(%d, %d)", xm, ym );
    logf( (char*)"	delta (%d, %d)", delta_x, delta_y );
    x_click = xm; y_click = ym;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::releaseRight( int xm, int ym )
{
    logf( (char*)"PanelSpinEditText::releaseRight(%d, %d)", xm, ym );
    
    logf( (char*)"	delta (%d, %d)", delta_x, delta_y );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::updatePos()
{
    //logf( (char*)"PanelSpinEditText::updatePos()" );
    PanelEditText::updatePos();

    int dx2 = pCadran->getDX() / 2;
    int dy2 = pCadran->getDY() / 2;

    vCentre = vec2( (double)(pCadran->getX() + dx2), (double)(pCadran->getY() + dy2) );
    //pClick->setPosAndSize( 0, 0, 40, 15 );
    
    Panel::updatePos();
    //vCentre = vec2( (double)(x_raw)+20, (double)(y_raw)+8 );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::idle(double f)
{
	PanelEditText::idle(f);
    //log( (char*)"PanelSpinEditText::idle()" ); 
/*
	long color;

    if ( var.getb("bNuit") )        color = 0xffff0000;
    else                            color = 0xffffffff;

    setColor( color );
    pCadran->setColor( color );
    pBoule->setColor( color );
    pEditCopy->setColor( color );
*/
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
//#define DBUG_CENTRE
void PanelSpinEditText::displayGL()
{
    PanelEditText::displayGL();

    if ( pEditCopy && pEditCopy->getVisible()==false )		return;

#ifdef DBUG_CENTRE
    //logf( (char*)"cadran (%d, %d)  boule (%d,%d)", pCadran->getX(), pCadran->getY(), pBoule->getX(), pBoule->getY() );


    glBegin(GL_LINES);
		//--------------------------------------------------------
		// graduation horizontale
		//--------------------------------------------------------
        vec4 color       = vec4( 0.5, 0.5, 0.9, 1.0 );    
		glColor4fv( (GLfloat*)&color );
		{
		    int x0 = vCentre.x-10;
		    int y0 = vCentre.y;
		    int x1 = vCentre.x+10;
		    int y1 = vCentre.y;

		    //xy2Screen(x0, y0);
		    //xy2Screen(x1, y1);
		    
		    glVertex2i( x0, y0 );
		    glVertex2i( x1, y1 );
		}
		//--------------------------------------------------------
		{
		    int x0 = vCentre.x;
		    int y0 = vCentre.y+10;
		    int x1 = vCentre.x;
		    int y1 = vCentre.y-10;

		    //xy2Screen(x0, y0);
		    //xy2Screen(x1, y1);
		    
		    glVertex2i( x0, y0 );
		    glVertex2i( x1, y1 );
		}
		//--------------------------------------------------------

	glEnd();
#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

