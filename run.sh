#---- you can modify the port you want here----
app_port=5000
server_port=6000
#----------------------------------------------
pid_app=$(lsof -t -i :$app_port)
pid_backend=$(lsof -t -i :$server_port)

if [ ! -z "$pid_app" ]; then
    kill -9 "$pid_app"
fi

if [ ! -z "$pid_backend" ]; then
    kill -9 "$pid_backend"
fi

g++ include/*.h src/*.cpp *.cpp -o code
./code $server_port info_socket.log error_socket.log &
python3 app.py --app_port $app_port --server_port $server_port &