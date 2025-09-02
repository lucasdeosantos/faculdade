import re
import random

def _clean_key(key):
    return re.sub(r'[^a-zA-Z ]', '', key).upper().replace('J', 'I')

def get_subkeys(key):
    subkeys = _clean_key(key).strip().split()
    return subkeys[:10]

def get_random_i_j():
    return random.choice(['I', 'J'])