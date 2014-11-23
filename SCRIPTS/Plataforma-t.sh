find /home/utnso/GRIDLogs/LOGPlataforma/ -type f -exec rm -f "{}" \+
sleep 1
./../Plataforma/Debug/Plataforma ../Plataforma/config.cfg -t
