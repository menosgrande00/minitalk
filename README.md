*This project has been created as part of the 42 curriculum by oonal.*

# Minitalk

## Description

Minitalk is a small client-server program written in C. It transfers a text
message from one process to another using only the UNIX signals `SIGUSR1` and
`SIGUSR2`.

Each byte is sent from its most significant bit to its least significant bit.
`SIGUSR1` represents `0`, `SIGUSR2` represents `1`, and the terminating null
byte marks the end of a message. The server writes completed bytes immediately.

Standard UNIX signals are not queued reliably when the same signal is already
pending. For this reason, the server sends a flow-control signal after every
received bit. The client sends the next bit only after receiving this signal.
This stop-and-wait protocol prevents corruption in long messages.

## Features

- Lossless transfer of long messages, including a tested 10,000-word payload.
- Multiple sequential clients without restarting the server.
- Strict PID and argument validation.
- A five-second acknowledgment timeout for interrupted transfers.
- No heap allocation and no Libft dependency.
- Bonus message acknowledgment and transparent UTF-8 support.

## Instructions

Build the mandatory programs:

```sh
make
```

Start the server and copy the PID it prints:

```sh
./server
```

In a second terminal, send a message:

```sh
./client <server_pid> "Hello from minitalk"
```

Build and run the bonus programs:

```sh
make bonus
./server_bonus
./client_bonus <server_pid> "Merhaba dünya 🌍"
```

The bonus client prints `Message received` after the server acknowledges the
complete message.

Available cleanup targets:

```sh
make clean
make fclean
make re
```

The message is supplied as one shell argument, as required by the subject.
Therefore, the operating system's command-line argument limit still applies.
Always quote messages that contain spaces or shell metacharacters.

## Protocol

### Mandatory

1. The client sends one data bit with `SIGUSR1` or `SIGUSR2`.
2. The server stores the bit and replies with `SIGUSR1` when ready.
3. The client waits for that reply before sending the next bit.
4. A null byte ends the message and resets the server state.

The reply is bit-level flow control. It does not provide a separate
whole-message confirmation.

### Bonus

The bonus protocol uses the same bit-level flow control. After receiving the
terminating null byte, the server replies with `SIGUSR2` instead. This is the
explicit whole-message acknowledgment required by the bonus. UTF-8 text works
because every byte is transmitted unchanged.

## Technical Choices

Each executable uses at most one global state structure. A client needs its
acknowledgment flag to be visible to both normal execution and the asynchronous
signal handler. A server needs its current byte, bit count, and active client PID
to persist between separate signal-handler calls. Grouping these related values
in one structure keeps the state explicit and respects the subject's global
variable limit.

The server normally reads the sender PID from `siginfo_t`. While a message is in
progress, it retains that verified PID as a fallback for platforms that may omit
sender metadata on a later pending standard signal. A valid signal from a new
PID resets any partial byte, so sequential clients start with clean state.

## Resources

- `man 2 sigaction` - installing handlers and masking signals.
- `man 2 kill` - sending a signal to another process.
- `man 2 write` - async-signal-safe output.
- `man 2 getpid` - retrieving the server PID.
- `man 3 usleep` - short waits used by the acknowledgment timeout loop.

AI was used to help interpret the subject, design the signal handshake, identify
race conditions, plan error cases, prepare tests, and draft documentation and
code. The generated work must be reviewed, understood, tested, and explained by
the student before submission or peer evaluation.
