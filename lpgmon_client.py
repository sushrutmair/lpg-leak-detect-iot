import requests
import datetime
import time
import logging
from colorama import init, Back, Fore, Style

#init state. Change these two per your liking.
freq 	= 2 		#request frequency in seconds
period 	= 2	 	#total request period in hours
#init state end.

path = 'your path to csv file'
log_file = open(path,'w')

#calculate iterations
period_in_sec = period * 60 * 60
iterations = int(period_in_sec / freq)

print('|| ',datetime.datetime.now(), ' || Starting monitoring for: ', period, ' HRS. ', iterations, ' iterations')

#Starting...

init()

print(Back.GREEN)
print(Fore.BLACK)

for i in range(iterations):
 time.sleep(freq)
 r = requests.get('http://your Node MCU's IP/lpglvl')
 if(r.status_code == 200):
  s = r.text
  s += ','
  s += datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
  print(s)
  s += '\n'
  log_file.write(s)

print(Style.RESET_ALL)
print('|| ',datetime.datetime.now(), '|| ', iterations, ' iterations completed.')

