# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

from enum import Enum

MAX_BUFFER_SIZE = 48

PLAYER1_ID = 0
PLAYER2_ID = 1
PLAYER3_ID = 2

class OP(Enum):
    BET = 0
    CARDS = 1
    ACTION = 2
    HIT = 3
    STAND = 4
    SURRENDER = 5
    DOUBLE = 6
    SPLIT = 7
    RESULT = 8
    CONFIRM_RESULT = 9
    ERROR = 10
    GAME_OVER = 11

def buildPackage(id_origin, destination_address, op, bet_payout=0, result_hand=0, n_cards=0, cards=None):
    package = id_origin.to_bytes(1, 'big')
    
    ip, port = destination_address
    package += ip.encode('utf-8').ljust(15, b'\0')
    package += port.to_bytes(2, 'big')

    package += op.to_bytes(1, 'big')
    
    package += bet_payout.to_bytes(2, 'big', signed=True)

    package += result_hand.to_bytes(1, 'big')
   
    package += n_cards.to_bytes(1, 'big')

    if cards:
        for card in cards:
            package += card.to_bytes(1, 'big')

    return package

def readPackage(package):
    id_origin = package[0]
    
    ip = package[1:16].decode('utf-8').strip('\0')
    port = int.from_bytes(package[16:18], 'big')
    destination_address = (ip, port)

    op = package[18]

    bet_payout = int.from_bytes(package[19:21], 'big', signed=True)

    result_hand = package[21]

    n_cards = package[22]

    cards = [package[i] for i in range(23, 23 + n_cards)] if n_cards > 0 else []

    return id_origin, destination_address, op, bet_payout, result_hand, n_cards, cards
