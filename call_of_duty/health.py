from pymem import *
from pymem.process import *

import keyboard

shortcut = "F4"

mem = Pymem("CoDSP.exe")
module = module_from_name(mem.process_handle,"gamex86.dll").lpBaseOfDll

offset = 0x2F0

def getAddr(base,offset):
    addr = mem.read_int(base)
    addr = addr + offset
    return addr


while True:
    if keyboard.is_pressed(shortcut):
        mem.write_int(getAddr(module+0x0005C8E4,offset),600)
        

