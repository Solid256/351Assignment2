# Makefile for project 1. Compiles main and the needed classes with the g++ compiler.

ALL: PROG1
CC = g++
PROG1 : 
	$(CC) -c main.cpp
	$(CC) -c Process.cpp
	$(CC) -c MemoryManager.cpp
	$(CC) -o OS_Simulator main.o Process.o MemoryManager.o