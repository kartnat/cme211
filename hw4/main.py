from os.path import abspath as a
import sys
import truss

#Only printing usage information if called with no input arguments, as specified
if len(sys.argv) == 1:
    print('Usage:')
    print('  python3 {} <joints file> <beams file> <optional plot output file>')
    sys.exit(0)
    
joint = sys.argv[1]
beam = sys.argv[2]
#Instantiate different instances of class depending on presence of optional argument
if len(sys.argv) == 4:
    plot = sys.argv[3]
    t = truss.Truss(a(joint), a(beam), a(plot))
else:
    t = truss.Truss(a(joint), a(beam), "no plot")

print(t)
