import socket
import sys
import os
from config import TAM_MAX, TAM_FILA, BASE_PORT, REPLICAS_PORTS
from servidor import save_file


def handle_server_request(connfd, replica_dir):
    while True:
        buffer = connfd.recv(TAM_MAX).decode()

        if not buffer:
            break

        args = buffer.split()
        cmd = args[0]

        if cmd == "REPLICA":
            client_id = args[1]
            filename = args[2]
            filesize = int(args[3])

            connfd.send(b"SEND_FILE")

            data = b""
            while len(data) < filesize:
                data += connfd.recv(TAM_MAX)

            file_saved = save_file(replica_dir, client_id, filename, data)

            if file_saved:
                connfd.send(b"REPLICA_SUCCESS")
            else:
                connfd.send(b"REPLICA_FAILURE")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Uso correto: {sys.argv[0]} <porta>")
        sys.exit(1)

    port = int(sys.argv[1])

    if port not in REPLICAS_PORTS:
        print(f"[ERROR] Porta inválida para servidor réplica. Use uma das seguintes portas: {REPLICAS_PORTS}")
        sys.exit(1)

    replica_dir = f"replica{(port - BASE_PORT) + 1}"
    os.makedirs(replica_dir, exist_ok=True)

    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    sockfd.bind(('', port))
    sockfd.listen(TAM_FILA)

    while True:
        connfd, addr = sockfd.accept()
        handle_server_request(connfd, replica_dir)
        connfd.close()