#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>  // For htons
#include <sys/ioctl.h>  // For ioctl and SIOCGIFINDEX
// #include <netinet/in.h>
// #include <netinet/ip.h>
// #include <netinet/udp.h>
// #include <netinet/ether.h>

int create_raw_socket(const char *ifname) {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    // 获取接口索引
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl(SIOCGIFINDEX)");
        close(sock);
        return -1;
    }

    // 绑定到指定接口
    struct sockaddr_ll sll = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        .sll_ifindex = ifr.ifr_ifindex
    };
    if (bind(sock, (struct sockaddr*)&sll, sizeof(sll)) == -1) {
        perror("bind");
        close(sock);
        return -1;
    }

    // 设置发送超时
    struct timeval tv = { .tv_sec = 0, .tv_usec = 5000 };
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    return sock;
}

void send_packet(int sock, const uint8_t *data, size_t len) {
    ssize_t sent = send(sock, data, len, 0);
    if (sent == -1) {
        perror("send");
    } else if (sent != len) {
        fprintf(stderr, "Partial send: %zd/%zu\n", sent, len);
    }
}

int main() {
    const char *ifname = "wlan0";
    int sock = create_raw_socket(ifname);
    if (sock == -1) exit(EXIT_FAILURE);

    // 示例：构造一个 802.11 数据帧（需根据实际协议填充）
    uint8_t packet[128] = { /* 802.11 Header + Payload */ };
    
    send_packet(sock, packet, sizeof(packet));
    close(sock);
    return 0;
}
