import struct

class Packet:
    def __init__(self):
        self.tick = 0
        self.temp = 0
        self.x_lin_vel = -0.1
        self.y_lin_vel = 0.2

    def format_packet(self):
        return struct.pack('!lidd', self.tick, self.temp, self.x_lin_vel, self.y_lin_vel)

    def time_tick(self):
        self.tick += 1
