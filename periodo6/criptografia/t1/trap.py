# T.R.A.P: TRAsposition + Playfair

import argparse
import sys
import re
import unicodedata
from pathlib import Path
from trap import encrypt, decrypt, alphabet

def clean_text(text):
    text = unicodedata.normalize('NFKD', text).encode('ASCII', 'ignore').decode('utf-8')
    text = re.sub(f'[^{re.escape(alphabet)}]', '', text)
    return text

def main():
    parser = argparse.ArgumentParser(
        description="Encrypts or decrypts a text file using the TRAP cipher.",
        epilog="USAGE: python trap.py [option] [key_file] [text_file]"
    )

    mode_group = parser.add_mutually_exclusive_group(required=True)
    mode_group.add_argument('-e', '--encrypt', action='store_true', help='Enable encryption mode.')
    mode_group.add_argument('-d', '--decrypt', action='store_true', help='Enable decryption mode.')
    parser.add_argument('key_file', help='Path to the .txt file containing the key phrase.')
    parser.add_argument('text_file',  help='Path to the .txt file containing the text to be processed.')
    args = parser.parse_args()

    try:
        with open(args.key_file, 'r', encoding='utf-8') as f:
            key = f.read()
        with open(args.text_file, 'r', encoding='utf-8') as f:
            text = f.read()
    except FileNotFoundError as err:
        print(f"Error: File not found - {err.filename}", file=sys.stderr)
        sys.exit(1)
    except Exception as err:
        print(f"Error reading files: {err}", file=sys.stderr)
        sys.exit(1)

    cleaned_text = clean_text(text)

    if args.encrypt:
        result = encrypt(key, cleaned_text)
    elif args.decrypt:
        result = decrypt(key, cleaned_text)

    suffix = "_encrypted" if args.encrypt else "_decrypted"
    input_path = Path(args.text_file)
    file_stem = input_path.stem
    output_filename = f"{file_stem}{suffix}.txt"

    try:
        with open(output_filename, 'w', encoding='utf-8') as f:
            f.write(result)
        sys.exit(0)
    except Exception as e:
        print(f"Error saving output file: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()