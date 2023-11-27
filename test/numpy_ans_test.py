import numpy as np

sz = int(input())

arr = []

for i in range(sz):
    arr.append([float(j) for j in input().split()])

mtx = np.matrix(arr)

print(np.linalg.det(mtx))
