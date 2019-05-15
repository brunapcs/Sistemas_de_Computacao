#!/bin/bash
echo "Compilando arquivos necessarios"  
gcc udpserver.c server.c permissoes.c -c
gcc udpserver.o server.o permissoes.o -o prog
./prog 3999 &
