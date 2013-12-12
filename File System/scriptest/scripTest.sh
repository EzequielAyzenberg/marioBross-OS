#!/bin/bash
# -*- ENCODING: UTF-8 -*-

mkdir carpeta1
mkdir carpeta1/car1-1
mkdir secret
rm -r secret
truncate -s 0 algo.txt
truncate -s 0 carpeta1/secreto.txt
truncate -s 5000 algo.txt
dd if=/home/utnso/Escritorio/archivogrande.rar of=disco.bin bs=1024 count=10240
cp /home/utnso/tp-2013-2c-c-o-no-ser/FileSystem/Testdisk/grasa-format grasa-format
./grasa-format disco.bin
cp /home/utnso/disco.bin disco.bin
cp /home/utnso/tp-2013-2c-c-o-no-ser/FileSystem/Testdisk/grasa-dump grasa-dump
./grasa-dump disco.bin > dump.txt
echo "probando cosas en el FS" > texto.txt
echo "haciendo movimientos raros" > carpeta1/car1-1/raro.txt
rm grasa-dump
cp dump.txt lctmab.txt
mkdir re
mkdir re/cur
mkdir re/cur/sion
a=0
while [ $a -lt 1000 ]

do
        echo "soy el registro numero " > re/cur/sion/recursive.txt
        echo $a > re/cur/sion/recursive.txt
        echo "\n" > re/cur/sion/recursive.txt
        a=`expr $a + 1`
done
