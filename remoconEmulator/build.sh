#!/bin/bash
g++ -Wall -pthread -o test test.cpp remoconEmulator.cpp -lpigpio -lrt