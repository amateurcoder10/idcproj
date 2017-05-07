import urllib2
import cookielib
from getpass import getpass
import sys
import os
from stat import *

alert=sys.argv[1]
message=""
if(alert=='1'):
    message="Alert:co2 levels are high.Possibility of a fire"
if(alert=='2'):
    message="Alert:lights are on in the warehouse"
if(alert=='3'):
    message="Alert:intruder detected"
if(alert=='4'):
    message="Irobot not moving.Please check batteries."

number = "9490451859"

#"9845592182"#9490451859"
#print message

if __name__ == "__main__":    
    username = "7207631105"
    passwd = "deepikaraj"

    message = "+".join(message.split(' '))
    #print message
 #logging into the sms site
    url ='http://site24.way2sms.com/Login1.action?'
    data = 'username='+username+'&password='+passwd+'&Submit=Sign+in'

 #For cookies

    cj= cookielib.CookieJar()
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))

 #Adding header details
    opener.addheaders=[('User-Agent','Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120')]
    try:
        usock =opener.open(url, data)
    except IOError:
        print "error"
        #return()

    jession_id =str(cj).split('~')[1].split(' ')[0]
    send_sms_url = 'http://site24.way2sms.com/smstoss.action?'
    send_sms_data = 'ssaction=ss&Token='+jession_id+'&mobile='+number+'&message='+message+'&msgLen=136'
    opener.addheaders=[('Referer', 'http://site25.way2sms.com/sendSMS?Token='+jession_id)]
    try:
        sms_sent_page = opener.open(send_sms_url,send_sms_data)
    except IOError:
        print "error"
        #return()

    print "success" 
    #return ()
