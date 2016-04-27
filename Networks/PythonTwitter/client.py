import socket
import sys
import threading
from thread import*

menu_lock = threading.Lock()
menu_bit = False
packet = ""

def get_packet():
    global menu_bit
    while not menu_bit:
        pass
    menu_packet = packet
    menu_bit = False
    return menu_packet

def print_messages():
    response = get_packet()
    i=0
    while response!='end':
        owner=response[:response.find(' ')]
        response = response[response.find(' ')+1:]
        message = response
        print 'Message #'+str(i)
        print 'User: '+owner
        print "\""+message+"\""
        response = get_packet()
        i = i + 1
    if i==0:
        print 'No messages to display'

def menu(conn):
    authenticated = False
    while not authenticated:
        name = raw_input("Login: ")
        password = raw_input("Password: ")
        cred = name+' '+password
        conn.sendall(cred)
        response = get_packet()
        if response[:6] == 'passed':
            print 'Hi '+name+', you have '+response[7:]+' unread messages.'
            authenticated = True
    log_in = True
    while log_in:
        c= raw_input("""Main Menu:
             -See Offline Messages (s)
             -Edit Subscriptions (e)
             -Post a Message (p)
             -Search by Tag (t)
             -Logout (l)
             """)
        if c=='s':
            c = raw_input("""Would you like to: 
            -Show all offline messages? (a)
            -Show messages from subscriptions? (s)
            -Go back (b)
            """)
            if c=='a':
                conn.sendall("1")
                print_messages()
            elif c=='s':
                conn.sendall("2")
                response = get_packet()
                subscribed = response.split()
                print('Subscriptions:')
                for user in subscribed:
                    print(user)
                request = raw_input('Type a space deliminated list' + \
                ' of subscriptions to print:')
                request_list = request.split()
                for user in request_list:
                    if not (user in subscribed):
                        print user+' is not a valid choice.'
                request_list[:] = [user for user in request_list \
                if (user in subscribed)]
                request = "3"+" ".join(request_list)
                conn.sendall(request)
                print_messages()
        elif c=='e':
            c = raw_input("""Would you like to:
            -Add subscription (a)
            -Remove subscription (r)
            -Go back (b)""")
            if c=='a':
                user=raw_input("Username to add:")
                conn.sendall("4"+user)
                response=get_packet()
                if response=="passed":
                    print "Now subscribed to "+user
                elif response=="failed":
                    print "User, \""+user+"\" does not exist or is already a subscription"
            elif c=='r':
                conn.sendall("2")
                response = get_packet()
                subscribed = response.split()
                print('Subscriptions:')
                for user in subscribed:
                    print(user)
                user = raw_input("Enter user to unsubscribe from:")
                conn.sendall("5"+user)
                response=get_packet()
                if response=="passed":
                    print "Now unsubscribed from "+user
                elif response=="failed":
                    print "You are not subscribed to \""+user+"\""

        elif c=='p':
            message = raw_input("Enter message:")
            msg_len = len(message)
            if msg_len<=140:
                tags = []
                tag = raw_input("Enter tags for post (\"end\" to stop):")
                while tag!="end":
                    tags.append(tag)
                    tag = raw_input("Enter tags for post (\"end\" to stop):")
                post = "6"+str(msg_len)+" "+message+" ".join(tags)
                conn.sendall(post)
            else:
                print "Message length exceeds 140 characters, returning to menu"
        elif c=='t':
            tag = raw_input("Enter tag to search:")
            conn.sendall("7"+tag)
            print_messages()
        elif c=='l':
            conn.sendall("8")
            log_in = False
        else:
            print 'Invalid input'

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
    print 'Error creating socket:' + str(msg[1])
    sys.exit()
    
server_ip = "127.0.0.1"

s.connect((server_ip, 9957))

#menu_lock.acquire()
start_new_thread(menu, (s,))

while 1:
    buff = s.recv(2048)
    
    while len(buff) > 0:
        packet = buff[:buff.find("\n")]
        buff = buff[buff.find("\n")+1:]
        if packet[0] == '1':
            print packet[1:]
            owner=packet[:packet.find(' ')]
            packet = packet[packet.find(' ')+1:]
            message = packet
            print 'New Message!'
            print 'User: '+owner
            print "\""+message+"\""
        else:
            packet = packet[1:]
            menu_bit = True
            #menu_lock.release()
            while menu_bit:
                pass
            #menu_lock.acquire()

s.close()

