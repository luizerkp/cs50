from sys import exit

# gets input from user
credit_card_number = [int(item) for item in input("Enter CC Number: ")]

# determines credit card number lenght
length = len(credit_card_number)

matches = (13, 15, 16)

if length not in matches:
    exit('INVALID')

# variables for checksum calculation 
checksum = 0

# numbers that get added per Luhn’s Algorithm
numbers_add = 0

# numbers that are multiplied by 2 per Luhn’s Algorithm
numbers_multiply = 0

# itterates trough each index of the list in reverse using [::-1]
for index, element in enumerate(credit_card_number[::-1]):
    # executes every other index 
    if index % 2 == 0:
        numbers_add += element
    else:
        # temporary value used to store the value of the number multiplied by 2 per Luhn’s Algorithm
        temp = 0
        temp = element * 2
        
        # if the result of temp is a multiple digit number (i.e. 10 or greater) will add each digit 
        # by using modulo 10 to get the last digit and adding 1, resulting in 10 being processed as 1 + 0, 11 as 1 + 1 ect per Luhn’s Algorithm
        if temp > 9:
            numbers_multiply += (temp % 10) + 1
            
        # if temp is less than 10 i.e only one digit then it gets added as is
        else:
            numbers_multiply += temp
            
# stores checksum as calculated           
checksum = numbers_add + numbers_multiply

# if checksum modulo 10 is 0, it indicates a posible valid credit card number
if checksum % 10 == 0:
    # checks if it is a valid Visa number by checking that it is 13 or 16 digits and begins with 4
    if length in (13, 16) and credit_card_number[0] == 4:
        print('VISA')
        
    # checks if it is a valid Amex number by checking it is 15 digits long and begins with 34 or 37
    elif length == 15 and credit_card_number[0] == 3 and credit_card_number[1] in (4, 7):
        print('AMEX')
        
    # checks if it is a valid Master Card by checking if it is 16 digits long and begins with 51, 52, 53. 54, or 55
    elif length == 16 and credit_card_number[0] == 5 and credit_card_number[1] in (1, 2, 3, 4, 5):
        print('MASTERCARD')
    else:
        print('INVALID')
        
# else the card is definately not a valid credit card number
else:
    print('INVALID')