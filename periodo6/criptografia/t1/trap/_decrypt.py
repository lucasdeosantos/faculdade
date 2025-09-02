from ._rules import get_rules
from ._utils import get_subkeys
from ._playfair import playfair

def _revert_rules(text):
    rules = get_rules("decrypt")
    result = []

    i = 0
    while i < len(text):
        c = text[i]
        if c == 'W':
            next_c = text[i + 1]
            if next_c == 'J':
                result.append('I')
            else:
                result.append(next_c)
            i += 2
        elif c in ('Y', 'Z'):
            code = c + text[i + 1]
            result.append(rules[code])
            i += 2
        else:
            char = c.lower()
            if char == 'j':
                char = 'i'
            result.append(char)
            i += 1
    
    return ''.join(result)

def _untranspose(text, num_columns):
    num_rows = (len(text) + num_columns - 1) // num_columns
    full_cols = len(text) % num_columns
    if full_cols == 0:
        full_cols = num_columns

    cols = []
    start = 0
    for col in range(num_columns):
        length = num_rows if col < full_cols else num_rows - 1
        cols.append(text[start:start+length])
        start += length

    result = []
    for row in range(num_rows):
        for col in range(num_columns):
            if row < len(cols[col]):
                result.append(cols[col][row])

    return ''.join(result)

def _remove_pad_char(text, pad_char='X'):
    return text.replace(pad_char, '')

def decrypt(key, text):
    subkeys = get_subkeys(key)[::-1]

    for subkey in subkeys:
        text = playfair(subkey, text, direction=-1)

    text = _remove_pad_char(text)
    text = _untranspose(text, len(subkeys))
    text = _revert_rules(text)

    return text