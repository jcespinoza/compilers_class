# ******************************************
# * File: IfTest.py
# * A test program for if statement
# ******************************************
random.seed(time.clock())

x = random.randint(0,5)
y = random.randint(0,9)

print ("X = ", x, " Y = ", y)

if x == y:
    print ('X is equal to Y')
else:
    print ('X is not equal to Y')
