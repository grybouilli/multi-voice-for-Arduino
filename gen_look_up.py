import matplotlib.pyplot as plt
import math
from common_utils import *

def gen_sinus():
    global f,m,TOP,phase
    t = np.linspace(0,1/f,m+1)
    s = np.sin(2*np.pi * f *t+ phase)
    s = (s+1)/2                             # puts signal in the range [0, 1]
    s *= TOP                                # puts signal in the range [0, TOP]

    return t,s

def gen_square(perc):
    global m,TOP
    p = int(perc*m)
    return [0 if e < m-p else TOP for e in range(m)]

def gen_square50():
    global m,TOP
    return gen_square(.5)

def gen_square25():
    global m,TOP
    return gen_square(.25)

def gen_triangle():
    global m,TOP
    half = m // 2
    inc = lambda x: x * (TOP/half)
    dec = lambda x: 2 * TOP - x * (TOP/half)
    return [int(inc(x)) for x in range(half)]+[int(dec(x)) for x in range(half,m)]

def gen_sawtooth():
    global m,TOP
    inc = lambda x: x * (TOP/(m-1))
    return [int(inc(x)) for x in range(m)]


def gen_semisine():
    global f,m,TOP,phase
    t = np.linspace(0,1/f,m+1)
    s = np.sin(np.pi * f *t)
    s *= TOP                                # puts signal in the range [0, TOP]

    return t, s
t, sinus = gen_sinus()
square50 = gen_square50()
square25 = gen_square25()
triangle = gen_triangle()
sawtooth = gen_sawtooth()
t, semisine = gen_semisine()


def plot_subplots(lists, titles=None):
    # Determine the number of subplots
    num_subplots = len(lists) - 1

    # Determine the number of rows and columns for the subplot grid
    num_cols = min(num_subplots, 3)
    num_rows = math.ceil(num_subplots / num_cols)
    if num_rows > num_cols + 2:
        num_rows = num_cols + 2
        num_cols = math.ceil(num_subplots / num_rows)

    # Create a figure and a set of subplots
    fig, axs = plt.subplots(num_rows, num_cols, figsize=(5*num_cols, 5*num_rows))

    # If there's only one subplot, axs will be a single Axes object, not an array
    if num_subplots == 1:
        axs = [[axs]]
    else:
        axs = axs.reshape(-1, num_cols)

    # Plot each list on its own subplot
    for i, ax in enumerate(axs.flat):
        if i < num_subplots:
            ax.plot(lists[0], lists[i+1])
            if titles and i < len(titles):
                ax.set_title(titles[i])
            else:
                ax.set_title(f'Subplot {i+1}')
        else:
            ax.axis('off')  # Hide empty subplots

    # Adjust the layout
    plt.tight_layout()

    # Show the plot
    plt.show()

if args.show_graph:
    plot_subplots([t[:-1], sinus[:-1], square25, square50, triangle, sawtooth, semisine[:-1]], ["Sine", "Square 25", "Square 50", "Triangle", "Sawtooth", "Semisine"])


print(f"const uint8_t sinewave_lu [] = {{ {print_c_array([int(e) for e in sinus[:-1]])} }};")
print(f"const uint8_t square25_lu [] = {{ {print_c_array(square25)} }};")
print(f"const uint8_t square50_lu [] = {{ {print_c_array(square50)} }};")
print(f"const uint8_t triangle_lu [] = {{ {print_c_array(triangle)} }};")
print(f"const uint8_t sawtooth_lu [] = {{ {print_c_array(sawtooth)} }};")
print(f"const uint8_t semisine_lu [] = {{ {print_c_array([int(e) for e in semisine[:-1]])} }};")