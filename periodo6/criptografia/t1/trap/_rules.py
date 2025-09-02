alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!;@().,[] ?:\"'*-#/\n"

rules = {
    '0': 'YA',
    '1': 'YV',
    '2': 'YM',
    '3': 'ZB',
    '4': 'ZF',
    '5': 'YG',
    '6': 'ZR',
    '7': 'ZC',
    '8': 'ZY',
    '9': 'YT',
    'y': 'YY',
    'z': 'ZZ',
    'j': 'YZ',
    'J': 'YK',
    'x': 'ZT',
    'X': 'YC',
    'w': 'ZA',
    '!': 'YB',
    ';': 'ZU',
    '@': 'YD',
    '(': 'ZE',
    ')': 'YF',
    '.': 'ZG',
    ',': 'YH',
    '[': 'ZL',
    ']': 'YE',
    ' ': 'ZK',
    '?': 'YL',
    ':': 'ZN',
    '"': 'ZO',
    "'": 'YP',
    '*': 'ZQ',
    '-': 'YR',
    '#': 'ZS',
    '/': 'ZP',  
    '\n': 'YU',
}

def get_rules(mode):
    if mode == "encrypt":
        return rules
    else: # decrypt
        return {code: c for c, code in rules.items()}