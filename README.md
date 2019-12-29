bismuth-csprng
##############

native python function takes just over 3 minutes to generate the 1gb seedfile (for bismuth's mmap'd sha224 hash).
by contrast, the c implementation takes just over 30 seconds.


#to use:

python setup.py install

python test.py

