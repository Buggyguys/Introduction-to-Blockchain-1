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
