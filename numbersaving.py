import pickle
import sys

filename = "/home/djuju/Dropbox/Projects/crankeVisuals/karte"
numbers = []
nLeds = 5
for i in range(nLeds):
    a= raw_input()
    numbers.append(int(a))

with open(filename, 'wb') as f:
    pickle.dump(numbers, f)


