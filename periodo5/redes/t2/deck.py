# ========================================================================
# Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
# Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
# Date: 18/11/2024
# ========================================================================

import random

suits = ['♦', '♠', '♥', '♣']
ranks = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]

def createDeck():
    return list(range(52))

def getCards(deck, n_cards):
    selected = random.sample(deck, n_cards)

    for card in selected:
        deck.remove(card)

    return selected

def getCardName(card):
    return ranks[card % 13] + suits[card // 13]

def equalRank(card1, card2):
    return 1 if (card1 % 13) == (card2 % 13) else 0
