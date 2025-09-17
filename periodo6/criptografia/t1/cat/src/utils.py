UNICODE_LIMIT = 0x110000
MAX_SUBKEYS = 10
DEFAULT_KEY = "how many lives does a cat have?"

def caesar_cipher(text, shift, decrypt=False):
    shift = shift + 9

    if decrypt:
        shift = -shift

    processed_chars = []
    for char in text:
        original_code = ord(char)

        shifted_code = (original_code + shift) % UNICODE_LIMIT
        processed_chars.append(chr(shifted_code))

    return "".join(processed_chars)


def get_subkeys(key):
    key = key.strip()

    if not key:
        key = DEFAULT_KEY

    return key.split()[:MAX_SUBKEYS]