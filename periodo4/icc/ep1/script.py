import subprocess

# Nome do arquivo de entrada
input_file = '../sistemas.dat'

# Abrir o arquivo de entrada
with open(input_file, 'r') as f:
    # Ler todas as linhas do arquivo e remover espaços em branco no início e no final
    lines = [line.strip() for line in f.readlines()]

# Lista para armazenar as entradas
entries = []
current_entry = []

# Processar cada linha do arquivo
for line in lines:
    if line:  # Se a linha não estiver em branco, adicionar à entrada atual
        current_entry.append(line)
    elif current_entry:  # Se encontramos uma linha em branco e há uma entrada atual
        entries.append(current_entry)  # Adicionar a entrada à lista de entradas
        current_entry = []  # Reiniciar a entrada atual

# Adicionar a última entrada, se houver
if current_entry:
    entries.append(current_entry)

# Para cada entrada, chamar o script em Bash com redirecionamento de entrada
for i, entry in enumerate(entries, 1):
    # Montar o comando para chamar o script Bash com redirecionamento de entrada
    bash_command = './script.sh'

    # Criar uma string com a entrada para redirecionamento
    input_string = '\n'.join(entry)

    print(f"------------------------------Input {i}------------------------------")
    # Executar o comando Bash com redirecionamento de entrada
    subprocess.run(bash_command, input=input_string, text=True, shell=True) 
