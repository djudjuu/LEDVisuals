filename = "/home/djuju/Arduino/LEDVisuals/KarteParallel.txt" 
inp = raw_input("enter path to filename you want to use or an 'x' if you want to use the default (./LEDkarte.txt)\n")
if inp != 'x':
    filename = inp

# read the file into a list
LEDkarte = []
with open(filename) as f:
    for line in f:
        LEDkarte.append(int(line))

# make a lookuptable:
# one list of LED indices for each segment
LEDlookup =[]
nSegs = max(LEDkarte)+1
for i in range(0,nSegs):
   LEDlookup.append([index for index,value in
                     enumerate(LEDkarte) if value == i])

#make a printout that can be added into arduino sketches
print "copy these lines into your arduino code"
print "int LEDmap [",len(LEDkarte), "] = " , str(LEDkarte).replace('[','{') .replace(']','}'),';'


#print "int seg2LedsLookup [",len(LEDkarte),"] = ", str(LEDlookup).replace('[','{') .replace(']','}'),';'



