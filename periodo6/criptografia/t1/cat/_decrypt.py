from ._utils import get_subkeys, caesar_cipher

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


def decrypt(key, text):
    subkeys = get_subkeys(key)[::-1]

    for subkey in subkeys:
        text = caesar_cipher(text, len(subkey), decrypt=True)

    text = _untranspose(text, len(subkeys))

    return text