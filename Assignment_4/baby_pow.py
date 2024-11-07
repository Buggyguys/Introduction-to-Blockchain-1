import socket
import hashlib
import re
import itertools
import string
import sha3  #for Keccak
import os

IP = os.getenv('SERVER_IP')
PORT = os.getenv('SERVER_PORT')

if PORT:
    PORT = int(PORT)

HASH_FUNCTIONS = {
    "sha256": hashlib.sha256,
    "md5": hashlib.md5,
    "keccak": sha3.sha3_256 
}

# brute-force the prefix for matching
def find_prefix(hash_func, suffix, expected_hash):
    for prefix_len in range(1, 5):
        for prefix in map(''.join, itertools.product(string.ascii_letters + string.digits, repeat=prefix_len)):
            combined_string = prefix + suffix
            computed_hash = hash_func(combined_string.encode()).hexdigest()
            if computed_hash == expected_hash:
                return prefix, combined_string, computed_hash
    return None, None, None

def solve_challenges():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((IP, PORT))
        challenge_count = 1

        while True:
            data = s.recv(1024).decode().strip()

            if "Congratulations" in data or "http" in data:
                print("\n---------------------------------------------------------------")
                print(data)
                print("---------------------------------------------------------------")
                break

            print(f"\nChallenge {challenge_count}")

            # parse server respone
            match = re.search(r'(\w+)\(xxxx \+ ([\w\d]+)\) == ([\w\d]+)', data)

            chosen_hash, suffix, expected_hash = match.groups()
            print(f"{chosen_hash}(xxxx + {suffix}) == {expected_hash}")

            hash_func = HASH_FUNCTIONS.get(chosen_hash)

            # find prefix
            prefix, combined_string, computed_hash = find_prefix(hash_func, suffix, expected_hash)

            print(f"   Prefix: '{prefix}'")
            print(f"   Combined String: '{combined_string}'")
            print(f"   Expected Hash: {expected_hash}")
            print(f"   Computed Hash: {computed_hash}")

            # send prefix
            s.sendall((prefix + '\n').encode())
            challenge_count += 1

def main():
    solve_challenges()

if __name__ == "__main__":
    main()
