# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

import socket
import package
import blackjack
import deck

from package import PLAYER1_ID, PLAYER2_ID, PLAYER3_ID
from addresses import DEALER_ADDRESS, PLAYER1_ADDRESS, PLAYER2_ADDRESS, PLAYER3_ADDRESS

def nextPlayerId(ids, current_id):
    n_ids = len(ids)

    current_index = ids.index(current_id)
    next_index = (current_index + 1) % n_ids

    return ids[next_index]

def sendResult(socket_udp, next_machine, dealer, player_id):
    hands = dealer.players[player_id].hands   

    hand1_bet = dealer.bets[player_id][0]
    hand2_bet = dealer.bets[player_id][1]
    hand1_result = blackjack.handWinner(dealer, hands[0]) if dealer.surrender[player_id] == 0 else 0 
    hand2_result = blackjack.handWinner(dealer, hands[1]) if dealer.split[player_id] == 1 else None

    payout = blackjack.playerPayout(hand1_bet, hand1_result, hand2_bet, hand2_result)
    dealer.players[player_id].bankroll += payout
    result = blackjack.blackjackResult(hand1_result, hand2_result)

    socket_udp.sendto(dealer.resultPackage(dealer.players[player_id], payout, result), next_machine)

def dealerRunner(socket_udp, dealer, next_machine):
    while True:
        pack, _ = socket_udp.recvfrom(package.MAX_BUFFER_SIZE)

        player_id, _, op, bet, hand, _, _ = package.readPackage(pack)

        if op == package.OP.BET.value:
            if len(dealer.players) == 1 and bet == 0:
                del dealer.players[player_id]
                break

            ids = list(dealer.players.keys())
            next_player = nextPlayerId(ids, player_id)

            if player_id == ids[-1]:
                socket_udp.sendto(dealer.actionPackage(dealer.players[next_player]), next_machine)
            else:
                dealer.players[next_player].hands[0].extend(deck.getCards(dealer.round_deck, 2))
                socket_udp.sendto(dealer.cardsPackage(dealer.players[next_player], dealer.players[next_player].hands[0]), next_machine)

            if bet != 0:
                dealer.bets[player_id][0] = bet
            else:
                del dealer.players[player_id]

        elif op == package.OP.HIT.value or op == package.OP.DOUBLE.value or op == package.OP.SPLIT.value:
            if op == package.OP.DOUBLE.value:
                dealer.bets[player_id][0] *= 2
            elif op == package.OP.SPLIT.value:
                dealer.split[player_id] = 1
                dealer.bets[player_id][1] = bet
                dealer.players[player_id].hands[1].append(dealer.players[player_id].hands[0].pop())

            new_card = deck.getCards(dealer.round_deck, 1)
            dealer.players[player_id].hands[hand] += new_card
            socket_udp.sendto(dealer.cardsPackage(dealer.players[player_id], new_card), next_machine)
 
        elif op == package.OP.STAND.value or op == package.OP.SURRENDER.value:
            if op == package.OP.SURRENDER.value:
                dealer.bets[player_id][0] //= 2
                dealer.surrender[player_id] = 1

            if dealer.split[player_id] == 1 and hand == 0:
                socket_udp.sendto(dealer.actionPackage(dealer.players[player_id]), next_machine)
            else:
                ids = list(dealer.players.keys())
                next_player = nextPlayerId(ids, player_id)

                if player_id == ids[-1]:
                    sendResult(socket_udp, next_machine, dealer, next_player)
                else:
                    socket_udp.sendto(dealer.actionPackage(dealer.players[next_player]), next_machine)

        elif op == package.OP.CONFIRM_RESULT.value:
            ids = list(dealer.players.keys())
            next_player = nextPlayerId(ids, player_id)

            if player_id == ids[-1]:
                dealer.reset()
                dealer.hand.extend(deck.getCards(dealer.round_deck, 2))
                dealer.players[next_player].hands[0].extend(deck.getCards(dealer.round_deck, 2))
                socket_udp.sendto(dealer.cardsPackage(dealer.players[next_player], dealer.players[next_player].hands[0]), next_machine)
            else:
                sendResult(socket_udp, next_machine, dealer, next_player)

            if not dealer.players[player_id].bankroll:
                del dealer.players[player_id]

        else: # op == package.OP.ERROR.value:
            if len(dealer.players) == 1:
                del dealer.players[player_id]
                break

            ids = list(dealer.players.keys())
            next_player = nextPlayerId(ids, player_id)

            if player_id == ids[-1]:
                sendResult(socket_udp, next_machine, dealer, next_player)
            else:
                socket_udp.sendto(dealer.actionPackage(dealer.players[next_player]), next_machine)

            del dealer.players[player_id]

def main():
    socket_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    socket_udp.bind(DEALER_ADDRESS)

    dealer = blackjack.Dealer(DEALER_ADDRESS)
    dealer.players[0] = blackjack.Player(PLAYER1_ID, PLAYER1_ADDRESS)
    dealer.players[1] = blackjack.Player(PLAYER2_ID, PLAYER2_ADDRESS)
    dealer.players[2] = blackjack.Player(PLAYER3_ID, PLAYER3_ADDRESS)

    next_machine = PLAYER1_ADDRESS

    dealer.hand.extend(deck.getCards(dealer.round_deck, 2))
    dealer.players[0].hands[0].extend(deck.getCards(dealer.round_deck, 2))
    socket_udp.sendto(dealer.cardsPackage(dealer.players[0], dealer.players[0].hands[0]), next_machine)

    dealerRunner(socket_udp, dealer, next_machine)

    socket_udp.sendto(dealer.gameOverPackage(PLAYER3_ADDRESS), next_machine)
    socket_udp.sendto(dealer.gameOverPackage(PLAYER2_ADDRESS), next_machine)
    socket_udp.sendto(dealer.gameOverPackage(PLAYER1_ADDRESS), next_machine)

if __name__ == "__main__":
    main()
