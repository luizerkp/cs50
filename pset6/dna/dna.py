from sys import argv
import argparse
import re
import pandas as pd

# parses argv arguments returning error messages if required argv argument is not inputed, removed help flag  
parser = argparse.ArgumentParser(prog='python dna.py', add_help=False)
# adds argv[1] argument
parser.add_argument('data.csv')
# adds argv[2] argument
parser.add_argument('sequence.txt')
args = parser.parse_args()

data = pd.read_csv(argv[1])
sequence = open(argv[2], 'r').read()

# Saving the column headers as a list
base_group_headers = data.columns.values.tolist()
# Amend list to not include the "names" column
base_group_headers = (base_group_headers[1:])

# dictionary to story highest number of repetions of a dna sequence, 
# using the database.csv headers to search and as keys for dicitonary  
sequence_groups = {}
for header in base_group_headers:
    # regex to find all repeating groups of header sub_string stored as a list
    check_groups = re.findall(rf'(?:{header})+', sequence)
    if check_groups:
        # stores string with the longest reapeating sub_string in list 
        find_max_repetition = max(check_groups)
        # returns an interger that represents the number of repetions of sub_string 
        count = find_max_repetition.count(header)
        # updates dictionary to add key:value pair for sub_string:number_of_repetitions 
        sequence_groups.update({header: count})
    else:
        # updates dictionary key:zero_value for headers not found in sequence.txt
        # helps pandas.loc.all provide more accurate search results as all headers are populated even if they conatain no value
        sequence_groups.update({header: 0})
# perform search by converting dicitonary into series and using .all with axis=1 to match all columns in a row returning false if not all columns match series     
find_match = data.loc[(data[list(sequence_groups)] == pd.Series(sequence_groups)).all(axis=1)]
# if .loc returns false find_match will be empty 
if not find_match.empty:
    # converts dataframe.loc results to a string, without index and .strip gets rid of whitespace to print just the name  
    print(find_match['name'].to_string(index=False).strip())
else:
    print('No Match')