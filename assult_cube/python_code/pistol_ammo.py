from pymem import *
from pymem.process import *

import time

mem = Pymem("ac_client.exe")
module = module_from_name(mem.process_handle,"ac_client.exe").lpBaseOfDll


offsets = [0x8,0x790,0x34,0xA4,0x4CC]

def getAddr(base,offsets):
    addr = mem.read_int(base)
    for offset in offsets:
        if offset != offsets[-1]:
            addr = mem.read_int(addr+offset)
    
    addr = addr + offsets[-1]

    return addr


while True:
    time.sleep(2)
    mem.write_int(getAddr(module+0x0010A280,offsets),100)
