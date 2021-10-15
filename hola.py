filename = "./files/memfilled.bin"
with open(filename, "rb") as f:
    for i in range(16):
        print(f"Entrada: {i}")
        chunk = f.read(256)
        contador = 0
        if chunk:
            for b in chunk:
                if i == 1:
                    print(contador, hex(b))
                contador += 1
            print("--"*34)