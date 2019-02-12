#!/usr/bin/env python3

import socket


def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('localhost',9875))
    server.listen(1)
    conn, addr = server.accept()

    fname = "tmp"
    with conn:
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)
            tmp = data.decode('utf-8')
            if not data:
                break
            if tmp[:1] == 'f':
                fname = str(data[1:], 'utf-8')
                fname = fname + ".recvd"
                conn.send(data[1:])
            else:
                print("writing some data")
                # we have file data
                with open(fname, 'a') as f:
                    f.write(tmp)
            



if __name__ == "__main__":
    main()
