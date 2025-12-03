import socket
import sys
import os
from config import NUM_REPLICAS, TAM_MAX


def upload(sockfd, client_id, filepath):
    if not os.path.isfile(filepath):
        print("[ERROR] arquivo não existe.")
        return

    with open(filepath, "rb") as f:
        data = f.read()

    filesize = len(data)
    filename = os.path.basename(filepath)

    header = f"UPLOAD {client_id} {filename} {filesize}"
    sockfd.send(header.encode())

    response = sockfd.recv(TAM_MAX).decode()
    
    if response == "SEND_FILE":
        print(f"Enviando arquivo '{filename}'...")
        for i in range(0, filesize, TAM_MAX):
            buffer = data[i:i+TAM_MAX]
            sockfd.send(buffer)

        response = sockfd.recv(TAM_MAX).decode()
        if response == "UPLOAD_SUCCESS":
            print(f"Envio concluído. Arquivo replicado com sucesso para {NUM_REPLICAS} servidores réplica.")
        else:
            print(f"[ERROR] falha ao salvar arquivo '{filename}' no servidor.")
    else:
        print(f"[ERROR] falha no envio do arquivo '{filename}'.")


def list(sockfd, client_id):
    header = f"LIST {client_id}"
    sockfd.send(header.encode())

    response = sockfd.recv(TAM_MAX).decode()
    
    print(response)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(f"Uso correto: {sys.argv[0]} <cliente_id> <host> <porta>")
        sys.exit(1)

    client_id = int(sys.argv[1])
    host = sys.argv[2]
    port = int(sys.argv[3])

    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sockfd.connect((host, port))
    print("Conexão criada com o servidor primário.")

    while True:
        user_input = input("> ").strip()
        
        if user_input == "exit":
            break

        args = user_input.split()
        cmd = args[0]

        if cmd == "upload":
            if len(args) != 2:
                print("[ERROR] número inválido de argumentos para upload.")
                print("Uso correto: upload <nome_arquivo>")
            else:
                filepath = args[1]
                filepath = os.path.expanduser(filepath)

                upload(sockfd, client_id, filepath)

        elif cmd == "list":
            list(sockfd, client_id)

        else:
            print("[ERROR] Operação inválida.")

    sockfd.close()