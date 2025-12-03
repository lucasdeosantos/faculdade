from .utils import get_subkeys, caesar_cipher

def untranspose(key, text):
    num_columns = len(key)
    num_rows = len(text) // num_columns
    
    ordered_indexes = sorted(range(num_columns), key=lambda i: key[i])
    
    cols = []
    start = 0
    for _ in range(num_columns):
        cols.append(text[start:start + num_rows])
        start += num_rows
    
    untransposed_text = []
    for row in range(num_rows):
        for col in range(num_columns):
            original_col_index = ordered_indexes.index(col)
            untransposed_text.append(cols[original_col_index][row])
    
    return "".join(untransposed_text).rstrip('\x00')


def decrypt(key, text):
    subkeys = get_subkeys(key)[::-1]

    for subkey in subkeys:
        text = caesar_cipher(text, len(subkey), decrypt=True)
        text = untranspose(subkey, text)

    return text