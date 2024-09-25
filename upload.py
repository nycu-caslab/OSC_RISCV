#!/usr/bin/env python3
import struct, sys, time

split = lambda v, sz: [v[i : i + sz] for i in range(0, len(v), sz)]

SERIAL = sys.argv[1]
FILE = sys.argv[2] if len(sys.argv) > 2 else "./disk/kernel.img"

with open(FILE, "rb") as f:
    kernel = f.read()
    size = len(kernel)

with open(SERIAL, "wb", buffering=0) as tty:
    # print("reboot")
    # tty.write(b"reboot\n")
    # time.sleep(3)

    tty.write(b"a")
    time.sleep(0.5)
    tty.write(struct.pack("<Q", 0x80200000))
    time.sleep(0.5)

    tty.write(b"p")
    time.sleep(0.5)

    print("send command")

    tty.write(b"s")
    time.sleep(0.5)

    print("send kernel size")
    tty.write(struct.pack("<I", size))
    time.sleep(0.5)

    print("send kernel", len(kernel))
    for s in split(kernel, 0x200):
        tty.write(s)
        time.sleep(0.01)

    print("jump kernel")
    tty.write(b"j")
