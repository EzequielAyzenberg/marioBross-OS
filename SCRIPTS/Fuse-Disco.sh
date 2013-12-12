# Crea y formatea un disco en la carpeta home
#./../File\ System/Disco-100MB.sh
dd if=/dev/urandom of=/home/utnso/disco/disco.bin bs=1024 count=102400
sleep 1
./Fuse-Formateo.sh
