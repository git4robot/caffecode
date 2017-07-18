
import os

fp = open('log.txt', 'r')
lastlin = ''
icnt = 0
for lin in fp:
	tmplin = lin[50:58]
	icnt = icnt + 1	
	if icnt == 1:
		lastlin = tmplin
		
	if icnt >= 6:
		print lin
	
	if tmplin != lastlin:
		icnt = 0
		