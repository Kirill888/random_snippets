from numpy import *

N = 100
a = 2*pi/N

ca,sa = cos(a),sin(a)

R1 = mat([[ca, -sa, 0 , 0],
          [sa,  ca, 0 , 0],
          [0 ,   0, 1 , 0],
          [0 ,   0, 0 , 1]])


R2 = mat([[ca, 0, -sa, 0],
          [0 , 1,  0 , 0],
          [sa, 0,  ca, 0],
          [0 , 0,  0 , 1]])


print R1
print R2

x0 = mat([1,0,0,1]).T
