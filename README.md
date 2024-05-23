# Assembly Emulator in C

This is a simple assembly emulator implemented in C. It can execute assembly-like programs defined in text files.

## How to Use

1. **Compile the Emulator**: First, compile the emulator using the following command:

    ```bash
    gcc -o emulator emulator.c
    ```

2. **Running Tests**: You can run the provided test cases using the `run_tests.sh` script. Make sure to give execute permission to the script:

    ```bash
    chmod +x run_tests.sh
    ./run_tests.sh
    ```

    This script will compile the emulator (if not already compiled) and execute each test case, comparing the output with the expected output.

3. **Writing Programs**: To write your own assembly-like programs, create a text file with the assembly instructions. Each line should represent an instruction or a label. You can use the provided test cases as examples.

4. **Executing Programs**: To execute your assembly program, use the following command:

    ```bash
    ./emulator your_program.asm
    ```

    Replace `your_program.asm` with the path to your assembly program file.

## Features

- Supports basic arithmetic operations (ADD, SUB, MUL, DIV)
- Conditional jumps (JMP, JNE, JEQ, JGT, JLT)
- Comparison (CMP) to set a comparison flag
- Register manipulation (LOAD, MOV)

## Contributing

Contributions are welcome! If you find any issues or want to add new features, feel free to submit a pull request.

## License

This project is licensed under the MIT License.
