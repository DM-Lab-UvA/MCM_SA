import time
import subprocess
import os
import numpy as np

runs = 20
mean_times = []

for i in range(runs):

	start = time.perf_counter()
	sa_args = ("../bin/saa.exe 100 -i my_data_n100_N2000")
	sa = subprocess.Popen(sa_args)
	sa.communicate()
	end = time.perf_counter()

	elapsed = end - start 
	mean_times.append(elapsed)

print(f'{np.mean(mean_times):.3f} +/- {np.std(mean_times):.3f}')





