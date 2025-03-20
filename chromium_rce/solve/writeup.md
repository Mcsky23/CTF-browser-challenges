# chromium rce mini writeup

This patch allows us to use the `set` method where either of the `target` or `receiver` can be `detached`, leading to UAF and so on. It also let's us use `%ArrayBufferDetach` without `--allow-natives-syntax`.

## leak libc

To leak libc, simply create 2 big array buffers. Detach one and set the other to the detached one. Now, we have libc pointers and heap pointers in the array buffer we can read.

## UAF + RCE

For RCE, there seems to be a `custom_deleter_` field in the `BackingStore` class. When `custom_deleter_` is set to true, the destructor calls a function pointer stored in the `type_specific_data_` union. 

If we manage to edit a `BackingStore` class allocation chunk, we can set `custom_deleter_` to true, set the function pointer to `system` and write `/bin/sh` in `buffer_start`. Then just detach it.

To exploit the actual UAF, after playing around for a little bit, I found that you can cause an array buffer to have it's `BackingStore` class chunk in place of an old, freed `buffer_start` chunk.

```javascript
arr1 = new ArrayBuffer(0x30); // arr1 first allocation
let arr1_typed = new Uint8Array(arr1);
%ArrayBufferDetach(arr1);
arr2 = new ArrayBuffer(0x30);
arr1 = new ArrayBuffer(0x30); // BackingStore class chunk of this will be in the same place as arr1 first allocation's buffer_start(which we can write whatever we want to using set, even )
```