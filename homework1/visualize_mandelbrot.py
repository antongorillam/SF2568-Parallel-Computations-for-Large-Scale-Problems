import matplotlib.pyplot as plt
import numpy as np

def main():
    # data = np.loadtxt("color.txt", skiprows=128)
    data = np.loadtxt("color.txt", dtype=int)
    # for i,line in enumerate(fh):
    #     if i is n: break
    print(data.shape)
    plt.imshow(data)
    plt.show()

if __name__ == '__main__':
    main()