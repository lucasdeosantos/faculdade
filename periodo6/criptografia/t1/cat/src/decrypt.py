from .utils import get_subkeys, caesar_cipher

def untranspose(key, text):
    num_columns = len(key)
    num_rows = len(text) // num_columns
    
    ordered_indexes = sorted(range(num_columns), key=lambda i: key[i])
    
    col_lengths  = [num_rows] * num_columns
    
    cols = []
    start = 0
    for length in col_lengths:
        cols.append(text[start:start + length])
        start += length
    
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
        text = untranspose(text, subkey)

    return text