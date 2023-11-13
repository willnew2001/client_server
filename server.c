#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define SERVER_PORT 4001
#define SERVER_IP "192.168.1.64"

int listening = 1;

struct Packet {
    int32_t tick;
    int32_t tmp;
    double x_lin_vel;
    double y_lin_vel;
};

uint32_t
flip_32_bit_int(uint32_t input)
{
    size_t byte_count = sizeof(input);
    int mask = 0xFF;
    int rs = 0;
    int ls = (byte_count-1)*8;
    uint32_t result = 0;

    for (int i = 0; i < byte_count; i++)
    {
        int cur_byte = (input & mask) >> rs;
        result |= cur_byte << ls;

        mask <<= 8;
        rs += 8;
        ls -= 8;
    }

    return result;
}

uint64_t
load_u64be(const unsigned char *buf)
{
    uint64_t result = 0;
    for (int i = 0; i < sizeof(buf); i++)
    {
        result |= ((uint64_t)buf[i] << (sizeof(buf)*(8-i-1)));
    }

    return result;
}

double
flip_double(double input)
{
    unsigned char byte_pointer[sizeof(double)];
    memcpy(&byte_pointer, &(input), sizeof(double));

    uint64_t flipped_raw = load_u64be(byte_pointer);
    double result;
    memcpy(&result, &flipped_raw, sizeof(double));
    return result;
}

void
handle_signal(int signo)
{
    if (signo == SIGINT)
    {
        printf("exiting\n");
        listening = 0;
    }
}

int
main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        return 1;
    }

    // Initialize server address struct
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    // bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        close(server_socket);
        return 1;
    }

    if (signal(SIGINT, handle_signal) == SIG_ERR)
    {
        perror("Error setting up signal handler");
        close(server_socket);
        return 1;
    }

    printf("Listening...\n");

    while (listening)
    {
        struct Packet packet;

        // Recieve data
        ssize_t received_bytes = recvfrom(server_socket, &packet, sizeof(packet), 0, (struct sockaddr*)&client_address, &client_address_len);

        if (received_bytes == -1)
        {
            perror("Error receiving data");
            continue;
        }

        printf("%d %d %0.2f %0.2f\n", flip_32_bit_int(packet.tick), flip_32_bit_int(packet.tmp), flip_double(packet.x_lin_vel), flip_double(packet.y_lin_vel));
    }

    close(server_socket);

    return 0;
}


