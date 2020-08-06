
import socket
import cv2
import numpy
import sys
import time


def send_video():
    address = ('192.168.1.10', 7)
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(address)

    except socket.error as msg:
        print(msg)
        sys.exit(1)

    capture = cv2.VideoCapture(0)
    ret, frame = capture.read()

    while ret:
        frame = cv2.resize(frame, (512, 512))

        if cv2.waitKey(10) >= 0:
            break

        sock.send(frame)
        ret, frame = capture.read()


if __name__ == '__main__':
    send_video()
