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
