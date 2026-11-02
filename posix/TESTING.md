# Banker's Algorithm Testing

## Setup

I tested the C version of our Banker's Algorithm project using VS Code on Windows with GCC 16.2.0. For testing, I used five customers and started the program with four resource types, each having 10 available instances.

I compiled the program using:

`gcc -Wall -Wextra posix\main.c posix\banker.c -o banker_test.exe`

The code compiled without any warnings or errors.

## Resource Requests

I started by using the `*` command to check whether the bank was initialized correctly. All five customers showed up, the Allocation values were zero, and Need matched Maximum.

Next, I tested:

`RQ 0 3 1 2 1`

The request was approved. I ran `*` again to check the updated values:

- Available: [7 9 8 9]
- Customer 0 Allocation: [3 1 2 1]
- Customer 0 Need: [3 3 5 2]

I also tried `RQ 0 4 0 0 0`, which was denied because Customer 0 only needed 3 more instances of R0. I checked the state afterward, and nothing had changed.

## Resource Releases

After allocating resources to Customer 0, I tested:

`RL 0 1 0 1 0`

The program returned `Released`, and the updated values were:

- Available: [8 9 9 9]
- Customer 0 Allocation: [2 1 1 1]
- Customer 0 Need: [4 3 6 2]

I also tried releasing more resources than the customer had using `RL 0 3 0 0 0`. The program denied it, and the allocation stayed the same.

## Invalid Input Testing

I tried several incorrect inputs to see how the program handled them:

| Input | What happened |
|---|---|
| `RQ 5 1 0 0 0` | Invalid customer number |
| `RQ 0 -1 0 0 0` | Invalid resource request |
| `RQ 0 1 1` | Rejected missing resource values |
| `RQ 0 1 1 1 1 1` | Rejected extra resource values |
| `RQ 0 abc 1 1 1` | Rejected non-numeric input |
| `RL 0 -1 0 0 0` | Invalid resource release |
| `RL 5 1 0 0 0` | Invalid customer number |

All of these inputs were rejected without crashing the program.

## Unsafe State Testing

I wanted to make sure the program wasn't just checking whether enough resources were available, but was actually checking whether the system would remain safe.

I started a new session with `[10 10 10 10]` and entered:

`RQ 0 0 0 4 0`

This was approved.

Then I entered:

`RQ 4 0 0 6 0`

This was denied. Even though the resources were available, granting the request would have left the system in an unsafe state.

I checked the bank afterward, and Available was still `[10 10 6 10]`. Customer 0 still had `[0 0 4 0]`, and Customer 4 had no resources allocated.

## Final Notes

From the tests I ran, the request and release commands updated the bank correctly, and denied operations left the existing values unchanged. The unsafe-state test also worked as expected.

These were manual tests. The program still needs a final review with the team before submission.