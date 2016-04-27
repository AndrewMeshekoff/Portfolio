import socket
import sys
from thread import*

class Message:
    def __init__(self, text, tags, owner):
        self.text = text
        self.tags = tags
        self.owner = owner

class User:
    def __init__(self, name, password, subscribed, unread):
        self.name = name
        self.password = password
        self.subscribed = subscribed
        self.unread = unread
        self.conn = None
        self.status = "offline"
    

accounts = []
accounts.append(User('trixy', '1234', ['steve', 'joe'], []))
accounts.append(User('steve', '5678', ['joe'], []))
accounts.append(User('joe', 'abcd', ['trixy'], []))

messages = []
myList = []

def send_protocol(conn, msg):
    conn.sendall(msg+"\n")

def send_msg(i,conn):
    response="0"+messages[i].owner+" "+messages[i].text
    send_protocol(conn, response)

def serverterminal():
    while 1:
        choice = raw_input("")
        if choice == "messagecount":
            print len(messages)

def client_thread(conn):
    verified = False
    while not verified:
        myList.append(conn)
        cred = conn.recv(2048)
        c_name = cred[:cred.find(' ')]
        c_pass = cred[cred.find(' ')+1:]
        verify = [account for account in accounts \
            if account.name == c_name and account.password == c_pass]
        if verify:
            msg_count =  str(len(verify[0].unread))
            send_protocol(conn, "0passed "+msg_count)
            verified = True
        else:
            send_protocol(conn, "0failed")
    
    acc_id = accounts.index(verify[0])
    exit = False
    accounts[acc_id].conn = conn
    accounts[acc_id].status = "online"
    while not exit:
        action = conn.recv(2048)
        if action[0] == '1':
            for msg in accounts[acc_id].unread:
                send_msg(msg, conn)
            send_protocol(conn, "0end")
        elif action[0] == '2':
            send_protocol(conn, "0"+" ".join(accounts[acc_id].subscribed))
        elif action[0] == '3':
            sub=action[1:].split()
            for msg in accounts[acc_id].unread:
                if messages[msg].owner in sub:
                    send_msg(msg, conn)
            send_protocol(conn, "0end")
        elif action[0] == '4':
            user = action[1:]
            account=[account for account in accounts if ((account.name \
            == user) and (account.name not in accounts[acc_id].subscribed))]
            if account:
                accounts[acc_id].subscribed.append(user)
                send_protocol(conn, "0passed")
            else:
                send_protocol(conn, "0failed")
        elif action[0] == '5':
            user = action[1:]
            account=[account for account in accounts if ((account.name \
            == user) and (account.name in accounts[acc_id].subscribed))]
            if account:
                accounts[acc_id].subscribed.remove(user)
                send_protocol(conn, "0passed")
            else:
                send_protocol(conn, "0failed")
        elif action[0] == '6':
            message = action[1:]
            print action
            count = int(message[:message.find(' ')])
            message = message[message.find(' ')+1:]
            text = message[:count]
            tag_text = message[count:]
            tags = tag_text.split()
            msg_id = len(messages)
            messages.append(Message(text, tags, accounts[acc_id].name))
            for account in accounts:
                if accounts[acc_id].name in account.subscribed:
                    if account.status == "online":
                        send_protocol(account.conn, "1"+accounts[acc_id].name+" "+text)
                    elif account.status == "offline":
                        account.unread.append(msg_id)
        elif action[0] == '7':
            tag = action[1:]
            i = 0
            for message in reversed(messages):
                if tag in message.tags:
                    i = i + 1
                    response="0"+message.owner+" "+message.text
                    send_protocol(conn, response)
                    if i >= 10:
                        break
            send_protocol(conn, "0end")
        elif action[0] == '8':
            accounts[acc_id].status = "offline"
            accounts[acc_id].unread = []
            exit = True
    conn.close()



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(('', 9957))

s.listen(3)

start_new_thread(serverterminal, ())

while 1:
    conn, addr = s.accept()

    start_new_thread(client_thread ,(conn,))

conn.close()
s.close()
