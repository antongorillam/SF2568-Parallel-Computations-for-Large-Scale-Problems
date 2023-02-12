import matplotlib.pyplot as plt
import numpy as np

def main():
    data = np.loadtxt("color_seq_c.txt", dtype=int)
    # data = np.loadtxt("color_python.txt", dtype=int)

    plt.imshow(data)
    plt.show()

if __name__ == '__main__':
    main()