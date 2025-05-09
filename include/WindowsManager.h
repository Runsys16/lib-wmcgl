#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include <vector>
#include <string>
#include <map>


class _Texture2D;
class Panel;
class _Font;
class TextUtil;
class TrueType;

#define color32 unsigned int
struct CTexFont
{
    unsigned char * m_TexBytes;
	unsigned int	m_TexID;
    int             m_TexWidth;     // power of 2
    int             m_TexHeight;    // power of 2
    float           m_CharU0[256];
    float           m_CharV0[256];
    float           m_CharU1[256];
    float           m_CharV1[256];
    int             m_CharWidth[256];
    int             m_CharHeight;
    int             m_NbCharRead;

    CTexFont();
    ~CTexFont();
};


CTexFont *TwGenerateFont(const unsigned char *_Bitmap, int _BmWidth, int _BmHeight);



extern CTexFont *DefaultSmallFont;
extern CTexFont *DefaultNormalFont;
extern CTexFont *DefaultLargeFont;

void wmcglGenerateDefaultFonts();
void wmcglDeleteDefaultFonts();




// -------------------------------
// Textures abstraites
// -------------------------------

class _Texture
{
public:
	virtual GLenum getTextureType() const = 0;

	void Gen();
	virtual bool Load(const std::string& name);
	void Destroy();

	void Bind(GLuint slot) const;
	void Unbind(GLuint slot) const;

	GLuint getHandle() const {return m_nHandle;} 

	static void EnableGenerateMipmaps(bool b) {s_bGenerateMipmaps=b;}
	void Bind() const;
	void Unbind() const;

	_Texture() {m_nHandle=0; /*std::cout << "_Texture::_Texture()" << std::endl;*/}
	~_Texture() {Destroy();}
	
	

//static int			getNbTextures()					{ return nb; }
	
	unsigned	dx;
	unsigned	dy;
	

protected:
	//void Bind() const;
	//void Unbind() const;
	bool LoadFile(GLenum target, const std::string& name);
	void LoadData(GLenum target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d);
	
protected:
		GLuint		m_nHandle;				// ID de la texture
											// Etat pour le chargement : generation ou non des mipmaps
static	bool 		s_bGenerateMipmaps;		
static	int			nb;
};





class FrameBufferObject;

// -------------------------------
// Textures 2D
// -------------------------------

class _Texture2D : public _Texture
{
public:
	virtual GLenum getTextureType() const {return GL_TEXTURE_2D;}
	bool Load(const std::string& name);
	bool Load(GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d);
static int			getNbTextures()					{ return nb; }

	_Texture2D() : _Texture() {}

protected:
	
};


//  ---------------------------------------------------------------------------
//
//  @file       TwOpenGL.h
//  @brief      OpenGL graph functions
//  @author     Philippe Decaudin - http://www.antisphere.com
//  @license    This file is part of the AntTweakBar library.
//              For conditions of distribution and use, see License.txt
//
//  notes:      Private header
//              TAB=4
//
//  ---------------------------------------------------------------------------






//  ---------------------------------------------------------------------------



const color32 COLOR32_BLACK     = 0xff000000;   // Black 
const color32 COLOR32_WHITE     = 0xffffffff;   // White 
const color32 COLOR32_ZERO      = 0x00000000;   // Zero 
const color32 COLOR32_RED       = 0xffff0000;   // Red 
const color32 COLOR32_GREEN     = 0xff00ff00;   // Green 
const color32 COLOR32_BLUE      = 0xff0000ff;   // Blue 
   


class TextUtil  {

public:
    TextUtil();

    void *				NewTextObj();
    void				DeleteTextObj(void *_TextObj);
    void				BuildText(void *_TextObj, const std::string *_TextLines, color32 *_LineColors, color32 *_LineBgColors, int _NbLines,  CTexFont *_Font, int _Sep, int _BgWidth);

    int					lenght(void *_TextObj, const std::string *_TextLines, CTexFont * );
    int					lenght(void *_TextObj, const std::string *_TextLines, CTexFont *, int );
    void				BeginGL();
    void				EndGL();
    void				DrawText(void *_TextObj, float angle, int _X, int _Y, color32 _Color, color32 _BgColor);
    void				DrawText(void *_TextObj, int _X, int _Y, color32 _Color, color32 _BgColor);

	void				ChangeViewport(int, int, int, int, int, int);

	GLuint 				GenFont( CTexFont * );
	GLuint 				GenFont( CTexFont *, color32 );
	void 				BindFont( const CTexFont * );
	void 				BindFont( const CTexFont *, int );
	void 				UnbindFont( int );
	void 				UnbindFont();
	void				setTabSize( int );



protected:
    bool                m_Drawing;
    GLuint              m_FontTexID;
    const CTexFont *    m_FontTex;
    GLfloat             m_PrevLineWidth;
    GLint               m_PrevTexEnv;
    GLint               m_PrevPolygonMode[2];
    GLint               m_MaxClipPlanes;
    GLint               m_PrevTexture;
    GLint               m_PrevArrayBufferARB;
    GLint               m_PrevElementArrayBufferARB;
    GLboolean           m_PrevVertexProgramARB;
    GLboolean           m_PrevFragmentProgramARB;
    GLuint              m_PrevProgramObjectARB;
    GLboolean           m_PrevTexture3D;
    enum EMaxTextures   { MAX_TEXTURES = 128 };
    GLboolean           m_PrevActiveTexture1D[MAX_TEXTURES];
    GLboolean           m_PrevActiveTexture2D[MAX_TEXTURES];
    GLboolean           m_PrevActiveTexture3D[MAX_TEXTURES];
    GLboolean           m_PrevClientTexCoordArray[MAX_TEXTURES];
    GLint               m_PrevActiveTextureARB;
    GLint               m_PrevClientActiveTextureARB;
    bool                m_SupportTexRect;
    GLboolean           m_PrevTexRectARB;
    GLint               m_PrevBlendEquation;
    GLint               m_PrevBlendEquationRGB;
    GLint               m_PrevBlendEquationAlpha;
    GLint               m_PrevBlendSrcRGB;
    GLint               m_PrevBlendDstRGB;
    GLint               m_PrevBlendSrcAlpha;
    GLint               m_PrevBlendDstAlpha;
    GLint               m_ViewportInit[4];
    GLfloat             m_ProjMatrixInit[16];
    int                 m_WndWidth;
    int                 m_WndHeight;
    int                 m_tab_size;

    struct Vec2         { GLfloat x, y; Vec2(){} Vec2(GLfloat _X, GLfloat _Y):x(_X),y(_Y){} Vec2(int _X, int _Y):x(GLfloat(_X)),y(GLfloat(_Y)){} };
    struct CTextObj
    {
        std::vector<Vec2>   m_TextVerts;
        std::vector<Vec2>   m_TextUVs;
        std::vector<Vec2>   m_BgVerts;
        std::vector<color32>m_Colors;
        std::vector<color32>m_BgColors;
    };
};

//  ---------------------------------------------------------------------------





using namespace std;


typedef void (*click_left_cb_t)(int,int);
typedef void (*release_left_cb_t)(int,int);
typedef void (*click_right_cb_t)(int,int);
typedef void (*release_right_cb_t)(int,int);

typedef void (*displayGL_cb_t)(void);

class Panel {
	public:
		//-----------------------------------------------------------------------------------
							Panel();
		//-----------------------------------------------------------------------------------
		void 				init();


		void				add( Panel* );
		void				sup( Panel* );

		void				onTop();
		void				onBottom();

		virtual void		passiveMotionFunc( int, int)			      {;};
		
		virtual void		clickLeft( int, int);
		virtual void		motionLeft( int, int)                         {;};
		virtual void		releaseLeft( int, int);

		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);

		virtual void		clickRight( int, int);
		virtual void		motionRight( int, int)                         {;};
		virtual void		releaseRight( int, int);

		virtual void		clickMiddle( int, int)                          {;};
		virtual void		motionMiddle( int, int)                         {;};
		virtual void		releaseMiddle( int, int)                        {;};

		virtual void		wheelUp( int, int)                              {;};
		virtual void		wheelDown( int, int)                            {;};

		virtual Panel*		isMouseOverRaw( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual Panel*		isMouseOverBorder( int, int);

		virtual void		haveFocus() 	    							{ bFocus = true;}
		virtual void		lostFocus() 									{ bFocus = false;}

		virtual void		haveCapture()	    							{;}
		virtual void		lostCapture()									{;}

		virtual void		displayGL();
		virtual void		updatePos();
		virtual void		idle(float);//										{;};
		virtual void		cb_keyboard( unsigned char ) 					{;};
		virtual void		cb_keyboard_special( unsigned char )			{;};
		virtual void		cb_keyboard_special_up( unsigned char )			{;};
		virtual void		debug( bool )									{;};
		virtual bool		isVisible(); 									
		
		inline void			setParent( Panel* p )							{parent = p;};
		inline Panel*		getParent()										{return parent;};
		inline void			setPosAndSize(int x0, int y0, int dx0, int dy0)	{x=x0; y=y0; dx=dx0 ;dy=dy0;};
		inline void			setPos(int x0, int y0)							{x=x0; y=y0;};
		inline void			setSize(int dx0, int dy0)						{dx=dx0 ;dy=dy0;};
		inline int			getX()											{return x_raw;};
		inline int			getY()											{return y_raw;};
		inline int			getDX()											{return dx_raw;};
		inline int			getDY()											{return dy_raw;};
		inline void			setID(int id)									{ID = id;};
		inline int			getID()											{return ID;};
		
		inline void			setPosX(int i)									{x = i;};
		inline void			setPosY(int i)									{y = i;};
		inline void			setPosDX(int i)									{dx = i;};
		inline void			setPosDY(int i)									{dy = i;};
		inline int			getPosX()										{return x;};
		inline int			getPosY()										{return y;};
		inline int			getPosDX()										{return dx;};
		inline int			getPosDY()										{return dy;};
		inline int			getNbPanel()									{return childs.size();}
	
		inline bool			getVisible()									{return visible;};
		inline void			swapVisible()									{ visible = !visible; };
		inline void			setVisible(bool b)								{visible=b;};
		
		inline bool			getCanMove()									{return bCanMove;};
		inline void			setCanMove(bool b)								{bCanMove=b;};
		

		inline void			setDisplayGL(displayGL_cb_t cb)                 {displayGL_cb=cb;};

		inline void			x2Screen(int & x)                               {x += x_raw;};
		inline void			y2Screen(int & y)                               {y += y_raw;};
		
		inline void			xy2Screen(int& x, int& y)                       {x += x_raw;y += y_raw;}
		
		inline int			Screen2x(int x)                                 {return x - x_raw;};
		inline int			Screen2y(int y)                                 {return y - y_raw;};
		
		inline 	std::vector<Panel*>& getChilds()							{return childs;};
		
		inline void			haveMove()                                      { bHaveMove = true;}
		inline bool			getHaveMove()                                   { return bHaveMove;}
		inline void			resetHaveMove()                                 { bHaveMove = false;}
		
		inline void			setClickLeft( click_left_cb_t cb)               { click_left_cb = cb;}
		inline void			setReleaseLeft( click_left_cb_t cb)             { release_left_cb = cb;}
		inline void			setClickRight( click_left_cb_t cb)              { click_right_cb = cb;}
		inline void			setReleaseRight( click_left_cb_t cb)            { release_right_cb = cb;}
		
		inline void			setPanelClickLeft( Panel* p )                   { panel_click_left   = p;}
		inline void			setPanelReleaseLeft( Panel* p )                 { panel_release_left = p;}
		inline void			setPanelClickRight( Panel* p  )                 { panel_click_right  = p;}
		inline void			setPanelReleaseRight( Panel* p )                { panel_release_right= p;}
		
		inline void			setScissor(bool b)                              { bScissor = b;}
		inline void			setFantome(bool b)                              { bFantome = b;}
		
		inline int 			getMouseOverBorder()                            { return mouseOverBorder;}

    	inline  void        setExtraString(string s)                        { sExtra = s; }
    	inline  string&     getExtraString()                                { return sExtra; }
    	inline  void        setParentCliping(bool b)                        { bParentCliping = b; }
	

		void				deleteChilds();
		

		//-----------------------------------------------------------------------------------
	protected:
		//-----------------------------------------------------------------------------------
		int					ID;
		int					TYPE;
		int					x_raw;
		int					y_raw;
		int					dx_raw;
		int					dy_raw;
	
		int					x;
		int					y;
		int					dx;
		int					dy;
	
		int					startX;
		int					startY;
	
		bool				visible;
		bool				bParentCliping;
		bool				bCanMove;
		bool				mouseVisible;
		int                 mouseOverBorder;
	
		Panel* 				parent;
		std::vector<Panel*> childs;
		
		bool				bDebug;
		
		displayGL_cb_t      displayGL_cb;
		click_left_cb_t     click_left_cb;
		release_left_cb_t   release_left_cb;
		click_right_cb_t    click_right_cb;
		release_right_cb_t  release_right_cb;
		
		bool                bHaveMove;
		bool                bScissor;
		bool                bFantome;
		bool                bFocus;
		bool                bCapture;

    	string              sExtra;

        Panel*              panel_click_left;
        Panel*              panel_release_left;
        Panel*              panel_click_right;
        Panel*              panel_release_right;

};








class PanelText : public Panel	{

	public:
		enum FONT { 	ARIAL, UBUNTU_B, UBUNTU_RI, UBUNTU_R, DEJA_VU_SANS_MONO,\
					 	NORMAL_FONT, SMALL_FONT, LARGE_FONT, \
					 	FREE_TYPE };
		enum ALIGN { LEFT, RIGHT, CENTER };


							~PanelText();
							PanelText();
							PanelText( std::string );
							PanelText( std::string, FONT );
							PanelText( std::string, FONT, int, int );
							PanelText( std::string, FONT, int, int, unsigned int );
							PanelText( char*, FONT, int, int );
							PanelText( char*, FONT, int, int, unsigned int );
							PanelText( char*, char*, int, int, uint32_t, uint32_t );
							PanelText( char*, char*, int, int, uint32_t, uint32_t, bool );

		void 				init();
		void 				buildString();
		
		inline std::string	getText()										{return text;};
		int					getTextLenght();
		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, bool );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		int					getTabSize()									{return tabSize;}
		void				setColor( int l);
inline	void				setChangeColor(bool b)							{ bColor = b; }
	
		virtual Panel*		isMouseOver( int, int);
		void				displayGLfreetype();
		void				displayGLInternal();
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		inline void 		setChangeText(bool b )							{bChange = b;};
		inline void 		setAlpha(float a )								{ alpha = a;};
		
		//----------------- functions
		std::string			strFont();		

	protected:
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		TrueType*			pTrueType;
		std::string			sFreeTypeName;
		std::string			text;
		uint32_t            color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		bool                bStatic;
		bool				bColor;
		
		float				alpha;
	
};







class PanelTextOmbre : public PanelText	{

	public:

							PanelTextOmbre();
							PanelTextOmbre( std::string );
							PanelTextOmbre( std::string, FONT );
							PanelTextOmbre( std::string, FONT, int, int );
							PanelTextOmbre( std::string, FONT, int, int, unsigned int );
							PanelTextOmbre( char*, FONT, int, int );
							PanelTextOmbre( char*, FONT, int, int, unsigned int );
							
		void				init();

		void 				buildString();
		
		inline std::string	getText()										{return text;};
//		int					getTextLenght();
//		int					getTextLenght(int);

		void 				changeText( char* );
		void 				changeText( char*, FONT );
		void 				changeText( std::string );
		void 				changeText( std::string, FONT );
		void 				changeText( std::string, bool );
		void 				changeText( std::string, FONT, bool );
		void 				eraseText( );
		void				setTabSize( int );
		void				setColor( unsigned long l);
		
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		
		inline void 		setAlign( ALIGN a )								{align = a;};
		
		//----------------- functions
//		std::string			strFont();		
		void				displayGLInternal();

	/*
		//----------------- members
		ALIGN				align;

		TextUtil*			textUtil;
		FONT				typeFont;
		std::string			text;
		unsigned int		color;
		//std::string			cmdLine;
		bool				bChange;
		
		void*				pTextGL;
		int					tabSize;
		*/
	protected:
		PanelText			pTextOmbre;
};








class PanelSimple : public Panel {
	public:
		PanelSimple();
		
		//void				buildText();

		void                setColor(long);

		virtual void		displayGL();
		virtual void		updatePos();
		virtual void        debug(bool);
		
		void				setBackground( char *);
		void				setBackground( GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d );
		void				deleteBackground();

inline	void                setBackground( _Texture2D* p )          { m_pTexBackground = p; }
inline _Texture2D*          getBackground()                         { return m_pTexBackground; }
inline 	unsigned int        getColor()                              { return color; }
inline	void                setColorBgr(unsigned int u)				{ colorBgr = u; }
		


//	private:
	protected:
		_Texture2D*		                m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
		
		unsigned int                    color;
		unsigned int                    colorBgr;
};







typedef void (* CB_CMD)(std::string);

class PanelEditTextCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};


class PanelEditText : public PanelSimple	{
	public:

									PanelEditText();
									//PanelEditText( int, int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelEditTextCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		void						addCmd(std::string cmd);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						moveCursor();
		void						addChar( char );
		void						supChar();
		void						delChar();
		void						supWord();
		void						delWord();
		
		void						incCursor();
		void						decCursor();
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		void                        setColor(long);

		inline std::string	        getText()					{ return text->getText(); };
		inline void      	        changeText(std::string s)   { text->changeText(s, PanelText::NORMAL_FONT, true ); };
		inline void					hideCursor()				{ bHideCursor = true; }
		inline void					showCursor()				{ bHideCursor = false; }
		
	private:
		PanelText *                 text;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelEditTextCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		//int 						currentLine;
		int							currentPos;
		//int 						currentCmd;
		//std::string					svgCmd;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		bool						bHideCursor;
		long                        color;
};






typedef void (*motion_cb_t)(int,int);


class ChangeValue
{
public:
	
        virtual void changeValueDouble( double val, void *p )			{};
};

class PanelSpinEditText : public PanelEditText
{
protected:
            double       min;
            double       max;
            double       step;
            double       nb;
            double       val_angle;
            double       val;
            double       angle;
            
            double*      pVal;
            
            int         delta_x;
            int         delta_y;
            int         nDecimal;
            
            vec2        vCentre;
            vec3        vRef;
            
       PanelEditText* 	pEditCopy;
       PanelSimple*     pCadran;
       PanelSimple*     pBoule;
       Panel*			pPrevParent;
       ChangeValue*     pChangeValue;
       //Panel*			pClick;
       
            vector<double>   t_val;
            int         x_click;
            int         y_click;
            motion_cb_t cb_motion;

			void*		pID;            
public:
                        PanelSpinEditText();
                        
			void        set_pVal(double*);
inline      void        set_delta(int x, int y)     	{ delta_x = 0; delta_y = 0; }                        
			void        set_val(double f);//            { val_angle = val = f; }                        
inline      void        set_min(double f)           	{ min = f; }                        
inline      void        set_max(double f)               { max = f; }                        
inline      void        set_step(double f)              { step = f; }                        
inline      void        set_nb(double f)                { nb = f; }                        
inline      void        set_ndecimal(int n)             { nDecimal = n; }                        
inline      void        set(double m, double M, double s, double n)
							                            { min = m; max = M; step = s; nb = n; }     

inline 		void		setMotion( motion_cb_t cb)      { cb_motion = cb; }
inline      double*     get_pVal()          		    { return pVal; }                        
inline      double		get_val()	          		    { return val; }
inline      void        setChangeValue(ChangeValue* p)	{ pChangeValue = p; }
inline      void        setID( void* p)					{ pID = p; }


            void        set_enum(vector<double>);
                        
            void        boule_pos(int, int);        

			void		computeRef( int, int );
			void		computeAngle( int, int );
            void        compute_pos_relatif(int, int);                   
                        
			void		ajusteDelta( int, int );
            void        clampVal();
            
	virtual void		clickLeft( int, int );
	virtual void		motionLeft( int, int );
	virtual void		releaseLeft( int, int );

	virtual void		clickRight( int, int );
	virtual void		releaseRight( int, int );

    virtual void		updatePos();
    virtual void		idle(double);
    virtual void		displayGL();

};





/*
typedef void (* CB_CMD)(std::string);

class PanelScrollTextCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};
*/

class PanelScrollText : public PanelSimple	{
	public:

									PanelScrollText();
									PanelScrollText( int, int );
		
		void						setPrompt( std::string );
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
		void						addChar( char );
		
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		void						setColor( long );
		
		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);
		
inline  int                         getHeightLine()                     { return heightLine; }

	private:
		int							maxCmd;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		
		int							heightLine;
		int							tabSize;
		
		int                         color;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		
		int                         oldDY;
		
};





class PanelScrollY : public PanelSimple {
	public:
		PanelScrollY();
		
		int 				computeDY();
		
		inline void			setDelta( int d )				{ y_delta = d; }
		//void				buildText();
		virtual void		clickUp( int, int);
		virtual void		clickDown( int, int);
		virtual Panel*		isMouseOver( int, int);


		virtual void		wheelUp( int, int);
		virtual void		wheelDown( int, int);

		virtual void		displayGL();
		virtual void		updatePos();

//	private:
	protected:
		int					dy_scroll;
		int					y_scroll;
		int					y_delta;
		
};







typedef void (* CB_CMD)(std::string);

class PanelConsoleCallBack	{
	public :
	virtual void					callback_cmd(std::string)=0;
};


class PanelConsole : public PanelSimple	{
	public:

									PanelConsole();
									PanelConsole( int, int );
		
		void						setPrompt( std::string );
		void						setCallBackCmd( CB_CMD );
		void						setCallBackCmd( PanelConsoleCallBack * );

		virtual void				displayGL();
		virtual void				updatePos();
		virtual void				idle(float);

		void						addCmd(std::string cmd);
		
		virtual void				cb_keyboard( unsigned char );
		virtual void				cb_keyboard_special( unsigned char );
		virtual void				cb_keyboard_special_up( unsigned char );
		
		void						moveCursor();
		void						scroll();
		void						rotateBuffer();
		void						addLine();
		void						eraseTexts();
		void						clear();
		void						affiche( std::string * );
		void						affiche( std::string );
		void						addChar( char );
		void						supChar();
		void						delChar();
		void						supWord();
		void						delWord();
		
		void						incCursor();
		void						decCursor();
		int							posWordPrec();
		int							posWordSuiv();
		void						wordPrec();
		void						wordSuiv();
		void						setTabSize( int );
		void                        setColor(long);
		
	private:
		int							maxCmd;
		std::vector<std::string*> 	cmds;
		std::vector<PanelText *>	texts;
		std::string					prompt;
		PanelText					cursor;
		CB_CMD						cb_cmd;
		PanelConsoleCallBack*		ppccb;
		
		int							heightLine;
		int							tabSize;
		
		int 						currentLine;
		int							currentPos;
		int 						currentCmd;
		std::string					svgCmd;
		
		float						cursorTime;
		bool						bIns;
		bool						bRightCtrl;
		bool						bLeftCtrl;
		bool						bCtrl;
		long                        color;
};






class PanelButton;

typedef void (* CALLBACK_MOUSE)(PanelButton*);
//typedef void (* CALLBACK_DOWN)(PanelButton*);

class PanelButtonCallBack	{
	public :
	virtual void					cb_button_mouse_over(PanelButton*)				{;};
	virtual void					cb_button_mouse_down(PanelButton*)				{;};
	virtual void					cb_button_mouse_up(PanelButton*)				{;};
};


class PanelButton : public PanelSimple {
	public:
		PanelButton();
		PanelButton( PanelButtonCallBack* );
		void init();
		
		
		virtual void		haveCapture();
		virtual void		lostCapture();
		virtual void		clickLeft( int, int);
		virtual void		releaseLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();

		void				debug(bool);

		void				texUp();
		void				texDown();
		void				texOver();
		
		void				setUp( char *);
		void				setDown( char *);
		void				setOver( char *);
		
		void				setCallBackOver( CALLBACK_MOUSE p)		{ pCallBackOver = p; }
		void				setCallBackUp( CALLBACK_MOUSE p)		{ pCallBackUp = p; }
		void				setCallBackDown( CALLBACK_MOUSE p)		{ pCallBackDown = p; }

	protected:
		PanelButtonCallBack*	pPanelButtonCallBack;
		_Texture2D*				m_pTexUp;
		_Texture2D*				m_pTexDown;
		_Texture2D*				m_pTexOver;

		_Texture2D*				m_pTexCurrent;
		
		CALLBACK_MOUSE			pCallBackDown;
		CALLBACK_MOUSE			pCallBackUp;
		CALLBACK_MOUSE			pCallBackOver;;
};






class PanelCheckBox;

typedef void (* CALLBACK_TRUE)();
typedef void (* CALLBACK_FALSE)();
typedef void (* CALLBACK_CHECK)(PanelCheckBox*);




class PanelCheckBox : public PanelSimple {
	public:
		PanelCheckBox();
		
		
		//void				buildText();

		virtual void		clickLeft( int, int);
		virtual Panel*		isMouseOver( int, int);
		virtual void		displayGL();
		virtual void		updatePos();
		void				debug(bool);
		void				setFalse( char *);
		void				setTrue( char *);
		
		void				setCallBackTrue( CALLBACK_TRUE p)		{ pCallBackTrue  = p; }
		void				setCallBackFalse( CALLBACK_FALSE p)		{ pCallBackFalse = p; }
		void				setCallBackMouse( CALLBACK_CHECK p)		{ pCallBackMouse = p; }
		
		bool				getVal()								{ return bVal; }
		void				setPercent( float );
		inline void			setVal( bool b )						{ bVal = b; }
		inline void			setListener( bool* p)					{ pVal = p; }
		inline void			setUseInverse( bool p)					{ bUseInverse = p; }

	protected:
		_Texture2D*		m_pTexTrue;
		_Texture2D*		m_pTexFalse;

		CALLBACK_TRUE	pCallBackTrue;;
		CALLBACK_FALSE	pCallBackFalse;
		CALLBACK_CHECK  pCallBackMouse;
		
		bool			bVal;
		bool*			pVal;
		bool			bUseInverse;
		
		float			percent;
//	private:
		/*	
	protected:
		_Texture2D*		m_pTexBackground;
		
		bool							bTextOK;
		void * 							cTextObj;
		TextUtil*						textUtil;
		std::vector<std::string>		str;

		//bool							bDebug;				in Panel.h
		PanelSimple*					pPsDebug;
		PanelText*						pPtDebug;
		std::string						sDebug;
		*/
};






class PanelWindow : public PanelSimple {
	public:
		enum SKIN { STD, RED, BLACK };
	
		PanelWindow();
		
		void				loadSkinPath( std::string );
		void				loadSkin( std::string );
		void				loadSkin( SKIN );
		
		void				setColor( long );

		void				setPosAndSize(int, int, int, int);
		void				setPos(int, int);
		void				setdSize(int, int);

		inline void 		setBorderSize( int n )											{ borderSize = n; };
		inline int	 		getBorderSize()													{ return borderSize; };

		void				displayGLtex( _Texture2D* , float, float, float, float );
		void				displayGLBordure();

        Panel *             isMouseOverUpperLeft(int,int);
        Panel *             isMouseOverUpper(int,int);
        Panel *             isMouseOverUpperRight(int,int);
        Panel *             isMouseOverLeft(int,int);
        Panel *             isMouseOverRight(int,int);
        Panel *             isMouseOverBottomLeft(int,int);
        Panel *             isMouseOverBottom(int,int);
        Panel *             isMouseOverBottomRight(int,int);

        virtual Panel*      isMouseOverBorder(int, int);
        virtual Panel*      isMouseOver(int, int);
		virtual void		clickLeft( int, int);
		virtual void		releaseLeft( int, int);

		virtual void		displayGL();
		virtual void		updatePos();
		
	private:
		_Texture2D*			m_tex_tl;
		_Texture2D*			m_tex_tr;
		_Texture2D*			m_tex_bl;
		_Texture2D*			m_tex_br;
		_Texture2D*			m_tex_t;
		_Texture2D*			m_tex_b;
		_Texture2D*			m_tex_l;
		_Texture2D*			m_tex_r;
		
		int					borderSize;
};















	extern void                log( char*  );
	extern void                logf(char *, ...);
    extern void                log_tab( bool );
	void                log( char*  );
	void                logf(char *, ...);
    void                log_tab( bool );



class WindowsManager {

public:


	void				init();

/*
static	void			log( char*  );
static	void            logf(char *, ...);
static	void            log_tab( bool );
*/

	void				setScreenSize(int, int);
	void				add(Panel *);
	void				supCapture(Panel *);
	void				supFocus(Panel *);
	void				sup(Panel *);
	void				onTop(Panel *);
	void				onBottom(Panel *);
	void				supByID(int);
	Panel*				getByID(int);
	int					getFreeID();
	Panel*				getParentScroll(Panel*);
	Panel*				getParentRoot(Panel*);
	Panel*				findPanelMouseOver(int, int);
	int					getOrder(Panel*);
	void				movePanel( int, int);
	void				movePanel( int, int, Panel* );
	void				swapVisible();
	void				debug(bool);
	void				debug();
	void			    changeCapture(Panel *);
	void			    changeFocus(Panel *);
	
	/*
	void			setWidth( int);
	int				getWidth();
	void			setHeight( int );
	int				getHeight();
	Font*			getFonts();
	int				getOffsetX();
	int				getOffsetY();
	*/


	inline int 			getNbPanel()	     			{return childs.size();}
	inline void			setDebug( bool b )				{debug(b);}
	inline int			getDebug()						{return bDebug;}
	inline void			setWidth( int w)				{width=w;}
	inline int			getWidth()						{return width;}
	inline void			setHeight( int h )				{height=h;}
	inline int			getHeight()						{return height;}
//	inline _Font*		getFonts()						{return fonts;}
	inline int			getOffsetX()					{return 0;}
	inline int			getOffsetY()					{return 0;}
	inline TextUtil*	getTextUtil()					{return &textUtil;}
	inline void			setSlot( int s )				{slot=s;}
	inline int			getSlot()						{return slot;}
	inline Panel *  	getCapture()    				{return panelCapture;}
	inline Panel *  	getFocus()      				{return panelFocus;}
	std::vector<Panel*>&	getChilds()					{return childs;}

	void				ChangeViewport(int, int, int, int, int, int);

	void				idleGL(float);
	void				displayGL();
	void				clearBufferGL();
	void				clearBufferGL( GLbitfield );
		
	void				passiveMotionFunc(int, int);
    bool                isPanelFocus(Panel *);
    bool                isPanelCapture(Panel *);
	void				motionFunc(int, int);
	void				mouseFunc(int, int, int, int);
	void				keyboardUpFunc( unsigned char, int, int );
	void				keyboardFunc( unsigned char, int, int );
	void				keyboardSpecialUpFunc( unsigned char, int, int );
	void				keyboardSpecialFunc( unsigned char, int, int );

	void				call_back_keyboard( Panel * );
	void				sup_call_back_keyboard( Panel * );
	bool				is_call_back_keyboard( Panel * );

	inline void         startKeyboard()                 { bStopKeyboard = false; }
	inline void         stopKeyboard()                  { bStopKeyboard = true; }
	inline float        getMouseX()                     { return mouseX; }
	inline float        getMouseY()                     { return mouseY; }
	inline int          getMouseState(int but)          { return iMouseButton[but]; }

	int					getNbTextures()					{ return _Texture2D::getNbTextures(); }
	
static GLubyte*         OpenImage( const std::string& filename, unsigned int& w, unsigned int& h, unsigned int& d);


static void             loadResourceImage( const std::string& );


	inline static WindowsManager&	getInstance()			{ if (!instance) instance = new WindowsManager();return *instance;}
	inline static void				Destroy()				{ if (instance) delete instance;instance=0;}

	static void				        genereMipMap(bool b);

	WindowsManager();
	WindowsManager(int, int);
	~WindowsManager();

	int					iTest;

				
private:
	static WindowsManager*	instance;
	
	int 						width;
	int							height;
	std::vector<Panel*>			childs;
	//std::vector<Panel*>		panels;
	//_Font*						fonts;
	
	int							xm_old;
	int							ym_old;
				
	void * 						cTextObj;
	TextUtil					textUtil;
	std::string					str[10];

	bool						bMovePanel;
	bool						bResize;
	bool						bMotionMiddle;
	bool						bMotionLeft;
	bool						bMotionRight;
	
	Panel*						panelMove;
	Panel*						panelCapture;
	Panel*						panelFocus;
	Panel*						panelResize;
	Panel*						panelMotionMiddle;
	Panel*						panelMotionLeft;
	Panel*						panelMotionRight;
	std::vector<Panel *>		panelCallBackKeys;
	
	bool						bDebug;
	int							slot;

	bool						bStopKeyboard;
        
    //string                      sTab;
    //int                         nb_tab;
    
    float                       mouseX;
    float                       mouseY;
    int                         iMouseButton[10];
	
};





#endif
