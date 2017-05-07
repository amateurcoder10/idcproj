import serial
import time
import urllib2
import os

myAPI = "LTBF1R2PUNZ4T657"
myDelay = 15 #how many seconds between posting data

port=serial.Serial("/dev/rfcomm1",baudrate=38400,timeout=300)
if(port.isOpen() == False):
    port.open()
port.flushInput()
port.flushOutput()
time.sleep(5)
baseURL ='https://api.thingspeak.com/update?api_key=%s' % myAPI
print baseURL

names=["Timestamp","Humidity","Temperature","Pressure","CO2","LDR","Intruder","Activity"]
print names
i=0
flag1=flag2=flag3=flag4=1
icount=0
counter=0
data=["0","0","0","0","0","0","0","0"]
while 1:
    data[i]=port.readline()
    data[i]=data[i].rstrip()
    print names[i] + "  :" + data[i]
    
    if(i>=7):
        i=-1
        print "******************************"
        counter=counter+1
        if((int(data[4])>200) & (flag1==1)):
            print "sending msg for co2"
            flag1=0
            os.system("python sms.py 1")
        if((int(data[5])>850) & (flag2==1)):
            print "sending msg for Light"
            flag2=0
            os.system("python sms.py 2")
        if((int(data[6])==1) & (flag3==1)):
            icount=icount+1
            if(icount>10):
                os.system("python sms.py 3")
                flag3=0
                print "Intruder alert"
                icount=0
        if((int(data[7])==0) & (flag4==1)):
            print "Robot is not moving..!"
            os.system("python sms.py 4")
            flag4=0
                
    if(counter>300):    
        f = urllib2.urlopen(baseURL +"&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s&field6=%s" % (str(data[0]),str(data[1]),str(data[2]),str(data[3]),str(data[4]),str(data[5])))
        flag1=flag2=flag3=flag4=1
        #print f.read()
        print "******************************"
        print "sending to cloud"
        print "******************************"
        counter=0
        
    i=i+1

port.close()
