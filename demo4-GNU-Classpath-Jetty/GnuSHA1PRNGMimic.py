import struct
import hashlib

class GnuSHA1PRNGMimic():
    hashf = hashlib.sha512
    hashlen = 64
    
    def __init__(self,seed = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]):
        s = "".join([struct.pack("=B",i) for i in seed]) # form string from list

        self.state = GnuSHA1PRNGMimic.hashf()
        self.state.update(s)
        self.acc = ""

    def setSeedL(self,l):
        self.state.update(struct.pack(">q",l))

    def nextBytesN(self,num):
        if num - len(self.acc) <= 0:
            x = self.acc[0:num]
            self.acc = self.acc[num:]
            return x

        nnum = num - len(self.acc)
        q = nnum / GnuSHA1PRNGMimic.hashlen
        r = nnum % GnuSHA1PRNGMimic.hashlen

        output = self.acc
        self.acc = ""
        
        for i in range(q):
            x = self.state.copy().digest()
            self.state.update(x)
            output += x
            
        if r != 0:
            y = self.state.copy().digest()
            self.state.update(y)
            output += y[:r]

            self.acc = y[r:]

        return output
        
       
    def nextBitsN(self,num):
        j = (num + 7) / 8

        k = 0
        temp = self.nextBytesN(j)
        for l in range(j):
            t = int(struct.unpack("=b",temp[l])[0]) & 0xff
            k = k ^ (t << (8*l))

        return k & (2**num-1)

    """
    Main method that generates value between 0 and limit-1
    It uses the same algorithm as for java.util.Random
    """
    def nextInt(self,limit):
        if limit <= 0:
            raise ValueError

        if (limit & -limit) == limit: # limit is power of 2
            return limit*self.nextBitsN(31) >> 31 # output limit*(seed >> 17) >> 31

        while True: # i is not power of 2
           v = self.nextBitsN(31) # (seed >> 17)
           k = v % limit # output is (seed >> 17) modulo limit 

           if (v-k + limit-1) > 0: # remove statistical bias
               return k

    def nextI32(self):
        x = self.nextBitsN(32)
        if (x >> 31) == 1:
            x = -(2**32-x)
        return x

    def nextLong(self):
        high = self.nextI32() << 32
        low = self.nextI32()

        return high+low

if __name__ == "__main__":
    seed = [13]*32
    rnd = GnuSHA1PRNGMimic(seed)
    rnd.setSeedL(rnd.nextLong())

    for i in range(10):
        print abs(rnd.nextLong())
