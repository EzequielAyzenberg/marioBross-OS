#mkdir /home/utnso/GRIDLogs
find /home/utnso/GRIDLogs -type f -exec rm -f "{}" \+
sleep 1
./Debug/Plataforma config.cfg

