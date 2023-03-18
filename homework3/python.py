 
file = open("/cfs/klemming/home/w/whafu/Private/SF2568-Parallel-Computations-for-Large-Scale-Problems/homework3/sorted_example_P1_N1000.txt", 'r')

array = []
for line in file.readlines():
    array = line.rstrip().split(', ') #using rstrip to remove the \n

array = [float(num.replace(",","")) for num in array] 
y = array[:]
y.sort()
sorted = (array==y)

if sorted:
    print("The array is sucessfully sorted")
else:
    print("The array is NOT sorted")
