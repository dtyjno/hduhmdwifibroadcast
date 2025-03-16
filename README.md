https://blog.csdn.net/hoopertsau/article/details/126150575

wifibroadcast

wifibroadcast

https://docs.px4.io/main/en/companion_computer/video_streaming_wfb_ng_wifi.html

sudo apt install libpcap-dev libsodium-dev python3-all python3-twisted
git clone -b stable https://github.com/svpcom/wfb-ng.git
cd wfb-ng && make deb && sudo apt install ./deb_dist/wfb-ng*.deb

问题：

sudo apt-get update
<!-- sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev -->
sudo apt-get install -y libgstrtspserver-1.0-dev

/usr/bin/python3: ModuleNotFoundError: No module named ‘virtualenv’
<!-- pip install virtualenv -->
<!-- python -m virtualenv --version -->

# Ubuntu/Debian
sudo apt-get install python3-virtualenv

export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
make deb


sudo apt install -y libpcap-dev libsodium-dev libevent-dev debhelper dh-python python3-all python3-pip python3-pyroute2 python3-future python3-twisted python3-msgpack python3-stdeb python3-all-dev python3-setuptools python3-jinja2 python3-serial virtualenv

编译，最重要的是编译过程需要下载大量的额github的内容因此需要翻墙



https://github.com/svpcom/wfb-ng/wiki/Setup-HOWTO
https://github.com/svpcom/wfb-ng/wiki/Setup-HOWTO

USB接线检查
高质量的USB连接至关重要！
 
劣质/细/无屏蔽的USB线可能导致大量FEC错误和丢包！
 
电源连接：始终将WiFi卡的+5V线连接到BEC（而非USB +5V），建议使用至少5A的BEC供电。
 
滤波电容：在电源与地之间添加≥470μF低ESR电容（如Panasonic EEUFR1V102），用于抑制电压尖峰。多根地线时需注意地回路问题。
 
天线警告：任何情况下不得在未连接天线时启动WiFi卡！
 
 
如何安装支持双向MAVLink遥测和IP隧道的WFB-NG
 
前提条件
 
硬件要求：
 
使用基于8812au或8812eu芯片的网卡（推荐BL-M8812EU2模块）
 
优质5V ≥5A DC-DC电源
 
Panasonic EEUFR1V102电容
 
30x30mm散热片+风扇（必需，BL-M8812EU2无主动散热会快速烧毁）
 

配置步骤

# wsl

定制内核  https://learn.microsoft.com/zh-cn/windows/wsl/connect-usb#attach-a-usb-device

```sh
usbipd list  

5-2    0bda:881a  RTL8812AU-VS(802.11ac 2x2 USB2.0)                             Not shared
```

linux系统加载vhci_hcd模块  
```sh
sudo modprobe vhci_hcd  
```
```
usbipd attach --wsl --busid <busid>  
usbipd detach --busid <busid>  
```
确认网卡已挂载
```
lsusb  
ip link show  # 检查接口名（如 wlan0）
```
# Check USB wiring

关键配件：
​电源：5V ≥5A 的独立电源（通过BEC供电，避免使用USB电源）
​电容：≥470μF 低ESR电容（如Panasonic EEUFR1V102）用于滤波
​散热：30x30mm散热片 + 主动风扇（必须安装，否则网卡易烧毁）
​天线：必须连接天线后通电！
 
Proper usb wiring is very important!

Cheap / thin / unshielded USB cables may issue strange behavior like a lot of FEC errors and lost packets!

Always connect wifi card +5V wire to the BEC (not to USB +5V). Use at least 5A BEC for card power!

Add >= 470uF low ESR capacitor (like ESC has, for example Panasonic EEUFR1V102) between power and ground to filter voltage spikes. Be aware of ground loop when using several ground wires.

In any condition don't power up wifi card without antennas!

# 驱动安装

1. 驱动安装
适用于8812au芯片：

## 安装补丁驱动v5.2.20  

https://github.com/svpcom/rtl8812au


git clone https://github.com/svpcom/rtl8812au.git

sudo apt-get install dkms

sudo ./dkms-install.sh

ifconfig #查看网卡


以下命令重新启动 NetworkManager：
```
$ sudo service NetworkManager restart
```
```

wlxbcec237f0e2e: flags=4099<UP,BROADCAST,MULTICAST>  mtu 4052
        ether bc:ec:23:7f:0e:2e  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

iwconfig #可以查询当前网卡的模式
```

wlxbcec237f0e2e  IEEE 802.11  ESSID:off/any  
          Mode:Managed  Access Point: Not-Associated   Tx-Power=18 dBm   
          Retry short limit:7   RTS thr:off   Fragment thr:off
          Power Management:off
```

# 注意：5.x内核勿用rt28xx系列网卡（注入性能差）  
# 禁用原厂驱动  
echo -e "blacklist 88XXau\nblacklist 8812au\nblacklist rtl8812au\nblacklist rtl88x2bs" | sudo tee /etc/modprobe.d/wfb.conf  

echo "options 88XXau_wfb rtw_tx_pwr_idx_override=30" | sudo tee -a /etc/modprobe.d/wfb.conf  

检查原厂 realtek 驱动程序（如果存在）是否已禁用！

要禁用将其添加到黑名单：
```sh
cat > /etc/modprobe.d/wfb.conf <<EOF
# blacklist stock module
blacklist 88XXau
blacklist 8812au
blacklist rtl8812au
blacklist rtl88x2bs
# maximize output power, see note below
options 88XXau_wfb rtw_tx_pwr_idx_override=30
EOF
```
sudo提权
```sh
sudo tee /etc/modprobe.d/wfb.conf <<EOF
# blacklist stock module
blacklist 88XXau
blacklist 8812au
blacklist rtl8812au
blacklist rtl88x2bs
# maximize output power
options 88XXau_wfb rtw_tx_pwr_idx_override=30
EOF
```
查看写入
```
 cat /etc/modprobe.d/wfb.conf
```

# 启用BPF JIT加速  
Add net.core.bpf_jit_enable = 1 to /etc/sysctl.conf. Reload sysctl.
```
echo "net.core.bpf_jit_enable = 1" | sudo tee -a /etc/sysctl.conf  
sudo sysctl -p  
```
# 禁用NetworkManager管理接口  
echo -e "[keyfile]\nunmanaged-devices=interface-name:wlan0" | sudo tee -a /etc/NetworkManager/NetworkManager.conf  
sudo systemctl restart NetworkManager  

# 设置 /etc/wifibroadcast.cfg

Create /etc/wifibroadcast.cfg with following content: common part for gs and drone:
```
[common]
wifi_channel = 161     # 161 -- radio channel @5825 MHz, range: 5815–5835 MHz, width 20MHz
                       # 1 -- radio channel @2412 Mhz, 
                       # see https://en.wikipedia.org/wiki/List_of_WLAN_channels for reference
wifi_region = 'BO'     # Your country for CRDA (use BO or GY if you want max tx power)  
```

Please note that radio band (2.4 or 5.8 GHz) depends on your wifi adapter model and used antennas!


add to gs:
```
[gs_mavlink]
peer = 'connect://127.0.0.1:14550'  # outgoing connection
# peer = 'listen://0.0.0.0:14550'   # incoming connection

[gs_video]
peer = 'connect://127.0.0.1:5600'  # outgoing connection for
                                   # video sink (QGroundControl on GS)
```
add to drone:
```
[drone_mavlink]
# use autopilot connected to /dev/ttyUSB0 at 115200 baud:
# peer = 'serial:ttyUSB0:115200'

# Connect to autopilot via malink-router or mavlink-proxy:
# peer = 'listen://0.0.0.0:14550'   # incoming connection
# peer = 'connect://127.0.0.1:14550'  # outgoing connection

[drone_video]
peer = 'listen://0.0.0.0:5602'  # listen for video stream (gstreamer on drone)
With this settings WFB-NG will listen on port 14550 on drone and connect to udp://127.0.0.1:14550 on GS.
```

etc


```
[common]  
wifi_channel = 161    # 5.8GHz频段5825MHz（20MHz带宽）  
wifi_region = 'BO'     # 使用玻利维亚地区代码（最大化发射功率）  

[drone_mavlink]  
peer = 'serial:/dev/ttyUSB0:115200'  # 连接飞控串口  

[drone_video]  
peer = 'listen://0.0.0.0:5602'       # 接收视频流端口  

高级调制参数
[drone_video]  
bandwidth = 20     # 20/40MHz带宽  
short_gi = False   # 禁用短保护间隔  
stbc = 1           # 空时块编码流数  
mcs_index = 1      # MCS调制等级  


[gs_mavlink]  
peer = 'connect://127.0.0.1:14550'  # 连接本地QGroundControl  

[gs_video]  
peer = 'connect://127.0.0.1:5600'   # 视频流输出端口  

```

```
sudo tee /etc/wifibroadcast.cfg > /dev/null <<EOF

[common]
wifi_channel = 149     # 149 -- radio channel @5745 MHz, range: 5735--5755 MHz, width 20MHz
wifi_region = 'BO'     # Your country for CRDA (use BO or GY if you want max tx power)
#wifi_power = 3000

[drone_mavlink]
# use autopilot connected to /dev/ttyUSB0 at 115200 baud:
# peer = 'serial:ttyUSB0:115200'

# Connect to autopilot via malink-router or mavlink-proxy:
peer = 'listen://0.0.0.0:14550'   # incoming connection
# peer = 'connect://127.0.0.1:14550'  # outgoing connection

[drone_video]
peer = 'listen://0.0.0.0:5602'  # listen for video stream (gstreamer on drone)

EOF
```

错误：
SIOCSIFFLAGS: Operation not possible due to RF-kill  

then you need to disable RFKill:

rfkill list all
sudo rfkill unblock all  # 解除RF锁定  


Do `systemctl daemon-reload`, 

`systemctl start wifibroadcast@gs` on the GS and 
`systemctl start wifibroadcast@drone` on the drone.

# Disable wpa_supplicant and other daemons on WFB-NG wlan interface! Use ps uaxwwww | grep wlan to check.



关于网卡自动配网等功能禁用的问题
        必须要禁用自动配置功能，这样才能使用wifibroadcast相关功能。
        
  查看还有哪些后台服务用到了网卡（wlan1 替换成你的网卡名称）
```
ps uaxwwww | grep wlan
```

Double check that card is in unmanaged state in nmcli output and ifconfig wlanXX doesn't show any address and card state is down.

Edit /etc/default/wifibroadcast and repace wlan0 with proper wifi interface name. Also add to /etc/NetworkManager/NetworkManager.conf following section:
```
[keyfile]
unmanaged-devices=interface-name:wlan0
```
If available /etc/dhcpcd.conf then edit it and add:
```
denyinterfaces wlan0
```
to ignore WFB-NG interface. 

重启系统就可以了，确保插入的无线网卡是wlan1，并且只禁用wlan1自动配网功能。



# 启动


sudo systemctl start wifibroadcast@gs    # 启动地面站服务  
sudo systemctl start wifibroadcast@drone # 启动无人机服务  
`wfb-cli gs `                              # 监控链路状态  

journalctl -u wifibroadcast@gs # 查看日志