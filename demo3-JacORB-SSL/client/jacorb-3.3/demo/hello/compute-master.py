#!/usr/bin/python

import hashlib
import binascii

#fixed 48 byte premaster secret
premaster="4C82F3B241F2AC85A93CA3AECE64CD1A4204D590D0E0F1614EA4182B969B2A28858D55D2AFA7802C059B073C8404"

def computeMasterSSL(challenge,random,premaster):

	def computeMaster16(prefix,challenge,random,premaster):
		md5 = hashlib.md5()
		sha1 = hashlib.sha1()

		sha1.update(prefix)
		sha1.update(binascii.unhexlify(premaster))
		sha1.update(binascii.unhexlify(challenge))
		sha1.update(binascii.unhexlify(random))
		
		md5.update(binascii.unhexlify(premaster))
		md5.update(sha1.digest())
		
		return md5.hexdigest()

	
	master = computeMaster16('A',challenge,random,premaster)
	master += computeMaster16('BB',challenge,random,premaster)
	master += computeMaster16('CCC',challenge,random,premaster)

	return master

def computeMasterTLS(challenge,random,premaster):

	def expand(h,p1,arr1,p2,p3,arr2,arr3,arr4):
		arr5 = [54]*64
		arr6 = [92]*64		

		for t in range(p3):
			arr5[t] = (arr5[t] ^ arr1[t+p2]) & 0xFF
			arr6[t] = (arr6[t] ^ arr1[t+p2]) & 0xFF
		
		inner1 = None
		inner2 = None

		j = 48
		k = 0

		while(j > 0):
			hc = h.copy()  ### To simulate MessageDigest behaviour

			hc.update(bytearray(arr5))

			if inner2 == None:
				hc.update(bytearray(arr2))
				hc.update(bytearray(arr3))
			else:
				hc.update(bytearray(inner2))
			
			inner1 = [ord(x) for x in hc.digest()]

			hc = h.copy()
			
			hc.update(bytearray(arr6))
			hc.update(bytearray(inner1))

			inner2 = [ord(x) for x in hc.digest()]
			
			hc = h.copy()

			hc.update(bytearray(arr5))
			hc.update(bytearray(inner2))
			hc.update(bytearray(arr2))
			hc.update(bytearray(arr3))

			inner1 = [ord(x) for x in hc.digest()]

			hc = h.copy()

			hc.update(bytearray(arr6))
			hc.update(bytearray(inner1))

			inner1 = [ord(x) for x in hc.digest()]
			
			m = min(p1, j)
			for n in range(m):
				arr4[k] = (arr4[k] ^ inner1[n]) & 0xFF
				k += 1

			j -= m
				
	arr1 = [ord(x) for x in binascii.unhexlify(premaster)]
	arr2 = [109, 97, 115, 116, 101, 114, 32, 115, 101, 99, 114, 101, 116]
	arr3 = [ord(x) for x in binascii.unhexlify(challenge+random)]
	arr4 = [0]*48

	md5 = hashlib.md5()

	expand(md5,16,arr1,0,24,arr2,arr3,arr4)

	sha1 = hashlib.sha1()
	expand(sha1,20,arr1,24,24,arr2,arr3,arr4)

	return binascii.hexlify("".join([chr(x) for x in arr4]))

print
print "<|[SSL-TLS master secret calculator for Java||||by 0ang3el]|>"
print

tp = "tls10"
while True:
	tp = raw_input('Enter ssl or tls10 [tls10 default] > ')
	if tp == "":
		tp = "tls10"
		break
	elif tp.lower() in ["ssl","tls10"]:
		break
		
sessionid = None
while True:
	sessionid = raw_input('Enter sessionid in HEX [32 bytes] > ')#'53440816f3b241f2ac85a93ca3aece64cd1a4204d590d0e0f1614ea4182b969b'#
	if sessionid != "":
		break

challenge = None
while True:
	challenge = raw_input('Enter challenge in HEX [32 bytes] > ')#'5344083BEC29246681839912310C36B1BD1A259382FFF4560776DC5C953F7874'#
	if challenge != "":
		break

random = None
while True:
	random = raw_input('Enter random in HEX [32 bytes] > ')#'53440816EC29246681839912310C36B1BD1A259382FFF4560776DC5C953F7874'
	if random != "":
		break

fname = None
while True:
	fname = raw_input('Enter path for output master log file > ')#r'/home/ema/jacorb-3.3/demo/hello/master0.txt'#
	if fname != "":
		break

f = open(fname,"w")

master = None
if tp == "tls10":
	master = computeMasterTLS(challenge,random,"0301"+premaster)
else:
	master = computeMasterSSL(challenge,random,"0300"+premaster)

f.write("RSA Session-ID:"+sessionid+" Master-Key:"+master+"\n")
f.flush()
f.close()

print
print "[!] Find master log file here -",fname
print "[GOOD LUCK]"

