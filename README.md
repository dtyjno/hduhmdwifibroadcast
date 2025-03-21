参考资料

OpenHD改造实现廉价高清数字图传:https://blog.csdn.net/hoopertsau/article/details/126150575

wfb-ng Setup HOWTO :https://github.com/svpcom/wfb-ng/wiki/Setup-HOWTO

修补了 wfb-ng 的 rtl88xxau 驱动程序: https://github.com/svpcom/rtl8812au#

wifibroadcast

wifibroadcast

https://docs.px4.io/main/en/companion_computer/video_streaming_wfb_ng_wifi.html
```sh
sudo apt install libpcap-dev libsodium-dev python3-all python3-twisted
git clone -b stable https://github.com/svpcom/wfb-ng.git
cd wfb-ng && make deb && sudo apt install ./deb_dist/wfb-ng*.deb
```

问题：
1. 
```
src/rtsp_server.c:20:10: fatal error: gst/gst.h: 没有那个文件或目录
   20 | #include <gst/gst.h>
      |          ^~~~~~~~~~~
```
解决
```bash
sudo apt-get update
```
<!-- sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev -->

sudo apt-get install -y libgstrtspserver-1.0-dev

2. 
```
/usr/bin/python3: ModuleNotFoundError: No module named ‘virtualenv’
```
<!-- pip install virtualenv -->
<!-- python -m virtualenv --version -->
解决
```sh
sudo apt-get install python3-virtualenv
```
3. 
解决：
```
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

sudo apt install -y libpcap-dev libsodium-dev libevent-dev debhelper dh-python python3-all python3-pip python3-pyroute2 python3-future python3-twisted python3-msgpack python3-stdeb python3-all-dev python3-setuptools python3-jinja2 python3-serial virtualenv
```
编译安装命令
```sh
cd wfb-ng
make deb
sudo apt install ./deb_dist/wfb-ng*.deb
```


编译，最重要的是编译过程需要下载大量的额github的内容因此需要翻墙

开始配置：

https://github.com/svpcom/wfb-ng/wiki/Setup-HOWTO
https://github.com/svpcom/wfb-ng/wiki/Setup-HOWTO

# USB接线检查
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
 


<!--# wsl 未实现

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
-->
 Check USB wiring

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


### raspberrypi
```
make
make install
```

 要编译就要有linux header，在/lib/modules/ 下面有对应版本的头文件，但是没有build目录 /lib/modules/5.10.17-v7+/build 无法编译
        要先安装 sudo apt install raspberrypi-kernel-headers   
        然后建立一个软连接到这里，ln -s /usr/src/linux-headers-5.10.63-v7+ /lib/modules/5.10.17-v7+/build
        这样就能编译了。


        准备好之后，就可以进行安装了，重点就以下两个语句。

#编译，安装驱动
sudo make dkms_install  
 
#用来删除驱动
sudo make dkms_remove 
        问题：
        sudo apt-get install raspberrypi-kernel-headers只能安装最新的版本，可能与当前的操作系统的版本不一致。
        所以要手动下载deb包，https://archive.raspberrypi.org/debian/pool/main/r/raspberrypi-firmware/
        到这个网站找一下版本号。
        因为版本不太好找，所以要查一下对应的linux内核编译的时间，可以通过一下指令
        strings /boot/start.elf | grep VC_BUILD_ID
        然后找到对应的deb包，下载，sudo apt install ./XXXX.deb
        注意，要先把之前的raspberrypi-kernel-headers最好卸载掉
        安装时间比较长，主要是因为会把module下面的模块全部编译一遍。。。


### 其他

git clone https://github.com/svpcom/rtl8812au.git

sudo apt-get install dkms

sudo ./dkms-install.sh

ifconfig #查看网卡


以下命令重新启动 NetworkManager：
```
sudo service NetworkManager restart
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

另一种添加命令：要禁用将其添加到黑名单：
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
# 生成密钥

​问题：WFB-NG 启动时需要加密密钥 /etc/gs.key，但该文件不存在。
​后果：视频流接收进程（wfb_rx）直接崩溃，导致整个服务退出。

Generate encryption keys for ground station and drone: `wfb_keygen`. 
```
./wfb_keygen 
Keypair generated from random seed
Drone keypair (drone sec + gs pub) saved to drone.key
GS keypair (gs sec + drone pub) saved to gs.key
```
You need to put `gs.key` to `/etc/gs.key` on the ground station and `drone.key `to `/etc/drone.key` on the drone. Mention that this script will regenerate key and delete old one. If you run it twice on GS, then you can lose your drone connectivity.
```
sudo mv gs.key /etc/gs.key
sudo mv drone.key /etc/drone.key
sudo chmod 600 /etc/gs.key /etc/drone.key  # 设置权限
```

# 启用BPF JIT加速  
Add net.core.bpf_jit_enable = 1 to /etc/sysctl.conf. Reload sysctl.
```
echo "net.core.bpf_jit_enable = 1" | sudo tee -a /etc/sysctl.conf  
sudo sysctl -p  
```
# 禁用NetworkManager管理接口  （不需要）
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

如果您想覆盖默认调制类型（MCS#1、长 GI、20MHz BW、STBC 1） 您可以为每个流执行此作。流设置是独立的。您可以对它们中的每一个使用不同的调制。 例如：

```
[drone_video]
bandwidth = 20     # bandwidth 20 or 40 MHz
short_gi = False   # use short GI or not
stbc = 1           # stbc streams: 1, 2, 3 or 0 if unused
mcs_index = 1      # mcs index

[drone_mavlink]
bandwidth = 20     # bandwidth 20 or 40 MHz
short_gi = False   # use short GI or not
stbc = 1           # stbc streams: 1, 2, 3 or 0 if unused
mcs_index = 1      # mcs index

[gs_mavlink]
bandwidth = 20     # bandwidth 20 or 40 MHz
short_gi = False   # use short GI or not
stbc = 1           # stbc streams: 1, 2, 3 or 0 if unused
mcs_index = 1      # mcs index
```

另一种配置样板：

地面站：
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

如果出现错误：
SIOCSIFFLAGS: Operation not possible due to RF-kill  

then you need to disable RFKill:

rfkill list all
sudo rfkill unblock all  # 解除RF锁定  


Do `systemctl daemon-reload`, 

`systemctl start wifibroadcast@gs` on the GS and 
`systemctl start wifibroadcast@drone` on the drone.

# Disable wpa_supplicant and other daemons on WFB-NG wlan interface! Use ps uaxwwww | grep wlan to check.（不需要）



关于网卡自动配网等功能禁用的问题
        必须要禁用自动配置功能，这样才能使用wifibroadcast相关功能。
        
  查看还有哪些后台服务用到了网卡（wlan1 替换成你的网卡名称）
```
ps uaxwwww | grep wlan1
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
wfb-cli gs                               # 监控链路状态  

sudo systemctl stop wifibroadcast@gs 

journalctl -u wifibroadcast@gs # 查看日志



​手动设置监控模式(可能需要)：
```bash
sudo ip link set wlxbcec237f0e2e down
sudo iw dev wlxbcec237f0e2e set monitor otherbss
sudo ip link set wlxbcec237f0e2e up
```
​检查网卡状态：
```bash
iw dev  # 确认接口已进入监控模式（Mode: Monitor）
```

```sh
dmesg | grep -i error      # 查看内核错误
lsmod | grep 88XXau        # 检查驱动加载
iw reg get                 # 验证射频区域设置


dmesg | grep -i 8812au  # 无输出: ​驱动未加载
txpower 18.00 dBm  # 当前功率
```


https://blog.csdn.net/lida2003/article/details/144726793


# 视频输出

安装gst-launch-1.0
步骤一：安装deb软件包
```sh
sudo apt install -y gstreamer1.0-tools gstreamer1.0-plugins-base gstreamer1.0-plugins-good
$ sudo apt install -y gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
```

步骤二：启动树莓派视频流
```
libcamera-vid --inline --width 960 --height 540 --bitrate 4000000 --framerate 30 --hflip --vflip --timeout 0 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=35 ! udpsink sync=false host=127.0.0.1 port=5602
```
```
gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! x264enc ! rtph264pay ! udpsink host=127.0.0.1 port=5602
```


---


发送端：摄像头采集 + H.264编码 + WiFiBroadcast发送
bash
```
# 通过ffmpeg采集摄像头数据并实时编码为H.264，通过管道发送到WiFiBroadcast
ffmpeg \
  -f v4l2 -input_format yuyv422 -video_size 640x480 -framerate 30 -i /dev/video0 \
  -vcodec libx264 -preset ultrafast -tune zerolatency -g 20 -bf 0 -f h264 \
  - | sudo wfb_tx -u 5600 -k 1 -n 12 wlan0

```
```
# 先启动WiFiBroadcast接收数据到本地UDP端口
sudo wfb_rx -u 5600 wlan0 &

# 再通过ffplay播放H.264流
ffplay -fflags nobuffer -flags low_delay -framedrop -strict experimental -vf "setpts=0" udp://127.0.0.1:5600
```


https://blog.csdn.net/lida2003/article/details/129491517
```
​一、GStreamer 命令解析
以下以 ​1280x720@30FPS 命令为例，逐段解析其功能：

bash
gst-launch-1.0 v4l2src do-timestamp=true io-mode=mmap device=/dev/video0 \
  extra-controls="controls,image_stabilization=1,h264_profile=4,h264_i_frame_period=60,h264_level=11,power_line_frequency=1,exposure_metering_mong=1,exposure_dynamic_framerate=0,scene_mode=0,video_bitrate_mode=1,video_bitrate=2000000,repeat_sequence_header=1" \
  ! video/x-h264,profile=high,width=1280,height=720,framerate=30/1,stream-format=byte-stream \
  ! h264parse disable-passthrough=true \
  ! rtph264pay config-interval=1 pt=96 mtu=1400 aggregate-mode=zero-latency \
  ! udpsink host=127.0.0.1 port=5602 sync=false
​1. 摄像头采集模块
​组件: v4l2src
​关键参数:
do-timestamp=true: 为视频帧添加时间戳，用于同步。
io-mode=mmap: 使用内存映射方式读取摄像头数据（高性能）。
device=/dev/video0: 指定摄像头设备路径。
extra-controls: 摄像头硬件参数控制（见下表）。
参数名	作用
image_stabilization=1	启用图像稳定功能
h264_profile=4	设置H.264编码为High Profile（高画质）
h264_i_frame_period=60	关键帧间隔60帧（每2秒一个关键帧）
video_bitrate=2000000	码率2Mbps（平衡画质与带宽）
repeat_sequence_header=1	重复发送H.264序列头（提升抗丢包能力）

​2. 编码与封装模块
​**video/x-h264**: 指定输出格式为H.264，设置分辨率、帧率、字节流格式。
​**h264parse disable-passthrough=true**: 强制生成完整的NAL单元（避免分片错误）。
​**rtph264pay**: 将H.264数据封装为RTP包，关键参数：
aggregate-mode=zero-latency: 零延迟模式（禁用RTP包聚合）。
mtu=1400: 适配WiFi MTU，避免分片丢包。

​3. 数据传输模块
​**udpsink**: 通过UDP协议发送数据到本地5602端口。
sync=false: 禁用时钟同步，减少延迟。
host=127.0.0.1: 仅限本地测试，实际需改为接收端IP。

​二、整合 WiFiBroadcast 的完整流程

​1. 发送端（摄像头端）​
bash
# 通过GStreamer生成H.264流，直接通过管道发送给WiFiBroadcast
gst-launch-1.0 v4l2src device=/dev/video0 \
  ! video/x-h264,width=1280,height=720,framerate=30/1 \
  ! h264parse \
  ! fdsink fd=1 | sudo wfb_tx -u 5602 -k 1 -n 12 wlan0
关键修改：

移除RTP封装（WiFiBroadcast自带有序传输，无需RTP）。
fdsink fd=1: 将H.264原始流输出到标准输出。
wfb_tx -u 5600: 使用5600端口发送（需与接收端一致）。

​2. 接收端（地面站）​
bash
# 接收数据并通过GStreamer低延迟播放
sudo wfb_rx -u 5602 wlan0 | \
gst-launch-1.0 fdsrc fd=0 \
  ! h264parse \
  ! avdec_h264 \
  ! videoconvert \
  ! autovideosink sync=false
参数优化：

sync=false: 禁用播放器同步，减少延迟。
avdec_h264: 使用FFmpeg解码器（兼容性更佳）。

​三、不同分辨率/帧率配置模板
​1. 1280x720@60FPS（高帧率模式）​
bash
gst-launch-1.0 v4l2src device=/dev/video0 \
  extra-controls="video_bitrate=4000000" \
  ! video/x-h264,width=1280,height=720,framerate=60/1 \
  ! h264parse \
  ! fdsink fd=1 | sudo wfb_tx -u 5600 -k 2 -n 24 wlan0
调整点：

video_bitrate=4000000: 提高码率至4Mbps（适应高帧率）。
wfb_tx -k 2: 增加FEC冗余包数量（抗高帧率丢包）。
​2. 640x480@60FPS（低延迟模式）​
bash
gst-launch-1.0 v4l2src device=/dev/video0 \
  extra-controls="h264_i_frame_period=30" \
  ! video/x-h264,width=640,height=480,framerate=60/1 \
  ! h264parse \
  ! fdsink fd=1 | sudo wfb_tx -u 5600 -k 1 -n 18 wlan0
调整点：

h264_i_frame_period=30: 关键帧间隔减半（提升抗丢包能力）。
-n 18: 降低发送速率（适配低分辨率）。

​四、延迟优化策略

​1. 你的测试数据（128ms）分析
​来源：GStreamer本地UDP传输延迟（未含WiFiBroadcast）。
​优化方向：
​编码阶段：启用硬件编码（如Jetson Nano的NVENC）。
​传输阶段：使用5GHz频段 + MIMO天线（减少无线干扰）。
​协议优化：禁用RTP封装（节省10-20ms）。
​2. 预期优化后延迟
优化措施	预期延迟
硬件编码 + 5GHz频段	80-120ms
WiFiBroadcast + 零冗余	+30ms
总延迟（端到端）	110-150ms
​五、操作脚本
​1. 动态配置脚本（send_video.sh）​
bash
#!/bin/bash
RES="1280x720"    # 可改为 640x480、800x600 等
FPS="60"          # 可改为 30、45、60
BITRATE="4000000" # 根据分辨率调整

gst-launch-1.0 v4l2src device=/dev/video0 \
  extra-controls="video_bitrate=$BITRATE,h264_i_frame_period=$((FPS*2))" \
  ! video/x-h264,width=$(echo $RES | cut -d'x' -f1),height=$(echo $RES | cut -d'x' -f2),framerate=$FPS/1 \
  ! h264parse \
  ! fdsink fd=1 | sudo wfb_tx -u 5600 -k 1 -n $((FPS*2)) wlan0
​六、常见问题处理
​1. 视频花屏/卡顿
​原因: 高帧率导致WiFi带宽不足。
​解决:
bash
# 降低分辨率或码率
RES="640x480" BITRATE="1000000" ./send_video.sh
​2. 无法启动硬件编码
​检查:
bash
v4l2-ctl --device=/dev/video0 --list-formats-ext
# 确认支持H.264输出
​备用方案: 使用软件编码（移除extra-controls中的H.264参数）。
​七、总结
通过将GStreamer的高效编码与WiFiBroadcast的可靠传输结合，可实现 ​1280x720@60FPS 端到端延迟约150ms 的实时视频传输。实际部署时需根据硬件性能（如CPU负载）动态调整参数，并通过 wfb_rx -d 查看实时丢包率优化FEC冗余。

```

运行以下命令查看摄像头支持的视频格式和分辨率：

```bash
v4l2-ctl --device=/dev/video0 --list-formats-ext
```


根据你的摄像头参数（支持 ​MJPG 1280x720@30FPS 和 ​YUYV 640x480@30FPS），以下是优化后的 ​低延迟视频传输方案，分为 ​MJPG硬编码 和 ​YUYV软编码 两种模式：

​方案一：MJPG硬编码（推荐）​
​1. 发送端（摄像头端）​
```bash

gst-launch-1.0 v4l2src device=/dev/video0 \
  ! image/jpeg,width=1280,height=720,framerate=30/1 \
  ! jpegdec \
  ! x264enc tune=zerolatency speed-preset=ultrafast bitrate=2000 key-int-max=30 \
  ! h264parse \
  ! rtph264pay config-interval=1 pt=96 mtu=1400 \
  ! udpsink host=127.0.0.1 port=5602 sync=false

```
​

2. 接收端（地面站）​

```bash
gst-launch-1.0 udpsrc port=5602 \
  ! application/x-rtp,media=video,encoding-name=H264 \
  ! rtph264depay \
  ! h264parse \
  ! avdec_h264 \
  ! videoconvert \
  ! autovideosink sync=false
```

