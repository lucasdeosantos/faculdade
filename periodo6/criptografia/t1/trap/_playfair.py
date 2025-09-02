from ._utils import get_random_i_j

def _generate_key_square(key):
    alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"
    
    cipher_alphabet = []
    for char in key:
        if char not in cipher_alphabet:
            cipher_alphabet.append(char)
            
    for char in alphabet:
        if char not in cipher_alphabet:
            cipher_alphabet.append(char)
            
    return [cipher_alphabet[i:i + 5] for i in range(0, 25, 5)]

def _generate_char_map(key_square):
    char_map = {}
    
    for r_idx, row in enumerate(key_square):
        for c_idx, char in enumerate(row):
            char_map[char] = (r_idx, c_idx)
            
    char_map['J'] = char_map['I']
    
    return char_map

def _handle_i_j_output(char):
    if char == 'I':
        return get_random_i_j()
    
    return char

def _get_substitute_pair(pair, char_map, key_square, direction):
    p1 = 'I' if pair[0] == 'J' else pair[0]
    p2 = 'I' if pair[1] == 'J' else pair[1]
    
    r1, c1 = char_map[p1]
    r2, c2 = char_map[p2]

    if r1 == r2:
        new_c1 = (c1 + direction) % 5
        new_c2 = (c2 + direction) % 5
        new_char1 = key_square[r1][new_c1]
        new_char2 = key_square[r2][new_c2]
    elif c1 == c2:
        new_r1 = (r1 + direction) % 5
        new_r2 = (r2 + direction) % 5
        new_char1 = key_square[new_r1][c1]
        new_char2 = key_square[new_r2][c2]
    else:
        new_char1 = key_square[r1][c2]
        new_char2 = key_square[r2][c1]

    return _handle_i_j_output(new_char1) + _handle_i_j_output(new_char2)

def playfair(key, text, direction):
    key_square = _generate_key_square(key)
    char_map = _generate_char_map(key_square)

    digraphs = [text[i:i + 2] for i in range(0, len(text), 2)]

    processed_text = []
    for pair in digraphs:
        substitute_pair = _get_substitute_pair(pair, char_map, key_square, direction)
        processed_text.append(substitute_pair)
        
    return "".join(processed_text)