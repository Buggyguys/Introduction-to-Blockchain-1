# Introduction to Blockchain Assignments
## [Assignment 1](Assignment_1)
### Requirements:
1. **External Libraries**:
```bash
pip install pycryptodome
pip install matplotlib
```
2. **External files**:
	The file [rockyou.txt](https://www.kaggle.com/datasets/wjburns/common-password-list-rockyoutxt) which can be found on the internet.
---
## [Assignment 2](Assignment_2)
### Requirements:
**MacOS**:
```bash
brew install openssl
```
### How to run:
**MacOS**:
```bash 
gcc -o merkle_tree solution.c merkle_tree.c -I$(brew --prefix openssl)/include -L$(brew --prefix openssl)/lib -lssl -lcrypto
```
---
## [Assignment 3](Assignment_3)
> [!IMPORTANT]  
> Ensure port forwarding on your router, update firewall rules to allow the specified port, and use public IP addresses for cross-network communication

### Requirements:
**Before running the program** set these environment variables in your terminal:
```bash
export EMAIL_FROM="your_email_example"
export EMAIL_TO="recipient_email_example"
export SMTP_SERVER="smtp.example.com:2525"
```
### How to run:
```bash 
go run server.go
```
```bash
go run client.go
```
---
## [Assignment 4](Assignment_4)
> [!NOTE]
> This assignment simulates the mining process, demonstrating proof-of-work (PoW) concepts.

### Environment Variables (for the server **IP** and **Port**)

Before running the code, set the environment variables as follows:

1. **Linux/macOS**
    ```bash
    export SERVER_IP='YOUR_SERVER_IP'
    export SERVER_PORT='YOUR_SERVER_PORT'
    ```

2. **Windows**

    - **Command Prompt**
      ```cmd
      set SERVER_IP=YOUR_SERVER_IP
      set SERVER_PORT=YOUR_SERVER_PORT
      ```

    - **PowerShell**
      ```powershell
      $env:SERVER_IP = "YOUR_SERVER_IP"
      $env:SERVER_PORT = "YOUR_SERVER_PORT"
      ```

> **Note:** Replace `YOUR_SERVER_IP` and `YOUR_SERVER_PORT` with the actual server IP address and port number.

---
### [1. Baby PoW](Assignment_4/baby_pow.py)

In this task, the server provides a hash, the hashing algorithm (which could be `sha256`, `md5`, or `keccak`), and a suffix string. Our goal is to find the missing prefix. The missing prefix is a string that can contain characters from `A-Z`, `a-z`, and `0-9`. The code uses brute force to generate possible prefixes, checking each one until it finds a match. Once the correct prefix is found, it’s sent back to the server, and the process repeats until we receive the flag (after 15 successful attempts).

**Python Packages**
```bash
pip install pysha3
```

**How to run**
```bash
python baby_pow
```
### [2. Adult PoW](Assignment_4/adult_pow.go)

In this task, we are given a SHA-256 hash and need to find a five-character string that can contain characters from `A-Z`, `a-n`, and `0-9`. The goal is to find five such strings in total, and once completed, the server will send back the final flag. Due to the high complexity of brute-forcing these strings, this task is implemented in Go to leverage its speed for faster hashing.

**How to Run**
```bash
go run adult_pow.go
