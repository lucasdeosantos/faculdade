# Quantidade de servidores réplica
NUM_REPLICAS = 3

# Tamanho máximo do buffer de dados
TAM_MAX = 1024 

# Tamanho da fila de conexões
TAM_FILA = 5

# Porta base dos servidores réplica
BASE_PORT = 9000

# Portas dos servidores réplica
REPLICAS_PORTS = [BASE_PORT + i for i in range(NUM_REPLICAS)]