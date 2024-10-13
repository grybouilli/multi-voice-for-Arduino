import argparse
import numpy as np

def print_c_array(l):
    carr = ""
    for i in range(len(l)):
        if i == len(l) - 1:
            carr += str(l[i])
        else:
            carr += str(l[i])+", "
    return carr

# Instantiate the parser
parser = argparse.ArgumentParser(description='Optional app description')
parser.add_argument('--sample_size', type=int, default=16,
                    help='The amount of sample in the waveform look-up table (default = 16)')
parser.add_argument('--max_top', type=int, default=127,
                    help='The maximum top value (default = 127)')
parser.add_argument('--skip', type=int, default=4,
                    help='The number of skipped overflows (default = 4)')
parser.add_argument('--voices', type=int, default=1,
                    help='The number of voices (default = 1)')
parser.add_argument('--base_freq', type=int, default=440,
                    help='The frequency from which the signal is generated (default is A4, 440Hz)')
parser.add_argument('--phase', type=float, default=0.0,
                    help='The phase at origin of the signal (default is 0)')
parser.add_argument('--show_graph', action='store_true',
                    help='Show the graph of the generated signal')
parser.add_argument('--word_bit', type=int, default=16,
                    help='The number of bits encoding the tuning word (default is 16)')

args = parser.parse_args()

m = args.sample_size
N = args.word_bit
VOICES = args.voices

MAX = args.max_top
TOP = int(np.floor(MAX/VOICES))

SKIP = args.skip

f = args.base_freq
f_cpu = (16000000/(MAX+1))/(SKIP)
phase = args.phase