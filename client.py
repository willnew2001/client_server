import socket

class Client:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def send_data(self, data):
        try:
            self.client_socket.sendto(data, (self.ip, self.port))

        except Exception as e:
            print("Error: ", e)

    def __del__(self):
        self.client_socket.close()
