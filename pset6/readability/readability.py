import re

text = input('Text: ')

# variables for the Coleman-Liau index (index = 0.0588 * L - 0.296 * S - 15.8)
Coleman_Liau_index_L = 0
Coleman_Liau_index_S = 0
Coleman_Liau_index = 0

# detemines the amount of letters in a string
letters = re.findall(r'\w', text)

# determines the amount of words in a string 
words = re.split(r'\s', text)

# determines the amount of sentences in string
sentences = re.findall("[A-Z].*?[\.!?]", text, re.MULTILINE | re.DOTALL)

#  L is the average number of letters per 100 words in the string
Coleman_Liau_index_L = (len(letters) * 100) / len(words)

# S is the average number of sentences per 100 words in the
Coleman_Liau_index_S = (len(sentences) * 100) / len(words)

Coleman_Liau_index = 0.0588 * Coleman_Liau_index_L - 0.296 * Coleman_Liau_index_S - 15.8

# rounds the value of index to nearest interger
Coleman_Liau_index = int(round(Coleman_Liau_index))

# prints out he grade based on the results of Coleman_Liau_index
if Coleman_Liau_index in range(1, 16, 1):
    print(f'Grade {Coleman_Liau_index}')
    
elif Coleman_Liau_index < 1:
    print('Before Grade 1')
    
elif Coleman_Liau_index >= 16:
    print('Grade 16+')