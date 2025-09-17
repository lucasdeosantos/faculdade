from .utils import get_subkeys, caesar_cipher

def transpose(key, text):
    num_columns = len(key)

    remainder = len(text) % num_columns
    if remainder != 0:
        padding_length = num_columns - remainder
        text += '\x00' * padding_length

    ordered_indexes = sorted(range(num_columns), key=lambda i: key[i])
    rows = [text[i:i+num_columns] for i in range(0, len(text), num_columns)]

    transposed_text = []
    for col_index in ordered_indexes:
        for row in rows:
            transposed_text.append(row[col_index])

    return "".join(transposed_text)


def encrypt(key, text):
    subkeys = get_subkeys(key)

    for subkey in subkeys:
        text = transpose(subkey, text)
        text = caesar_cipher(text, len(subkey))

    return text