from ._rules import get_rules
from ._utils import get_subkeys
from ._playfair import playfair

def _apply_rules(text):
    rules = get_rules("encrypt")
    result = []

    for c in text:
        if c in rules:
            result.append(rules[c])
        elif c.isupper():
            result.append(f"W{c}")
        elif c.islower():
            result.append(c.upper())

    return ''.join(result)

def _transpose(text, num_columns):
    rows = [text[i:i+num_columns] for i in range(0, len(text), num_columns)]
    transposed = []

    for col in range(num_columns):
        for row in rows:
            if col < len(row):
                transposed.append(row[col])

    return ''.join(transposed)

def _add_pad_char(text, pad_char='X'):
    processed_chars = []

    i = 0
    while i < len(text):
        current_char = text[i]
        processed_chars.append(current_char)

        if i + 1 < len(text):
            next_char = text[i+1]

            if current_char == next_char:
                processed_chars.append(pad_char)
                i += 1
            else:
                processed_chars.append(next_char)
                i += 2
        else:
            i += 1

    if len(processed_chars) % 2 != 0:
        processed_chars.append(pad_char)

    return ''.join(processed_chars)

def encrypt(key, text):
    return text
    subkeys = get_subkeys(key)

    text = _apply_rules(text)
    text = _transpose(text, len(subkeys))
    text = _add_pad_char(text)

    for subkey in subkeys:
        text = playfair(subkey, text, direction=1)

    return text