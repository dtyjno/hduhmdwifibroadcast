import cv2
import numpy as np

# GStreamer 管道配置（接收 UDP 视频流）
# 假设视频流为 H.264 over RTP，监听端口 5600
gst_pipeline = (
    "udpsrc port=5600 caps=application/x-rtp,media=video,encoding-name=H264 ! "
    "rtph264depay ! h264parse ! avdec_h264 ! "
    "videoconvert ! video/x-raw,format=BGR ! appsink"
)

# 创建 VideoCapture 对象
cap = cv2.VideoCapture(gst_pipeline, cv2.CAP_GSTREAMER)

if not cap.isOpened():
    print("无法打开视频流")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("视频流中断")
        break

    # 显示视频帧
    cv2.imshow('WFB-NG Video Stream', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()