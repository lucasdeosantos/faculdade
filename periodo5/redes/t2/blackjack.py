# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

import package
import deck

from addresses import DEALER_ADDRESS

BLACKJACK = 21
DEALER_MIN_SCORE = 17

class Player:
    def __init__(self, id, address):
        self.address = address
        self.id = id
        self.hands = [[], []]
        self.bankroll = 1000

    def reset(self):
        self.hands = [[], []]

    def betPackage(self, bet):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.BET.value, bet)

    def hitPackage(self, hand=0):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.HIT.value, result_hand=hand)

    def standPackage(self, hand=0):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.STAND.value, result_hand=hand)

    def surrenderPackage(self):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.SURRENDER.value)

    def doublePackage(self):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.DOUBLE.value)

    def splitPackage(self, bet):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.SPLIT.value, bet)

    def confirmResultPackage(self):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.CONFIRM_RESULT.value)

    def errorPackage(self):
        return package.buildPackage(self.id, DEALER_ADDRESS, package.OP.ERROR.value)

class Dealer():
    def __init__(self, address):
        self.address = address
        self.players = {}
        self.hand = []
        self.surrender = [0, 0, 0]
        self.split = [0, 0, 0]
        self.bets = [[0, 0], [0, 0], [0, 0]]
        self.round_deck = deck.createDeck()

    def reset(self):
        for player in self.players.values():
            player.reset()

        self.hand = []
        self.surrender = [0, 0, 0]
        self.split = [0, 0, 0]
        self.bets = [[0, 0], [0, 0], [0, 0]]
        self.round_deck = deck.createDeck()

    def cardsPackage(self, player, cards):
        return package.buildPackage(0, player.address, package.OP.CARDS.value, n_cards=1 + len(cards), cards=[self.hand[0]] + cards)

    def actionPackage(self, player):
        return package.buildPackage(0, player.address, package.OP.ACTION.value)

    def resultPackage(self, player, payout, result):
        return package.buildPackage(0, player.address, package.OP.RESULT.value, bet_payout=payout, n_cards=len(self.hand), cards=self.hand, result_hand=result)

    def gameOverPackage(self, player_address):
        return package.buildPackage(0, player_address, package.OP.GAME_OVER.value)

def cardValue(card):
    rank = card % 13
    if rank == 0:
        return 1
    if rank >= 10:
        return 10
    return rank + 1

def calculateHand(hand):
    total = 0
    aces = 0

    for card in hand:
        value = cardValue(card)
        total += value
        if value == 1:
            aces += 1

    while aces > 0 and total + 10 <= BLACKJACK:
        total += 10
        aces -= 1

    return total

def handWinner(dealer, player_hand):
    player_points = calculateHand(player_hand)
    
    if player_points == BLACKJACK:
        return 1

    if player_points > BLACKJACK:
        return 0
    
    dealer_points = calculateHand(dealer.hand)
    while dealer_points < DEALER_MIN_SCORE:
        new_card = deck.getCards(dealer.round_deck, 1)[0]
        dealer.hand.append(new_card)
        dealer_points = calculateHand(dealer.hand)
    
    if dealer_points > BLACKJACK:
        return 1

    return 1 if player_points >= dealer_points else 0

def blackjackResult(hand1_result, hand2_result):
    if hand1_result == 0 and hand2_result == None:
        return 0
    if hand1_result == 1 and hand2_result == None:
        return 1
    if hand1_result == 0 and hand2_result == 0:
        return 2
    if hand1_result == 1 and hand2_result == 0:
        return 3
    if hand1_result == 0 and hand2_result == 1:
        return 4
    return 5

def playerPayout(hand1_bet, hand1_result, hand2_bet, hand2_result):
    payout_hand1 = hand1_bet if hand1_result == 1 else -hand1_bet
    payout_hand2 = hand2_bet if hand2_result == 1 else -hand2_bet if hand2_result == 0 else 0
    return payout_hand1 + payout_hand2
