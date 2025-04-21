#ifndef PANEL_SPIN_EDIT_TEXT_H
#define PANEL_SPIN_EDIT_TEXT_H


#include "PanelEditText.h"


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

#endif
