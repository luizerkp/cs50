from cs50 import get_int
from sys import exit
from numbers import Number

while True:
    size = get_int('Height: ')
    # gate-check to make sure only numbers from 1 - 8 are accepted
    if size > 0 and size < 9 and isinstance(size, Number):
        break


def main():
    draw_pyramid(size)
    

# recursive function that prints pyramid 
def draw_pyramid(n):

    if n == 0:
        return
    
    draw_pyramid(n - 1),
    
    # left half pyramid
    for i in range(size - n):
        print(' ', end='')
    
    for i in range(n):
        print('#', end='')
        
    # middle spaces
    print(' ' * 2, end=''),
    
    # right half pyramid
    for i in range(n):
        print('#', end='')
        
    # adds new line after each iteration
    print()


if __name__ == "__main__":
    main()