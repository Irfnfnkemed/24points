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
    print('Received message:', message)
    # 在此处理接收到的消息，可以与C++后端进行交互等
    response = c_cpp_interface(message)  # 调用C++后端的接口函数
    print(response)
    emit('response', response)


def c_cpp_interface(data):
    # 创建套接字
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接到C++后端服务器
    server_address = ('localhost', 6000)  # C++后端服务器地址和端口
    s.connect(server_address)

    # 发送数据给C++后端
    s.sendall(data.encode())

    # 接收C++后端的响应
    response = s.recv(65536).decode()

    # 关闭套接字
    s.close()

    return response


if __name__ == '__main__':
    socketio.run(app, allow_unsafe_werkzeug=True)
