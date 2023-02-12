from PIL import Image, ImageDraw
import numpy as np
import sys
np.set_printoptions(threshold=sys.maxsize)

N = 256
B = 2
# Image size (pixels)
W = 100
H = 100
# Plot window
RE_START = -2
RE_END = 1
IM_START = -1
IM_END = 1

palette = []

def mandelbrot(c):
    z = 0
    n = 0
    while abs(z) <= B and n < N:
        z = z*z + c
        n += 1
    return n

def main():
    colors = np.zeros((W, H), dtype=int)
    im = Image.new('RGB', (W, H), (0, 0, 0))
    draw = ImageDraw.Draw(im)

    for x in range(0, W):
        for y in range(0, H):
            # Convert pixel coordinate to complex number
            c = complex(RE_START + (x / W) * (RE_END - RE_START),
                        IM_START + (y / H) * (IM_END - IM_START))
            # Compute the number of iterations
            m = mandelbrot(c)
            # The color depends on the number of iterations
            color = 255 - int(m * 255 / N)
            print(f"m: {m}, c: {c}, color: {color}")
            # Plot the point
            draw.point([x, y], (m, m, m))
            colors[x, y] = int(m)
    
    # print(colors)
    np.savetxt('color_python.txt', colors.astype(int), delimiter=' ', fmt='%i') 
    im.save('color_python.png', 'PNG')

if __name__ == "__main__":
    main()