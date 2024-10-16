import hashlib  # task 1/2/6

#task 6
from Crypto.Util.number import bytes_to_long

# functions
from functions import *

def task_1():
    print("TASK 1")

    #get/check first/last name
    f_name=verify("First name: ")
    l_name=verify("Last name: ")

    x=hashlib.sha256(b"Blockchain")
    y=f_name+"."+l_name+"."+x.hexdigest()

    print("\n Hashed 'Blockchain': " +x.hexdigest())
    print("\n String to be hashed: "+y)

    y = hashlib.sha256(y.encode())
    print("\n Result: "+y.hexdigest())

    wait()

def task_2():
    print("TASK 2")

    tr="8ec47631827329eb114714a20d9b3eeb8a9a4293bd5d6823af0632e8d4f30dfa" #target hash
    textf="rockyou.txt"

    try:
        with open(textf, 'r', encoding="ISO-8859-1") as f:
            for psw in f:
                psw=psw.strip()
                hpsw=hashlib.sha256(psw.encode()).hexdigest()

                if hpsw==tr:
                    print(f"\nPassword found: {psw}")
                    break
            else:
                print("Password not found in the dictionary.")
    except FileNotFoundError:
        print(f"'{textf}' not found.")

    wait()

def task_3():
    print("TASK 3")

    # given variables
    p=17
    q=23
    e=65537 # exponent
    n=p*q # modulus N=p*q
    ms=12 # message

    print(f" N= {n} (modulus)")
    print(f" e= {e} (exponent)")
    print(f" ms= {ms} (message we want to encrypt)")

    ct=pow(ms,e,n)  # encrypt using modular exponentiation ct = (ms^e) % n
    print(f"\n Encrypted ciphertext: {ct}")

    wait()

def task_4():
    print("TASK 4")

    # given variables
    p = 857504083339712752489993810777
    q = 1029224947942998075080348647219
    e = 65537 # exponent

    phi_n=(p-1)*(q-1) # compute phi(N) (Euler s totient function)
    print(f"\n Euler's totient: {phi_n}")

    d=pow(e,-1,phi_n)

    print(f"\nPrivate key d: {d}")

    wait()

def task_5():
    print("TASK 5")

    # given variables
    n=882564595536224140639625987659416029426239230804614613279163 # modulus
    c=77578995801157823671636298847186723593814843845525223303932  # ciphertext from Task 5
    d=121832886702415731577073962957377780195510499965398469843281 # private key from previous task

    ptext = pow(c, d, n) # decrypt with modular exponentiation

    print(f"\nDecrypted plaintext: {ptext}")

    wait()

def task_6():
    print("TASK 6")

    ms=b"crypto{Immut4ble_m3ssag1ng}"# message to sign

    with open("private.key", "r") as keyf: #load N/d from private.key
        lines=keyf.readlines()
        n=int(lines[0].split('=')[1].strip()) # modulus
        d=int(lines[1].split('=')[1].strip()) # private key

    ms=hashlib.sha256(ms)  # hash the message
    ms=ms.digest()  # Get the hash result as bytes
    ms=bytes_to_long(ms) #convert form bytes to long int

    sig = pow(ms, d, n)  # sign message with modular exponentiation

    print(f"Signed message (signature): {sig}")

    wait()

def task_7():
    print("TASK 7")

    # ELLIPTIC CURVE parameters
    p=9739  # Modulus for the finite field
    p_x=8045
    p_y=6936

    #because we're on an elliptic curve q_x variable is the same as p_x but the y coordinates must be negated mod 9739
    q_x=p_x%p
    q_y=(-p_y)%p

    print(f"The point Q(x, y) is: ({q_x}, {q_y})")

    wait()

def task_8():
    print("TASK 8")

    # Curve parameters
    a=497
    b=1768
    ff=9739  # Modulus for the finite field

    # points P/Q/R
    p=(493, 5564)
    q=(1539, 4742)
    r=(4403, 5202)

    pp=point_addition(p, p, a, ff)
    print(f"P + P: {pp}")

    ppq=point_addition(pp, q, a, ff)
    print(f"P + P + Q: {ppq}")

    s=point_addition(ppq, r, a, ff)
    print(f"S (P + P + Q + R): {s}")

    # check S
    if is_on_curve(s, a, b, ff):
        print(f"Point S is on the curve: {s}")
    else:
        print(f"Point S is not on the curve: {s}")

    wait()

def quit_program():
    print("Exiting program.")
    exit()

def switch_case():
    options = {
        "1": task_1,
        "2": task_2,
        "3": task_3,
        "4": task_4,
        "5": task_5,
        "6": task_6,
        "7": task_7,
        "8": task_8,
        "q": quit_program
    }

    while True:
        print("\nChoose a task to run:")
        print("1 - Task 1")
        print("2 - Task 2")
        print("3 - Task 3")
        print("4 - Task 4")
        print("5 - Task 5")
        print("6 - Task 6")
        print("7 - Task 7")
        print("8 - Task 8")
        print("q - Quit")

        choice = input("Enter your choice: ")

        if choice in options:
            options[choice]()
        else:
            print("Invalid choice. Please try again.")
            wait()

if __name__ == "__main__":
    switch_case()

