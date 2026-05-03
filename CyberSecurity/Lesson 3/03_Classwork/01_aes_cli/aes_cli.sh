#!/bin/bash

echo "LESSON 3, TASK 1 - AES през Командния ред (CLI) (aes_cli.c)"
echo ""

echo -n "Hello" > plain.txt
echo "Text file [plain.txt] is created with text \"Hello\""

openssl enc -aes-128-ecb -in plain.txt -out cipher.bin -K 0112233445566778899AABBCCDDEEFF -nosalt
echo "File is encrypted"

echo "[plain.txt]:" 
cat plain.txt
echo ""
echo "[cipher.bin]:" 
cat cipher.bin
