#Makefile pour Lab5


#Configuration standard
OPTIONS =

#Configuration avec debugage
#OPTIONS = -g -O0 -Wall

#Configuration avec optimisation
#OPTIONS = -O3 -Wall

# Ajoutez random_passages pour la tâche 4
#all: test_liste lab5
all: test_liste lab5 random_passages

test_liste: test_liste.cpp liste.h
	$(CXX) $(OPTIONS) -o test_liste test_liste.cpp

lab5: lab5.cpp liste.h
	$(CXX) $(OPTIONS) -o lab5 lab5.cpp

random_passages: random_passages.cpp
	$(CXX) $(OPTIONS) -std=c++11 -o random_passages random_passages.cpp
#  ou
#	$(CXX) $(OPTIONS) -std=c++0x -o random_passages random_passages.cpp


clean :
	rm -f test_liste
	rm -f lab5
	rm -f random_passages
