import subprocess

def read_input(file):
    inputs = []
    with open(file, 'r') as f:
        while True:
            try:
                n = int(next(f))
                matrix = [list(map(float, next(f).split())) for _ in range(n)]
                inputs.append((n, matrix))
                next(f)
            except StopIteration:
                break
    return inputs

def execute_input(input_data):
    n, matrix = input_data
    command = ['./leos22/perfSL']
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    input_str = f"{n}\n"
    for row in matrix:
        input_str += " ".join(map(str, row)) + "\n"
    stdout, stderr = process.communicate(input=input_str)
    return stdout, stderr

def main(input_file):
    inputs = read_input(input_file)
    for i, input_data in enumerate(inputs, start=1):
        print(f"---------------------------Input {i}----------------------------")
        stdout, stderr = execute_input(input_data)
        print(stdout)
        if stderr:
            print("Error:")
            print(stderr)

if __name__ == "__main__":
    input_file = "sistemas.dat"
    main(input_file)

