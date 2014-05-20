class JavaLCGMimic():
    """
    JavaLCGMimic class mimics Java class java.util.Random.

    Programmers mostly use nextInt(int n) method of java.util.Random.
    Which produces pseudo random value between 0 and n-1.

    If instances of JavaLCGMimic and java.util.Random classes are seeded with the
    same value, they produce identical outputs.
    """
    multiplier = 0x5deece66dL
    addend = 11L
    mask = 0xffffffffffffL # 2^48-1 (2 to the power of 48 minus 1)
    
    def __init__(self,seed):
        # initialize seed with value (multiplier XOR seed) AND mask
        self.seed = long(seed ^ JavaLCGMimic.multiplier) & long(JavaLCGMimic.mask)

    def nextI(self,i):
        while True:
            seed1 = (long(self.seed*JavaLCGMimic.multiplier) + JavaLCGMimic.addend) & long(JavaLCGMimic.mask)
            if seed1 != self.seed:
                self.seed = seed1 
                break

        r = self.seed >> 48 - i
        return r

    def nextLong(self):
        high = self.nextI(32) << 32
        if (high >> 63) == 1:
            high = -(2**64-high)

        low = self.nextI(32)
        if (low >> 31) == 1:
            low = -(2**32-low)

        return high+low

        #long(self.nextI(32) << 32)

    def forceSeed(self,fseed):
        self.seed = fseed

    def nextInt(self,limit):
        if limit <= 0:
            raise ValueError

        if (limit & -limit) == limit: # limit is power of 2
            return limit*self.nextI(31) >> 31 # output limit*(seed >> 17) >> 31

        while True: # i is not power of 2
           v = self.nextI(31) # (seed >> 17)
           k = v % limit # output is (seed >> 17) modulo limit 

           if (v-k + limit-1) > 0: # remove statistical bias
               return k


class JavaLCGSub():
    """
    Class JavaLCGSub represents LCG with reduced seed of lentgth 17+MaxPowerOf2(limit).

    If we have two generators JavaLCGMimic and JavaLCGSub that are seeded with the same value.
    And limit is even but not the power of 2. And 2^p divides limit.
    
    x y z - outputs of JavaLCGMimic.
    q w e - outputs of JavaLCGSub.

    Then LSB(x,p) = q, LSB(y,p) = w, and LSB(z,p) = e.
    Where LSB(x,p) - p least significant bits of x. I.e. LSB(00110,3)=110.
    """
    
    multiplier = 0x5deece66dL
    addend = 11L

    # Computes max. possible value of i that 2^i divides limit
    def MaxPowerOf2(self, limit):
        i = 0
        while True:
            if limit%2**i == 0:
                i += 1
                continue

            return i-1

    
    def __init__(self,seed,limit):
        self.mask = 2**(17+self.MaxPowerOf2(limit))-1 # modulo for generator 2^(17+i), i=MaxPowerOf2(limit)

        self.limit = limit
        
        self.seed = long(seed ^ JavaLCGSub.multiplier) & long(self.mask)

    def forceSeed(self,fseed):
        self.seed = fseed

    def nextInt(self):
        while True:
            seed1 = (long(self.seed*JavaLCGSub.multiplier) + JavaLCGSub.addend) & long(self.mask)
            if seed1 != self.seed:
                self.seed = seed1 
                break

        return (self.seed >> 17)%2**self.MaxPowerOf2(self.limit)


def TestLCG(limit = 88,seed = 207514182814786,count = 20):
    """
    Just for testing.
    """
    
    g1 = JavaLCGMimic(seed)

    print "1. Now test JavaLCGMimic work. Seed values is '%i'. Compare it with java.util.Random output."%seed
    for i in range(count):
        print g1.nextInt(limit),

    print
    print

    if limit%2 != 0:
        print "Limit is not divisble by 2. Bye."
        return

    if (limit & -limit) == limit:
        print "Limit is power of 2. Bye."
        return

    g2 = JavaLCGSub(seed,limit)
    g3 = JavaLCGMimic(seed)
    print "2. Now test JavaLCGSub work. Limit values is '%i'. Compare output of JavaLCGSub with %i LSB of JavaLCGMimic output."%(limit,g2.MaxPowerOf2(limit))
    print
    print "JavaLCGMimic\tversus\tJavaLCGSub"
    for i in range(count):
        print bin(g3.nextInt(limit))+ "\t\tversus\t" + bin(g2.nextInt())

    print

if __name__ == "__main__":
    #TestLCG()
    import math,random
    g = JavaLCGMimic(100)
    print g.nextLong()
    print g.nextLong()
    print g.nextLong()
    #g.forceSeed(33269761620847)
    #for i in range(15):
        #print "Output =",g.nextInt(33),"Seed =",g.seed, "SeedH =",(g.seed >> 17),"SeedL =",(g.seed % 2**17)
    #print g.seed
    
        
