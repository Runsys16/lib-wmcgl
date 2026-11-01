#include "PanelSpinEditText.h"
#include "wm.h"
#include "macro.h"
//--------------------------------------------------------------------------------------------------------------------
//#define DEBUG
//------------------------------
#define ZONE_MORTE		20.0
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PanelSpinEditText::PanelSpinEditText() :     PanelEditText()
{
	//string nameFile = "/usr/share/wmcgl/images/" + name;



    this->setExtraString("SpinEditText");

    pCadran = new PanelSimple();
    pCadran->setExtraString("PanelSpinEditText:Cadran");
    pCadran->setBackground((char*)"/usr/share/wmcgl/images/cadran.tga");
    pCadran->setPosAndSize( -100+20, -100+8, 200, 200 );

    pBoule = new PanelSimple();
    pBoule->setExtraString("PanelSpinEditText:Boule");
    pBoule->setBackground((char*)"/usr/share/wmcgl/images/boule.tga");
    pBoule->setPosAndSize( 55, 55, 20, 20 );

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
    setVisible( true );
    
	pUnite = NULL;
    //setChangeValue((ChangeValue*)NULL);
    //setID((void*)NULL);
    pChangeValue	= NULL;
    pID				= NULL;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PanelSpinEditText::~PanelSpinEditText()
{
	//string nameFile = "/usr/share/wmcgl/images/" + name;

 	WindowsManager&     wm  = WindowsManager::getInstance();

    pEditScissor->sup( pEditCopy );
    pCadran->sup( pBoule );
    pCadran->sup( pEditScissor );
    
    wm.sup( pCadran );

    delete pCadran;
    delete pBoule;
	delete pEditScissor;
	delete pEditCopy;
	

    //setChangeValue((ChangeValue*)NULL);
    //setID((void*)NULL);
    //pChangeValue	= NULL;
    //pID				= NULL;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_pVal( double*  p )
{
	pVal	= p;
	val		= *pVal;
	
    char s[50];
    char u[50];
    
	if ( pUnite != NULL )		snprintf( u, sizeof(u), "%s", pUnite );
	else						u[0] = 0;
    
    switch( nDecimal )
    {
        case 0:
            snprintf( s, sizeof(s), "%0.0lf %s", val, u );
            break;
        case 1 :
            snprintf( s, sizeof(s), "%0.1f", val );
            break;
        case 2:
            snprintf( s, sizeof(s), "%0.2f", val );
            break;
        case 6:
            snprintf( s, sizeof(s), "%0.6f", val );
            break;
        case 14:
            snprintf( s, sizeof(s), "%0.14f", val );
            break;
        default:
            snprintf( s, sizeof(s),"%0.0f", val );
            break;
    }
    //logf_wm( (char*)"val=%0.2f max=%0.2f", val, max );
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_val( double  d )
{
	if ( pVal )		*pVal	= d;
	val		= d;
	
    char s[50];
    char u[50];
    
	if ( pUnite != NULL )		snprintf( u, sizeof(u), "%s", pUnite );
	else						u[0] = 0;
    
    switch( nDecimal )
    {
        case 0:
            sprintf( s, "%0.0f %s", val, u );
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
    
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::set_unite( char* pS )
{
    //logf_wm( (char*)"PanelSpinEditText::set_unite( %s )", pS );
	pUnite = pS;
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
void PanelSpinEditText::setChangeValue(ChangeValue* p)
{
    //logf_wm( (char*)"PanelSpinEditText::setChangeValue(ChangeValue* %p)", (void*)p );
    
	pChangeValue = p;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::setID( void* p)
{
    //logf_wm( (char*)"PanelSpinEditText::setID(void* %p)", (void*)p );
	pID = p;
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
        logf_wm( (char*)"compute_pos_relatif()  angle=%0.2f val_angle=%0.8f", angle, val_angle ); 
        logf_wm( (char*)"                       min=%0.8f max=%0.8f", min, max ); 
#endif
        
        val += val_angle;
        clampVal();
    }    
    //--------------------------------------
    else
    {
#ifdef DEBUG
	    logf_wm( (char*)"compute_pos_relatif()  angle=%0.2f", angle ); 
#endif
        val_angle += nb / 360.0 * angle;
        if ( val_angle <  0.0 )         val_angle = 0.0;
        if ( val_angle >= (nb-1 ))      val_angle = nb-1;
        int i = val_angle;
        val = t_val[i];
    }    
    //--------------------------------------
    //logf_wm( (char*)"compute_pos_relatif()  angle=%0.2f norm=%0.2f val=%0.2f", val_angle, norm, val ); 
    if ( pVal!= NULL )          	*pVal = val;
    if ( pChangeValue )				pChangeValue->changeValueDouble( val, pID );
//#undef DEBUG
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
#ifdef DEBUG
    //logf_wm( (char*)"PanelSpinEditText::clickLeft(%d, %d)", xm, ym );
#endif

#ifdef DEBUG
    log_tab(true);
    logf_wm( (char*)"delta old (%d, %d)", delta_x, delta_y );
#endif

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

    
#ifdef DEBUG
    logf_wm( (char*)"position (%d, %d)", xScreen, yScreen );
#endif

    pCadran->setPos(xScreen, yScreen);
    pCadran->updatePos();
    
	ajusteDelta( ws, hs );

#ifdef DEBUG
    logf_wm( (char*)"delta new (%d, %d)", delta_x, delta_y );
    logf_wm( (char*)"pos_raw old (%d, %d)", pEditCopy->getX(), pEditCopy->getY() );
	logf_wm( (char*)"cadran (%d, %d)  boule (%d,%d)", pCadran->getX(), pCadran->getY(), pBoule->getX(), pBoule->getY() );
#endif
    
    computeRef( xm, ym );
    clampVal();

    wm.onTop(pCadran);
    if ( click_left_cb != NULL )        (*click_left_cb)( xm, ym);
    

#ifdef DEBUG
    log_tab(false);
#endif
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::motionLeft( int xm, int ym )
{
#ifdef DEBUG
    logf_wm( (char*)"PanelSpinEditText::motionLeft(%d, %d)", xm, ym );
#endif
    
    
    compute_pos_relatif( xm, ym );
    clampVal();
    
    char s[50];
    char u[50];
    
	if ( pUnite != NULL )	snprintf( u, sizeof(u), "%s", pUnite );
	else					u[0] = 0;

    switch( nDecimal )
    {
        case 0:
            sprintf( s, "%0.0f %s", val, u );
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
    //logf_wm( (char*)"val=%0.2f max=%0.2f", val, max );
    changeText( (char*)s );
    pEditCopy->changeText( (char*)s );

 	WindowsManager&     wm  = WindowsManager::getInstance();
    wm.onTop(pEditCopy);

    //logf_wm( (char*)"PanelSpinEditText::motionLeft()   %p->%p", (void *)pChangeValue, (void*)pID );
	if ( cb_motion != NULL )			(*cb_motion)(xm, ym);
#ifdef DEBUG
	else								logf_wm( (char*)"cb_motion == NULL" );
#endif

    if ( pChangeValue )					pChangeValue->changeValueDouble( val, pID );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::releaseLeft( int xm, int ym )
{
	#ifdef DEBUG
		logf_wm( (char*)"PanelSpinEditText::releaseLeft(%d, %d)", xm, ym );
	#endif
    setVisible( true );

    pCadran->setVisible( false );
    pCadran->setCanMove(true);

    clampVal();
    
    if ( pVal!= NULL )          		*pVal = val;
    if ( pChangeValue )					pChangeValue->changeValueDouble( val, pID );
    if ( pChangeValue )					pChangeValue->changeValueDoubleClick( val, pID );
    if ( release_left_cb != NULL )		(*release_left_cb)( xm, ym);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::clickRight( int xm, int ym )
{
#ifdef DEBUG
    logf_wm( (char*)"PanelSpinEditText::clickRight(%d, %d)", xm, ym );
    logf_wm( (char*)"	val ( %d)", val );
#endif
    x_click = xm; y_click = ym;
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::releaseRight( int xm, int ym )
{
#ifdef DEBUG
    logf_wm( (char*)"PanelSpinEditText::releaseRight(%d, %d)", xm, ym );
    logf_wm( (char*)"	val ( %d)", (int)val );
#endif
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::wheelUp( int xm, int ym )
{
	#ifdef DEBUG
		logf_wm( (char*)"PanelSpinEditText::wheelUp(%d, %d)", xm, ym );
		logf_wm( (char*)"	val ( %d)", val );
	#endif

	val += 1.0;
	if ( val > max )			val = max;
    clampVal();
	set_val( val );

    if ( pChangeValue )			pChangeValue->changeValueDouble( val, pID );
    if ( pChangeValue )			pChangeValue->changeValueDoubleClick( val, pID );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::wheelDown( int xm, int ym )
{
	#ifdef DEBUG
		logf_wm( (char*)"PanelSpinEditText::releaseRight(%d, %d)", xm, ym );
		logf_wm( (char*)"	delta (%d, %d)", delta_x, delta_y );
	#endif

	val -= 1.0;
    clampVal();
	set_val( val );

	if ( val < min )			val = min;
    if ( pChangeValue )			pChangeValue->changeValueDoubleClick( val, pID );
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Panel* PanelSpinEditText::isMouseOver(int xm, int ym)
{
//#define DEBUG
	//if ( !isVisible() || bFantome )		return NULL;

	
	#ifdef DEBUG
		logf_wm( (char*)"PanelSpinEditText::MouseOver '%s' = %d,%d %dx%d  visible %s",
		             sExtra.c_str(), x, y, dx_raw, dy_raw, BOOL2STR(getVisible()) );
	#endif

	#ifdef DEBUG
		if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		
			{ logf_wm( (char*)"Mouse over '%s'", sExtra.c_str() ); return this; }
	#endif

	if ( x_raw <= xm && xm <= (x_raw+dx_raw) && y_raw <= ym && ym <= (y_raw+dy_raw) )		return this;
	else																					return NULL;

//#undef DEBUG
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::updatePos()
{
	#ifdef DEBUG
		logf_wm( (char*)"PanelSpinEditText::updatePos()" );
	#endif
	
    PanelEditText::updatePos();

    int dx2 = pCadran->getDX() / 2;
    int dy2 = pCadran->getDY() / 2;

    vCentre = vec2( (double)(pCadran->getX() + dx2), (double)(pCadran->getY() + dy2) );
    
    Panel::updatePos();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PanelSpinEditText::idle(double f)
{
	PanelEditText::idle(f);
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
    //logf_wm( (char*)"cadran (%d, %d)  boule (%d,%d)", pCadran->getX(), pCadran->getY(), pBoule->getX(), pBoule->getY() );


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
		    
		    glVertex2i( x0, y0 );
		    glVertex2i( x1, y1 );
		}
		//--------------------------------------------------------
		{
		    int x0 = vCentre.x;
		    int y0 = vCentre.y+10;
		    int x1 = vCentre.x;
		    int y1 = vCentre.y-10;
		    
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

