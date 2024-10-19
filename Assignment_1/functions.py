
#SWITCH CASE -----------------------------------------
def wait():
    print("\nInput 'c' tp continue.")
    while True:
        key = input("Continue : ")
        if key == "c":
            break

#TASK 1 ----------------------------------------------
# Function to validate name input
def verify(a):
    while True:
        name=input(a)
        if name.isalpha() and name[0].isupper() and name[1:].islower():
            return name
        else:
            print("The name should start with an uppercase letter followed by lowercase letters.")

#TASK 8 ----------------------------------------------

def inverse(a, p):
    return pow(a, p - 2, p)

def point_addition(p, q, a, ff):
    if p==q:  # point doubling
        if p[1]==0: # point at infinity O
            return (0, 0)
        lam=(3*p[0]**2+a)*inverse(2*p[1], ff)%ff # slope
    else:  # point addition
        if p==(0, 0):
            return q
        if q==(0, 0):
            return p
        if p[0]==q[0] and (p[1]+q[1])%ff==0: # P+(-P)=0
            return (0, 0)
        lam=(q[1]-p[1])*inverse(q[0]-p[0], ff)%ff # slope

    x_r=(lam ** 2-p[0]-q[0])%ff
    y_r=(lam*(p[0]-x_r)-p[1])%ff
    return (x_r, y_r)

def is_on_curve(point, a, b, ff):
    x, y=point
    return (y**2-(x**3+a*x+b))%ff==0
