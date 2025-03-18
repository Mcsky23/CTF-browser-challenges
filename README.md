# 🔥 CTF browser challenges 🔥

> Collection of browser challenges forked from [here](https://github.com/exd0tpy/CTF-browser-challenges/) changed with my own solutions. I plan on expanding this.

|Challenge|CTF|Difficulty|
|------|-----|---|
|[Baby WASM](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Baby_WASM)|RITSEC CTF 2021|⭐|
|[Kit Engine](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Kit_Engine)|picoCTF 2021|⭐|
|[oob-v8](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/oob-v8)|*CTF 2019|⭐✚|
|[Is this pwn or web](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Is_this_pwn_or_web)|DownUnderCTF 2020|⭐✚|
|[Download Horsepower](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Download_Horsepower)|picoCTF 2021|⭐⭐|
|[Turboflan](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Turboflan)|picoCTF 2021|⭐⭐|
|[Chromium RCE](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/chromium_rce)|0ctf/tctf 2020|⭐⭐|
|[Teen WASM](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/Teen_WASM)|RITSEC CTF 2021|⭐⭐✚|
|[babychrome](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/babychrome)|LINE CTF 2021|⭐⭐⭐|
|[The False Promise](https://github.com/Mcsky23/CTF-browser-challenges/tree/main/The_False_Promise)|PlaidCTF 2021|❓❓❓|

## Setup

To build `d8` follow these intructions taken from [here](https://faraz.faith/2019-12-13-starctf-oob-v8-indepth/).

```bash
pwn@ubuntu:~$ fetch v8
pwn@ubuntu:~$ cd v8
pwn@ubuntu:~/v8$ ./build/install-build-deps.sh # Assumes you're using apt
pwn@ubuntu:~/v8$ git checkout "<v8 commit hash>"
pwn@ubuntu:~/v8$ gclient sync
pwn@ubuntu:~/v8$ git apply ../patch.diff
pwn@ubuntu:~/v8$ ./tools/dev/v8gen.py x64.release
pwn@ubuntu:~/v8$ ninja -C ./out.gn/x64.release # Release version
pwn@ubuntu:~/v8$ ./tools/dev/v8gen.py x64.debug
pwn@ubuntu:~/v8$ ninja -C ./out.gn/x64.debug # Debug version
```

## Debugging

You can also use this simple script to start gdb faster when debugging.

```bash
#!/bin/bash

# search for d8 process and attach gdb
pid=$(pgrep -f "./d8")
if [ -z "$pid" ]; then
  echo "d8 process not found"
  exit 1
fi

echo "Attaching to d8 process $pid"
gdb -p $pid
```

If you want to debug using vscode, build `d8` like normal and then copy this configuration file into `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug d8",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/v8/out.gn/x64.debug/d8", // Path to the d8 binary
            "args": ["--allow-natives-syntax", "--trace-turbo", "--trace-opt", "--trace-deopt", "--shell", "${workspaceFolder}/solve/exploit.js"], // Optional: Add arguments for the d8 binary here
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}/v8/out.gn/x64.debug",
            // "sourceFileMap": { "../../": "${workspaceRoot}" },
            "externalConsole": false,
            "MIMode": "gdb", // Change to "lldb" if using lldb
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```



