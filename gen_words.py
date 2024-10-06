import numpy as np
from common_utils import *

def twelth_root_of_two(tol=0.0000001):
    x = 1
    while (x**12 - 2)**2 > tol:
        x = (11*x + 2/x**11) / 12

    return x

def power(x, y):

    if(y == 0):
        return 1
    temp = power(x, int(y / 2))

    if (y % 2 == 0):
        return temp * temp
    else:
        if(y > 0):
            return x * temp * temp
        else:
            return (temp * temp) / x

phase_acc = (2**(N-1)-1)
trot = twelth_root_of_two()

f_notes= [f * power(trot, n) for n in range (-9, 2)]

words = [phase_acc * (f / f_cpu) for f in f_notes] # 2**N * (np.array(f_notes)/f_cpu)

index_size = int(phase_acc / m)

print(f"const uint16_t tuning_words [] = {{ {print_c_array([int(np.ceil(e)) for e in words])} }};")
print(f"const uint16_t index_size = {index_size};")
