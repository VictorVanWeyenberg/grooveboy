from random import random
from math import sin

wanted = [2, 2, 3, 3, 4, 5, 5, 6, 6, 0, 0, 1]

def fit(*args):
    correct = 0
    a = args[0]
    b = args[1]
    h = args[2]
    k = args[3]
    for x in range(12):
        if int(a*sin(b*(x-h))+k) == wanted[x]:
            correct += 1
    return correct / float(len(wanted))

maxi = 0
while True:
    test = tuple((random()*40)-20 for _ in range(4))
    result = fit(*test)
    if result > maxi:
        maxi = result
        print("Loading... " + str(test) + " => " + str(result))
    if result == 1:
        print("Yippie! " + str(test))
        break


