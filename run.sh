pid_app=$(lsof -t -i :5000)
pid_backend=$(lsof -t -i :6000)

if [ ! -z "$pid_app" ]; then
    kill -9 "$pid_app"
fi

if [ ! -z "$pid_backend" ]; then
    kill -9 "$pid_backend"
fi

g++ include/*.h src/*.cpp *.cpp -o code
./code "6000" info_socket.log error_socket.log &
python3 app.py &