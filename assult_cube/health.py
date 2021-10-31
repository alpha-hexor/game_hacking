from pymem import *
from pymem.process import *

import time

mem = Pymem("ac_client.exe")
module = module_from_name(mem.process_handle,"ac_client.exe").lpBaseOfDll


offsets = [0x8,0xA0,0x34,0x34,0x3E0]

def gethealtaddr(base,offsets):
    addr = mem.read_int(base)

    for offset in offsets:
        if offset != offsets[-1]:
            addr = mem.read_int(addr+offset)

    addr = addr+offsets[-1]

    return addr


while True:
    time.sleep(5)
    mem.write_int(gethealtaddr(module+0x0010A280,offsets),200)