#!/usr/bin/python3
import sys
import os
import matplotlib.pyplot as plot
import numpy as np

noOfUsers = list()
throughputs = list()
rtts = list()
os.system("gcc load_gen.c -o load_gen -lpthread")
n = len(sys.argv)
for i in range(100, 5100, 500):
    print("Starting for " + str(i) + " number of users")
    os.system("taskset -c 1,2,3 ./load_gen localhost " +sys.argv[1] +" "+  str(i) + " 0.01 20 >> /dev/null")
    file = open("load_gen.log", "r")
    for line in file:
        if "Number of Users" in line:
            noOfUsers.append(float(line.split(": ")[-1].split('\n')[0].strip()))
        elif "Average Throughput" in line:
            throughputs.append(float(line.split(": ")[-1].split('\n')[0].strip()))
        elif "Average Response Time" in line:
            rtts.append(float(line.split(": ")[-1].split('\n')[0].strip()))
file.close()

print(noOfUsers)
print(throughputs)
print(rtts)



#fig,axis=plot.subplots(2)
plot.figure(1)
plot.subplot(211)
plot.title("No. of users vs Throughput")
plot.plot(noOfUsers, throughputs, label="Throughput", marker='o',ls='-', color='red', markersize=9, mew=2, linewidth=2)
plot.xlabel('Number Of users(n)')
plot.ylabel('Throughput(number of requests/sec)')
plot.title("No. of users vs Throughput")
plot.grid(visible='on')
plot.legend(loc=0)
plot.subplot(212)
plot.plot(noOfUsers, rtts, label="RTT", ls='--', marker='o',color='green', markersize=9, mew=2, linewidth=2)
plot.legend(loc=0)
plot.grid(visible='on')
plot.title("No. of users vs Response Time")
plot.xlabel('Number Of users(n)')
plot.ylabel('Response Time(sec)')
#plot.tight_layout()
plot.show()
