
import socket
import cv2
import numpy
import sys


def send_pic():
    address = ('192.168.1.10', 7)
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(address)

    except socket.error as msg:
        print(msg)
        sys.exit(1)

    img = cv2.imread('/Users/xiangwang/Downloads/lena.jpg')
    img = cv2.resize(img, (512, 512))
    # cv2.imshow("hello", img)
    # cv2.waitKey()

    sock.send(img)

if __name__ == '__main__':
    send_pic()