import signal
import sys
import time
from packet import Packet
from client import Client

if __name__ == "__main__":
    server_ip = '192.168.1.64'
    server_port = 4001
    rate = 30 # transmission rate in hz

    def signal_handler(sig, frame):
        print("exiting")
        sys.exit(0)

    signal.signal(signal.SIGINT, signal_handler)

    client = Client(server_ip, server_port)
    packet = Packet()
    while True:
        # This is where you'd fetch the NL2 data
        # This is where you'd convert NL2 data to the packet format
        client.send_data(packet.format_packet())
        packet.time_tick()
        time.sleep(1.0/float(rate))
        #print(packet.tick)

