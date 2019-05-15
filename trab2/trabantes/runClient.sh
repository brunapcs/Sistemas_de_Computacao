#!/bin/bash
echo "Compilando arquivos necessarios"  
gcc udpcliente.c -o cliente
./cliente localhost 3999
