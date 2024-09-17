# Hydra
A harmless, but very annoying and persistent virus that replicates itself every time you close one of its windows, while drawing pixels in each window, and beeping, successfully eating at the GPU, CPU, and RAM of the host machine.

## WARNING ⚠️
Hydra is for educational purposes only. Do not run this program on any machine that you do not have permission to run it on. The author is not responsible for any damage caused by this program.

## Table of Contents
- [Hydra](#hydra)
  - [WARNING ⚠️](#warning-️)
  - [Table of Contents](#table-of-contents)
  - [How to run](#how-to-run)
  - [How to stop](#how-to-stop)

## How to run
1. Clone the repository
```bash
git clone https://github.com/zanderlewis/hydra.git
```
2. Change directory
```bash
cd hydra
```
3. Compile the program
```bash
gcc hydra.c -o hydra -lSDL2  # Or any other similar compiler
```
4. Run the program
```bash
./hydra
```

## How to stop
The only way to stop the virus is to kill the process. This can be done by opening the task manager and ending the process, or by running the following command in the terminal:
```bash
killall hydra
```
