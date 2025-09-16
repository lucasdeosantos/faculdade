UNICODE_LIMIT = 0x110000
MAX_SUBKEYS = 17

def caesar_cipher(text, shift, decrypt=False):
    if decrypt:
        shift = -shift

    processed_chars = []
    for char in text:
        original_code = ord(char)

        shifted_code = (original_code + shift) % UNICODE_LIMIT
        processed_chars.append(chr(shifted_code))

    return "".join(processed_chars)

def get_subkeys(key):
    return key.strip().split()[:MAX_SUBKEYS]