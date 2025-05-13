# ECDSA

dependência:
```bash
sudo apt install libgcrypt20-dev
```

```bash
g++ ecdsa384.cpp -lgcrypt -o ecdsa
```

```bash
./ecdsa
```

## PQC

```bash
chmod +x install.sh 
```

```bash
./install.sh
```

```bash
cp sig.cpp liboqs-cpp/examples/sig.cpp 
```

```bash
cmake --build liboqs-cpp/build --target sig
```

```bash
./liboqs-cpp/build/sig
```
