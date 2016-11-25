import pickle
filename = "/home/djuju/Dropbox/Projects/crankeVisuals/karte"
with open(filename, 'rb') as f:
    a = pickle.load(f)
print a
