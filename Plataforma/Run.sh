#mkdir /home/utnso/GRIDLogs
find /home/utnso/GRIDLogs -type f -exec rm -f "{}" \+
sleep 1
#./Debug/Plataforma config.cfg
#valgrind -v --log-file=LogValgrind.log_Heavy --leak-check=full ./Debug/Plataforma config.cfg
valgrind --log-file=LogValgrind_Soft.log ./Debug/Plataforma config.cfg
