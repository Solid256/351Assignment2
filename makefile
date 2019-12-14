# Makefile for project 1. Compiles main and the needed classes with the g++ compiler.

ALL: PROG1
CC = g++
PROG1 :
	$(CC) -c -g main.cpp
	$(CC) -c -g Process.cpp
	$(CC) -c -g MemoryManager.cpp
	$(CC) -o OS_Simulator main.o Process.o MemoryManager.o
