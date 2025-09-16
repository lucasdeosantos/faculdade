from ._utils import get_subkeys, caesar_cipher

def _transpose(text, num_columns):
    rows = [text[i:i+num_columns] for i in range(0, len(text), num_columns)]
    transposed = []

    for col in range(num_columns):
        for row in rows:
            if col < len(row):
                transposed.append(row[col])

    return ''.join(transposed)

def encrypt(key, text):
    subkeys = get_subkeys(key)

    text = _transpose(text, len(subkeys))

    for subkey in subkeys:
        text = caesar_cipher(text, len(subkey))

    return text