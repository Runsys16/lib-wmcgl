#!/usr/bin/python
#-----------------------------------------------------------------------------
# depend.py
#-----------------------------------------------------------------------------
# Creation des dependance pour le fichier makefile
# this script run in src directory
#-----------------------------------------------------------------------------
#  exemple de ligne :
#		$(OBJDIR)Effects.o :   $(SRCDIR)Effects.cpp 
#			@echo ---------   compilation de $@
#			$(CPP) -c $< -o $@  $(CFLAGS)
#-----------------------------------------------------------------------------
# le programme liste toutes les entrees du repertoite courant.
#-----------------------------------------------------------------------------


import os
import sys



def list_dir( name="." ):
	for dirname, dirnames, filenames in os.walk(name):
		#print( "-------------------------")
		#print dirname
		#print( "-------------------------")
		#print filenames
		#print( "-------------------------")
		#print dirnames
		
		listfiles = []

		for filename in filenames:
			if filename.find(".cpp") != -1:
				#listfiles.append(os.path.join(dirname, filename))
				listfiles.append( filename )
			
		return listfiles



def have_file( includes, include ):
	return (include in includes)



def test_file( filename, dirname="./", includes = [], ext_dirname = "" ):
	try:
		f = open( dirname+ext_dirname+filename )
	except:
		print( "Erreur d'ouverture de : \"%s\"" % (dirname+ext_dirname+filename) )
		sys.exc_info()
		sys.exit(1)

	for line in f.readlines():
		
		if line.find("#include \"") != -1 and line.split("#include \"")[0].find("//") == -1:
			#print( "------------" )
			include_name = line.split("\"")[1].split( dirname )[-1]
			include_name = dirname + ext_dirname + include_name

			try:
				ext_dirname = include_name.split(dirname)[1].split("/")[-2] + "/"
			except:
				ext_dirname = ""
			
			if ( ext_dirname != "" ):
				include_name = include_name.split(ext_dirname)[-1]
			
			make_name = ext_dirname + include_name.split(dirname)[-1]
			#print( "MAKENAME : %s" % (make_name) )
			#print( "Recursif dir  : %s" % dirname )
			#print( "Recursif ext  : %s" % ext_dirname )
			#print( "Recursif file : %s" % include_name )
			
			if not have_file( includes, make_name ):
				includes.append( make_name )
				test_file( include_name.split(dirname)[-1], dirname,  includes, ext_dirname)
				
				
	f.close()
	return includes
	

#
#		MAIN
#
print ( "#-------------------------------------------------------" )	
print ( "#              libwmcgl.so.0.1" )
print ( "#-------------------------------------------------------" )	
print
for filename in sorted(list_dir(name="./")):
	#print "MAIN : parours : " + filename
	includes = test_file( filename, dirname="./", includes=[] )

	name = filename.split(".cpp")[0]
	
	#----------------------------------------------------
	# print for makefile
	#----------------------------------------------------
	print( "$(OBJDIR)%s.o :   $(SRCDIR)%s.cpp " % (name, name) ) ,

	for include in includes:
		print( "$(SRCDIR)%s " % include ) ,

	print
	print (	"\t@echo ---------   compilation de $@" )
	print( "\t$(CPP) -c $< -o $@  $(CFLAGS)" )
	print
	
sys.exit(0)

