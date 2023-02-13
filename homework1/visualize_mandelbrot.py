import matplotlib.pyplot as plt
import numpy as np

def main():
    file_name = "color_parallel_zoom_dardel_size_2"
    data = np.loadtxt(file_name + ".txt", dtype=int)
    # data = np.loadtxt("color_python.txt", dtype=int)

    plt.imshow(data)
    plt.show()
    plt.imsave(file_name + ".png", data)

if __name__ == '__main__':
    main()