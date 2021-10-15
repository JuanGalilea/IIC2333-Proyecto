filename = "./files/memformat.bin"
with open(filename, "r+b") as f:
    for i in range(16):
        chunk = f.read(256)
        if chunk:
            for j,b in enumerate(chunk):
                if b!= 0 :
                    print(b,j,i)
clean = 0
if clean:
    with open(filename, "r+b") as f:
        for i in range(16):
            if chunk:
                for j,b in enumerate(chunk):
                    f.seek(i*256+j)
                    f.write(bytes(1))