import random as rand

file = open('input.txt', 'w')
file.write(','.join([str(rand.randint(1, 2000)) for x in range(2000000)]))
file.close()
