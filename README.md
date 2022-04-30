# cs50
This is a collection of Projects completed  2020-2021 for Cs50 on Edx. This is Harvard's Introduction to computer Science. 
The projects include:

## Problem Set 1: (written in C and executed from the command line)
  ### hello
  Simple hello program
  ### mario
  This Program takes a user input of 1 >= n <= 8 and builds a double pyramid of hashes  ( ***#*** ) whose height is equal to ***n*** 
  ### cash
  This is a program that will take in an input from user of change owed and computes the least amount of coins that can be used 
  to give coin change.
  ### credit
  This is program that takes a number input and validates the number on whether it is a valid Visa, Amex or MasterCard credit card 
  number using Luhn's Algorithm.
  ### lab1: Population
  This program that takes a ***start_size*** and ***end_size*** input and calculates the number of years it will take for a llama popluation to
  reach ***end_size*** assuming that a in a population of size ***n*** llamas; each year, ***n / 3*** new llamas are born, and 
  ***n / 4*** llamas pass away.
  
## Problem Set 2: (written in C and executed from the command line)
  ### readability 
  Compute grade reading level of a text that ranges from ***before grade 1*** to ***grade 16+*** using the formula 
  ***index = 0.0588 * L - 0.2396 * S - 15.8***
  ### caesar
  Encrypts text using Caesar's Algorithm ***ci = (pi + k) % 26*** by rotating the alphabet by ***k*** positions.
  > More formally, if p is some plaintext (i.e., an unencrypted message), pi is the ith character in p, and k is a secret key (i.e., a non-negative integer), 
  then each letter, ci, in the ciphertext, c. source: https://cs50.harvard.edu/x/2021/psets/2/caesar/
  ### substitution
  Encrypts a texts by mapping each letter in the alphabet to a another letter via a key.
  > A key, for example, might be the string NQXPOMAFTRHLZGECYJIUWSKDVB. This 26-character key means that A (the first letter of the alphabet) should be 
  > converted into N (the first character of the key), B (the second letter of the alphabet) should be converted into Q (the second character of the key), 
  > and so forth. Source: https://cs50.harvard.edu/x/2021/psets/2/substitution/
  ### lab2: scrabble
  plays a quick hand of a Scrabble *like* game and determines winner based on computed score that uses an array of 26 point values where each index 
  represents the value of a matching letter in the alphabet i.e. *arr[0] to arr[25]* representing **A** to **Z**
  
## Problem Set 3: (written in C and executed from the command line)
  ### plurality
  Simulates a plurality vote election and outputs a winning candidate *i.e. The candidate with the most votes*
  ### runoff
  Simulates a runoff vote election and outputs a winner. This is a ranked choice election where the candidate with the most *highest preference* 
  votes wins.
  ### tideman
  Simulates a Tideman vote election and outputs a winner
  > The Tideman voting method (also known as “ranked pairs”) is a ranked-choice voting method that’s guaranteed to produce the Condorcet winner of the 
  > election if one exists. Source: https://cs50.harvard.edu/x/2021/psets/3/tideman/
  ### lab3: sort
  This is an exercise on identifying Selection, Bubble and Merge sort algorithms based on running three different sorting programs 
  using info like performance sorting different size sets of data that are sorted, randomized and reversed.
  
## Problem Set 4: (written in C and executed from the command line)
  ### filter 
  Applies different filters to a *.bmp* image: Grayscale, Sepia, Reflection, Blur, and Edges.
  ### recover
  Recovers *.jpg* files from *.raw* file and writes them out naming as *###.jpg*. The purpose of the project is to recover 50 jpg images from 
  *card.raw* file.
  ### lab4: volume
  takes an *input.wav* file and a scaling number ***n*** and outputs file that has ***n*** times the volume of the original file.
  
## Problem Set 5: (written in C and executed from the command line)
  ### speller
  Implements version of a spell-check by loading a dictionary from disk into memory use of hash table. The program outputs the words
  "misspelled" i.e. not in the dictionary as well as different performance info like time in loading/unloading the dictionary.
  ### lab5: inheritance
  Builds a family tree based on blood type alleles
  > Complete the implementation of inheritance.c, such that it creates a family of a specified generation size and assigns 
  > blood type alleles to each family member. The oldest generation will have alleles assigned randomly to them.
  > Source: https://cs50.harvard.edu/x/2021/labs/5/

## Problem Set 6: (written in Python)
  ### hello
  same spec as pset1/hello
  ### mario
  same spec as pset1/mario
  ### cash
  same spec as pset1/cash
  ### credit
  same spec as pset1/credit
  ### readability
  same spec as pset2/readability
  ### dna 
  Takes dna sequence text file and using the information read from that file identifies the owner the sequence based on matching data in a CSV file
  
## Problem Set 7: (written in SQL)
  ### movies
  Write SQL queries to answer 13 questions by querying data from a movie database.
  ### fiftyville
  Figure out a mystery crime by querying a database and figure out the culprit based of date in the different tables show your thought
  process and what queries you performed.
  ### lab7: songs
  Write queries to get information from a songs database
  
## Problem Set 8: (written using HTML, CSS, JavaScript, bootstrap)
  ### homepage
  >build a front end homepage using HTML and CSS, the page must link to at least 4 different pages
  For my project, I built blog style website showcasing 5 countries, a *tips & tricks* page, a homepage and an *about me* page.
  ### lab8: trivia
  build a trivia pages with some questions at least one multiple choice and one text based free response use JavaScript to check 
  responses
  
## Problem Set 9: (written using HTML, CSS, Python, jQuery, bootstrap, Jinja)
  ### finance
  A stock trading simulation API that retrieve real time stock prices to simulate trading. Allows users to create a trading account,
  check stock prices and buy/sell stocks. Keeps track of transaction history, current stocks in portfolio including real time stock
  prices. Updates portfolio with current cash and total value.
  ### lab9: birthdays
  A page where you can add birthdays 
