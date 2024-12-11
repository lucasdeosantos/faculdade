# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

import socket

BASE_PORT = 5000

def getLocalIP():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            s.connect(("8.8.8.8", 80))
            ip = s.getsockname()[0]
        return ip
    except Exception:
        return "127.0.0.1"

def initializeAddresses(base_port):
    local_ip = getLocalIP()
    dealer_address = (local_ip, base_port)
    player1_address = (local_ip, base_port + 1)
    player2_address = (local_ip, base_port + 2)
    player3_address = (local_ip, base_port + 3)

    return {
        "DEALER_ADDRESS": dealer_address,
        "PLAYER1_ADDRESS": player1_address,
        "PLAYER2_ADDRESS": player2_address,
        "PLAYER3_ADDRESS": player3_address,
    }

addresses = initializeAddresses(BASE_PORT)
DEALER_ADDRESS = addresses["DEALER_ADDRESS"]
PLAYER1_ADDRESS = addresses["PLAYER1_ADDRESS"]
PLAYER2_ADDRESS = addresses["PLAYER2_ADDRESS"]
PLAYER3_ADDRESS = addresses["PLAYER3_ADDRESS"]
