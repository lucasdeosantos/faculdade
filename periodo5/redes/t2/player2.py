# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

import socket
import package
import blackjack
import deck

from package import PLAYER2_ID
from addresses import PLAYER2_ADDRESS, PLAYER3_ADDRESS

MAX_ERROR = 3

def play():
    print("\n========================================================")
    for _ in range(MAX_ERROR):
        choice = input("Você quer jogar uma rodada de Blackjack? [y/n]: ").lower()
        if choice in ['y', 'n']:
            return 1 if choice == 'y' else 0
        print("Entrada inválida. Por favor, digite 'y' para sim ou 'n' para não.")

    print("Número máximo de tentativas atingido. Você está fora do jogo.")
    return 0

def placeBet(player, first_bet=0):
    available_bankroll = player.bankroll - first_bet
    for _ in range(MAX_ERROR):
        try:
            bet = int(input(f"Você tem {available_bankroll} fichas disponíveis. Faça sua aposta: "))
            if 0 < bet <= available_bankroll:
                return bet
            print("Aposta inválida. Tente novamente.")
        except ValueError:
            print("Entrada inválida. Por favor, insira um número.")

    print("Número máximo de tentativas atingido. Você está fora do jogo.")
    return 0

def actions(socket_udp, player, next_machine, prompt="Escolha uma opção [1-2]: ", hand=0, first=0, bet=0):
    print("\nEscolha uma jogada:")
    print("1. Hit (comprar carta)")
    print("2. Stand (manter a mão)")
    if first == 1:
        print("3. Surrender (desistir da mão)")
        print("4. Double (dobrar a aposta e comprar uma carta)")
        print("5. Split (criar dois jogos diferentes a partir de duas cartas iguais)")

    for _ in range(MAX_ERROR):
        try:
            choice = int(input(prompt))
            if choice == 1:
                socket_udp.sendto(player.hitPackage(hand), next_machine)
                return 0
            if choice == 2:
                socket_udp.sendto(player.standPackage(hand), next_machine)
                return 0
            if choice == 3 and first == 1:
                socket_udp.sendto(player.surrenderPackage(), next_machine)
                return 0
            if choice == 4 and first == 1 and player.bankroll > bet * 2:
                socket_udp.sendto(player.doublePackage(), next_machine)
                return 0
            if choice == 5 and first == 1 and deck.equalRank(player.hands[0][0], player.hands[0][1]) and player.bankroll - bet != 0:
                player.hands[1].append(player.hands[0].pop())
                new_bet = placeBet(player, bet)
                if new_bet:
                    socket_udp.sendto(player.splitPackage(new_bet), next_machine)
                    return 1
                else:
                    socket_udp.sendto(player.errorPackage(), next_machine)
                    return 0
            print("Opção inválida. Por favor, escolha novamente.")
        except ValueError:
            print("Entrada inválida. Por favor, insira um número.")

    print("Número máximo de tentativas atingido. Você está fora do jogo.")
    socket_udp.sendto(player.errorPackage(), next_machine)
    return 0

def formatHand(hand):
    return ", ".join(deck.getCardName(card) for card in hand)

def displayHands(player, dealer_hand):
    print("\nMão do Dealer: " + formatHand(dealer_hand))
    if not player.hands[1]:
        print("Sua Mão: " + formatHand(player.hands[0]))
    else:
        print("Primeira Mão: " + formatHand(player.hands[0]))
        print("Segunda Mão: " + formatHand(player.hands[1]))

def displayResult(result):
    if result == 0:
        print("\nDealer venceu!")
    elif result == 1:
        print("\nVocê venceu!")
    elif result == 2:
        print("\nDealer venceu as duas mãos!")
    elif result == 3:
        print("\nVocê venceu a primeira mão!")
    elif result == 4:
        print("\nVocê venceu a segunda mão!")
    else:
        print("\nVocê venceu as duas mãos!")
    
def playerRunner(socket_udp, player, next_machine):
    hand, bet, split, first, dealer_card = 0, 0, 0, 1, None
    while True:
        pack, _ = socket_udp.recvfrom(package.MAX_BUFFER_SIZE)

        _, destination_address, op, payout, result, _, cards = package.readPackage(pack)

        if(destination_address != PLAYER2_ADDRESS):
            socket_udp.sendto(pack, next_machine)
        else:
            if op == package.OP.CARDS.value:
                if not player.hands[0]:
                    if play():
                        bet = placeBet(player)
                    dealer_card = cards[0]
                    player.hands[0].extend(cards[1:])
                    socket_udp.sendto(player.betPackage(bet), next_machine)
                else:
                    player.hands[hand].append(cards.pop())
                    displayHands(player, [dealer_card])
                    if blackjack.calculateHand(player.hands[hand]) >= 21:
                        socket_udp.sendto(player.standPackage(hand), next_machine)
                        if split == 1 and hand == 0:
                            hand = 1
                    else:
                        actions(socket_udp, player, next_machine, hand=hand)

            elif op == package.OP.ACTION.value:
                displayHands(player, [dealer_card])
                if blackjack.calculateHand(player.hands[0]) >= 21 and split == 0:
                    socket_udp.sendto(player.standPackage(), next_machine)
                else:
                    if first == 1:
                        split = actions(socket_udp, player, next_machine, "Escolha uma opção [1-5]: ", first=first, bet=bet)
                        first = 0
                    else:
                        if split == 1 and hand == 0:
                            hand = 1
                        actions(socket_udp, player, next_machine, hand=hand)

            elif op == package.OP.RESULT.value:
                displayHands(player, cards)
                displayResult(result)
                player.bankroll += payout
                print(f"\nVocê possui {player.bankroll} fichas.")
                hand, bet, split, first, dealer_card = 0, 0, 0, 1, None
                player.reset()
                socket_udp.sendto(player.confirmResultPackage(), next_machine)

            else: # op == package.OP.GAME_OVER.value:
                break
 
def main():
    socket_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    socket_udp.bind(PLAYER2_ADDRESS)

    player = blackjack.Player(PLAYER2_ID, PLAYER2_ADDRESS)

    next_machine = PLAYER3_ADDRESS

    playerRunner(socket_udp, player, next_machine)

if __name__ == "__main__":
    main()
