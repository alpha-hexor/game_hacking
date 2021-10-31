from pymem import *
from pymem.process import *

import time


mem = Pymem("ac_client.exe")
module = module_from_name(mem.process_handle,"ac_client.exe").lpBaseOfDll

ammo_offset = [0x8,0x65C,0x34,0x6C,0x1D0]


health_offset = [0x8,0xA0,0x34,0x34,0x3E0]


def getAddr(base,offsets):
    addr = mem.read_int(base)
    for offset in offsets:
        if offset != offsets[-1]:
            addr = mem.read_int(addr+offset)

    addr = addr+offsets[-1]

    return addr

while True:
    time.sleep(2)
    #reload ammo
    mem.write_int(getAddr(module+0x0010A280,ammo_offset),1000)
    #reload health
    mem.write_int(getAddr(module+0x0010A280,health_offset),300)


