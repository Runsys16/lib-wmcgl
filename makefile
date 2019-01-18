CPP  = g++
CC   = gcc

VER = 0.1


OBJDIR		= ./build/
BINDIR		= ./bin/
INSTALLDIR	= /usr/lib/
INCLUDEDIR	= /usr/include/

SRCDIR		= ./src/lib/

SRC		= $(wildcard $(SRCDIR)*.cpp)
OBJ		= $(SRC:$(SRCDIR)%.cpp=$(OBJDIR)%.o)
BIN		= $(BINDIR)libwmcgl.so.$(VER)
BINNAME = libwmcgl.so

LIBS	=  -lfreetype -lGL -lglut -lGLU -lGLEW -lIL -shared

INCS =  -I"/usr/include" -I"/usr/include/freetype2" 
CFLAGS = $(INCS) -O2 -fpic -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result
CFLAGS = $(INCS) -g -O2 -fpic -fpermissive -mtune=core2 -Wno-deprecated -Wno-unused-result





RM = rm -f
RMDIR = rm -rf
MKDIR = mkdir -p
CP = cp -f
LN = ln -sf


.PHONY: all

all: before $(BIN)

before:
	@$(MKDIR) $(OBJDIR)
	@$(MKDIR) $(BINDIR)

print:
	@echo -----------------------------------------
	@echo 'Variable OBJ'
	@echo $(OBJ)
	@echo -----------------------------------------
	@echo 'Variable SRC'
	@echo $(SRC)
	@echo -----------------------------------------
	@echo 'Variable BINDIR BIN VER'
	@echo $(BINDIR)$(BIN).$(VER)
	@echo -----------------------------------------

clean: 
	@$(RMDIR) $(OBJDIR)
	@$(RMDIR) $(BINDIR)

sample:
	cd src/sample-00;make clean;make
	cd src/sample-01;make clean;make
	cd src/sample-02;make clean;make

$(BIN): $(OBJ)
	@echo ---------   edition des liens library $@
	$(CPP) $(OBJ) -o $@ $(LIBS)


install:
	@echo ---------   Installation de library $@ dans $(INSTALLDIR)
	$(CP) $(BIN) $(INSTALLDIR)
	$(LN) $(INSTALLDIR)$(BINNAME).$(VER) $(INSTALLDIR)$(BINNAME)
	@./src/build_include
	$(CP) ./include/WindowsManager.h $(INCLUDEDIR)
	#mkdir /usr/share/wmcgl
	#ln -s /home/rene/programmes/opengl/lib-wmcgl/images /usr/share/wmcgl/images



#-------------------------------------------------------------------------------------
#        Scripts
#        Fabrication des dependances de chaque fichier .o
#-------------------------------------------------------------------------------------
# depend.py, build_makefile
#-------------------------------------------------------------------------------------
# Recuperation des scripts (dans le repertoire src) :
# $ cat ../makefile | grep "^#00#" | sed -e "s/^....\(.*\)/\1/g" >depend.py
# $ cat ../makefile | grep "^#01#" | sed -e "s/^....\(.*\)/\1/g" >build_makefile
#-------------------------------------------------------------------------------------



#00##!/usr/bin/python
#00##-----------------------------------------------------------------------------
#00## depend.py
#00##-----------------------------------------------------------------------------
#00## Creation des dependance pour le fichier makefile
#00## this script run in src directory
#00##-----------------------------------------------------------------------------
#00##  exemple de ligne :
#00##		$(OBJDIR)Effects.o :   $(SRCDIR)Effects.cpp 
#00##			@echo ---------   compilation de $@
#00##			$(CPP) -c $< -o $@  $(CFLAGS)
#00##-----------------------------------------------------------------------------
#00## le programme liste toutes les entrees du repertoite courant.
#00##-----------------------------------------------------------------------------
#00#
#00#
#00#import os
#00#import sys
#00#
#00#
#00#
#00#def list_dir( name="." ):
#00#	for dirname, dirnames, filenames in os.walk(name):
#00#		#print( "-------------------------")
#00#		#print dirname
#00#		#print( "-------------------------")
#00#		#print filenames
#00#		#print( "-------------------------")
#00#		#print dirnames
#00#		
#00#		listfiles = []
#00#
#00#		for filename in filenames:
#00#			if filename.find(".cpp") != -1:
#00#				#listfiles.append(os.path.join(dirname, filename))
#00#				listfiles.append( filename )
#00#			
#00#		return listfiles
#00#
#00#
#00#
#00#def have_file( includes, include ):
#00#	return (include in includes)
#00#
#00#
#00#
#00#def test_file( filename, dirname="./", includes = [], ext_dirname = "" ):
#00#	try:
#00#		f = open( dirname+ext_dirname+filename )
#00#	except:
#00#		print( "Erreur d'ouverture de : \"%s\"" % (dirname+ext_dirname+filename) )
#00#		sys.exc_info()
#00#		sys.exit(1)
#00#
#00#	for line in f.readlines():
#00#		
#00#		if line.find("#include \"") != -1 and line.split("#include \"")[0].find("//") == -1:
#00#			#print( "------------" )
#00#			include_name = line.split("\"")[1].split( dirname )[-1]
#00#			include_name = dirname + ext_dirname + include_name
#00#
#00#			try:
#00#				ext_dirname = include_name.split(dirname)[1].split("/")[-2] + "/"
#00#			except:
#00#				ext_dirname = ""
#00#			
#00#			if ( ext_dirname != "" ):
#00#				include_name = include_name.split(ext_dirname)[-1]
#00#			
#00#			make_name = ext_dirname + include_name.split(dirname)[-1]
#00#			#print( "MAKENAME : %s" % (make_name) )
#00#			#print( "Recursif dir  : %s" % dirname )
#00#			#print( "Recursif ext  : %s" % ext_dirname )
#00#			#print( "Recursif file : %s" % include_name )
#00#			
#00#			if not have_file( includes, make_name ):
#00#				includes.append( make_name )
#00#				test_file( include_name.split(dirname)[-1], dirname,  includes, ext_dirname)
#00#				
#00#				
#00#	f.close()
#00#	return includes
#00#	
#00#
#00##
#00##		MAIN
#00##
#00#print ( "#-------------------------------------------------------" )	
#00#print ( "#              libwmcgl.so.0.1" )
#00#print ( "#-------------------------------------------------------" )	
#00#print
#00#for filename in sorted(list_dir(name="./")):
#00#	#print "MAIN : parours : " + filename
#00#	includes = test_file( filename, dirname="./", includes=[] )
#00#
#00#	name = filename.split(".cpp")[0]
#00#	
#00#	#----------------------------------------------------
#00#	# print for makefile
#00#	#----------------------------------------------------
#00#	print( "$(OBJDIR)%s.o :   $(SRCDIR)%s.cpp " % (name, name) ) ,
#00#
#00#	for include in includes:
#00#		print( "$(SRCDIR)%s " % include ) ,
#00#
#00#	print
#00#	print (	"\t@echo ---------   compilation de $@" )
#00#	print( "\t$(CPP) -c $< -o $@  $(CFLAGS)" )
#00#	print
#00#	
#00#sys.exit(0)
#00#




#01##!/bin/bash
#01##-----------------------------------------------------------------------------
#01## build_makefile
#01##-----------------------------------------------------------------------------
#01## this script run in src directory
#01##-----------------------------------------------------------------------------
#01#cd ./src
#01#cat ./makefile.base >../makefile
#01#echo "#-------------------------------------------------------------------------------------" >> ../makefile
#01#echo "#        Scripts" >> ../makefile
#01#echo "#        Fabrication des dependances de chaque fichier .o" >> ../makefile
#01#echo "#-------------------------------------------------------------------------------------" >> ../makefile
#01#echo "# depend.py, build_makefile" >> ../makefile
#01#echo "#-------------------------------------------------------------------------------------" >> ../makefile
#01#echo "# Recuperation des scripts (dans le repertoire src) :" >> ../makefile
#01#echo "# $ cat ../makefile | grep \"^#00#\" | sed -e \"s/^....\(.*\)/\1/g\" >depend.py" >> ../makefile
#01#echo "# $ cat ../makefile | grep \"^#01#\" | sed -e \"s/^....\(.*\)/\1/g\" >build_makefile" >> ../makefile
#01#echo "#-------------------------------------------------------------------------------------" >> ../makefile
#01#
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#
#01#cat ./depend.py | sed -e 's/.*/#00#\0/g' >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#
#01#cat ./build_makefile | sed -e 's/.*/#01#\0/g' >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#echo  >> ../makefile
#01#
#01#cd lib
#01#../depend.py >>../../makefile
#01#cd ..




#-------------------------------------------------------
#              libwmcgl.so.0.1
#-------------------------------------------------------

$(OBJDIR)Font.o :   $(SRCDIR)Font.cpp  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)wm.h  $(SRCDIR)Panel.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)FreeType.o :   $(SRCDIR)FreeType.cpp  $(SRCDIR)FreeType.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)ImageTools.o :   $(SRCDIR)ImageTools.cpp  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)InternalFonts.o :   $(SRCDIR)InternalFonts.cpp  $(SRCDIR)InternalFonts.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)Panel.o :   $(SRCDIR)Panel.cpp  $(SRCDIR)Panel.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelButton.o :   $(SRCDIR)PanelButton.cpp  $(SRCDIR)PanelButton.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelCheckBox.o :   $(SRCDIR)PanelCheckBox.cpp  $(SRCDIR)PanelCheckBox.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelConsole.o :   $(SRCDIR)PanelConsole.cpp  $(SRCDIR)PanelConsole.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelScrollText.o :   $(SRCDIR)PanelScrollText.cpp  $(SRCDIR)PanelScrollText.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelScrollY.o :   $(SRCDIR)PanelScrollY.cpp  $(SRCDIR)PanelScrollY.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelSimple.o :   $(SRCDIR)PanelSimple.cpp  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelText.o :   $(SRCDIR)PanelText.cpp  $(SRCDIR)PanelText.h  $(SRCDIR)Panel.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelTextOmbre.o :   $(SRCDIR)PanelTextOmbre.cpp  $(SRCDIR)PanelTextOmbre.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)PanelWindow.o :   $(SRCDIR)PanelWindow.cpp  $(SRCDIR)PanelWindow.h  $(SRCDIR)PanelSimple.h  $(SRCDIR)Panel.h  $(SRCDIR)PanelText.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)wm.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)ResourceManager.o :   $(SRCDIR)ResourceManager.cpp  $(SRCDIR)ResourceManager.h  $(SRCDIR)Singleton.h  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)Shader.o :   $(SRCDIR)Shader.cpp  $(SRCDIR)Shader.h  $(SRCDIR)Mathlib.h  $(SRCDIR)glInfo.h  $(SRCDIR)Singleton.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)TextUtil.o :   $(SRCDIR)TextUtil.cpp  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)wm.h  $(SRCDIR)Panel.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)Texture.o :   $(SRCDIR)Texture.cpp  $(SRCDIR)Texture.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)Texture2D.o :   $(SRCDIR)Texture2D.cpp  $(SRCDIR)Texture2D.h  $(SRCDIR)Texture.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)glInfo.o :   $(SRCDIR)glInfo.cpp  $(SRCDIR)glInfo.h  $(SRCDIR)Singleton.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

$(OBJDIR)wm.o :   $(SRCDIR)wm.cpp  $(SRCDIR)wm.h  $(SRCDIR)Panel.h  $(SRCDIR)Font.h  $(SRCDIR)FreeType.h  $(SRCDIR)TextUtil.h  $(SRCDIR)InternalFonts.h  $(SRCDIR)ImageTools.h  $(SRCDIR)Mathlib.h 
	@echo ---------   compilation de $@
	$(CPP) -c $< -o $@  $(CFLAGS)

