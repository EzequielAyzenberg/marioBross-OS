#./../File\ System/Disco-100MB.sh
dd if=/dev/urandom of=../File\ System/disco.bin bs=1024 count=102400
sleep 1
./../File\ System/grasa-format ../File\ System/disco.bin
