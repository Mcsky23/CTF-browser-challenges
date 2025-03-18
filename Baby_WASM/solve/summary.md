# Baby_WASM - very high level writeup

Libc version: 2.35

This v8 patch implements a wasm memory object shrinking function. If we create a dataview for a memory object's buffer, then shrink, the dataview will still be able to access the freed buffer. This is a UAF vulnerability. Leaking libc is easy.

To get arbitrary write:
- shrink an object to size 0x70
- create a DataView for the object
- shrink the object to any smaller size
- now our DataView can be used to edit the freed 0x70 chunck.
- tcache poisoning to get arbitrary write

For RCE use nobodyisnobody's fsop. 
