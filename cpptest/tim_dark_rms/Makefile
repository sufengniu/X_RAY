
CC=g++
CFLAGS=-g
TARGET=main

all : $(TARGET) 

t_executable.o : t_executable.cpp t_executable.h
	g++ -c t_executable.cpp
	
count_exe.o : count_exe.cpp count_exe.h t_executable.h
	g++ -c count_exe.cpp
	
imgave_exe.o : imgave_exe.cpp imgave_exe.h t_executable.h
	g++ -c imgave_exe.cpp

imgcomp_exe.o : imgcomp_exe.cpp imgcomp_exe.h t_executable.h
	g++ -c imgcomp_exe.cpp
	
t_thread.o : t_thread.cpp t_thread.h t_executable.h
	g++ -c t_thread.cpp	
	
t_threadpool.o : t_threadpool.cpp t_threadpool.h t_thread.h t_executable.h
	g++ -c  t_threadpool.cpp	
	
$(TARGET).o : $(TARGET).cpp
	g++ -c $(TARGET).cpp

tif.o: tif.cpp tif.h
	g++ -c tif.cpp -o tif.o

#need ti add -pthread flag for linux on below gcc. on cygwin, no flag
$(TARGET) : $(TARGET).o t_threadpool.o t_thread.o count_exe.o t_executable.o imgave_exe.o imgcomp_exe.o
	g++ -pthread $(TARGET).o t_threadpool.o t_thread.o count_exe.o t_executable.o imgave_exe.o imgcomp_exe.o -o $(TARGET) -ltiff	

clean : 
	rm -f *.o
	rm -f $(TARGET)
