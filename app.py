import argparse
from flask import Flask, render_template
from flask_socketio import SocketIO, emit
import socket

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret_key'
socketio = SocketIO(app)


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('message')
def handle_message(message):
    response = c_cpp_interface(message)
    emit('response', response)


def c_cpp_interface(data):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', server_port)  # C++后端服务器地址和端口
    s.connect(server_address)
    s.sendall(data.encode())
    response = s.recv(65536).decode()
    s.close()
    return response


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--app_port', type=int, default=5000, help='应用程序端口号')
    parser.add_argument('--server_port', type=int, default=6000, help='服务器端口号')
    args = parser.parse_args()
    app_port = args.app_port
    server_port = args.server_port
    socketio.run(app, host='0.0.0.0', port=app_port, allow_unsafe_werkzeug=True)
