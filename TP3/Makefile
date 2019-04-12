# TP3
# Object files to either reference or create
OBJECTS = Histoire.o Phrase.o DocumentXML.o tp3.o
# The executable file that will be created at the end
EXEC = tp3
# The flags to use for compilation
FLAGS = -Wall -std=c++11

# Perform action on all object files (May or may not exist)
all : histoire.o phrase.o documentXML.o tp3.o compile

histoire.o : Histoire.cpp Histoire.h
	g++ -c -o Histoire.o Histoire.cpp

phrase.o : Phrase.cpp Phrase.h
	g++ -c -o Phrase.o Phrase.cpp

documentXML.o : DocumentXML.cpp DocumentXML.h
	g++ -c -o DocumentXML.o DocumentXML.cpp

tp3.o : tp3.cpp Graphe.h
	g++ -c -o tp3.o tp3.cpp

compile :
	g++ -o $(EXEC) $(OBJECTS) $(FLAGS)

clean :
	rm -f tp3 *.o