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
        fname = conn.recv(1024)
        print("fname: ", fname)
        conn.send(fname)
        print("returned to client: ", fname)
        while True:
            data = conn.recv(1024)
            if not data:
                break
            else:
                print("writing some data")
                # we have file data
                with open(fname, 'ab') as f:
                    f.write(data)
            



if __name__ == "__main__":
    main()
