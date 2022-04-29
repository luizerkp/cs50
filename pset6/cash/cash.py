from numbers import Number
from cs50 import get_float
# gets input from user and ensures that only a positive number 
while True:
    change = get_float('Change Owed: ')
    change = round(change, 2) * 100
    if change > 0 and isinstance(change, Number):
        break
# stores the amount of coins used
coins = 0
# keeps track of quaters
while change >= 25:
    change -= 25
    coins += 1
# dimes
while change >= 10:
    change -= 10
    coins += 1
# nickels
while change >= 5:
    change -= 5
    coins += 1
# pennies    
while change >= 1:
    change -= 1
    coins += 1
# prints total number of coins used
print(f'{coins}'),