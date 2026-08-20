*This project has been created as part of the 42 curriculum by randrade.*

# get_next_line

*Rank 01 — reading a file one line at a time.*

## Description

`get_next_line` returns the next line from a file descriptor, newline included, and
`NULL` when there is nothing left to read or an error occurred:

```c
char	*get_next_line(int fd);
```

Repeated calls walk through the file line by line. The catch is that `read(2)` knows
nothing about lines — it hands back a fixed-size chunk of bytes that may contain
several newlines, or none at all. So between two calls the function has to *remember*
whatever it read past the end of the line it just returned. That memory is the point
of the project: it is the cursus' introduction to **static variables**.

The buffer size is a compile-time constant supplied with `-D BUFFER_SIZE=n`. The
implementation has to behave identically whether that is `1`, `42`, or `9999`, and
whether or not the flag is passed at all (the header defaults it to `10`).

The project comes in two parts:

- **Mandatory** — one file descriptor, a single static buffer.
- **Bonus** — the same function using a single static variable, but able to interleave
  reads across many file descriptors without mixing up their positions.

Neither part may use libft, `lseek()`, or global variables.

## Instructions

### Prerequisites

- `cc`
- `make` (only for the test harness in `Main/`)

### Build

There is no library or binary to produce — the three source files are meant to be
compiled directly into whatever project consumes them:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_main.c
```

For the bonus (multiple file descriptors):

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c
```

Omitting `-D BUFFER_SIZE` is fine — the header falls back to `10`.

### Running the tests

`Main/` holds a test harness with its own Makefile and a set of fixture files —
an empty file, a single character with no newline, a file of nothing but newlines, a
file with no trailing newline, and a long text:

```sh
cd Main && make        # mandatory harness -> ./GNL
cd Main && make bonus  # bonus harness (multi-fd) -> ./GNL
```

| Target        | Effect                                              |
|---------------|-----------------------------------------------------|
| `make`        | Build the mandatory test binary `GNL`               |
| `make bonus`  | Build the bonus test binary (also named `GNL`)      |
| `make clean`  | Remove object files                                 |
| `make fclean` | `clean` + remove `GNL`                              |
| `make re`     | `fclean` then `all`                                 |

To exercise a different buffer size, pass it through: `make CCFLAGS="-Wall -Wextra -Werror -D BUFFER_SIZE=1"`.

### Usage

```c
#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
	int		fd = open("file.txt", O_RDONLY);
	char	*line;

	while ((line = get_next_line(fd)) != NULL)
	{
		/* line ends with '\n', except possibly the last one */
		free(line);
	}
	close(fd);
	return (0);
}
```

The caller owns the returned string and must `free()` it.

## Function reference

### Public

| Function | Prototype | Description |
|----------|-----------|-------------|
| `get_next_line` | `char *get_next_line(int fd);` | Returns the next line from `fd` including its trailing `\n`, or without one if the file ends mid-line. Returns `NULL` at end of file, on a read error, or on an invalid `fd`. |

### Helpers (`get_next_line_utils.c`)

| Function | Prototype | Description |
|----------|-----------|-------------|
| `ft_strlen_gnl` | `int ft_strlen_gnl(char *str);` | Length of `str` up to **and including** the first `\n`; full length if there is none; `0` for `NULL`. |
| `ft_cleanbuf_gnl` | `int ft_cleanbuf_gnl(char *str);` | Consumes the first line from the buffer: shifts anything after the first `\n` down to the front, zeroes the rest. Returns `1` if a newline was found, `0` otherwise. |
| `ft_strjoin_gnl` | `char *ft_strjoin_gnl(char *s1, char *s2);` | Concatenates `s1` with `s2` truncated at its first newline, frees `s1`, and returns the new string. |

## Project structure

```
get_next_line/
├── get_next_line.h              # prototype + BUFFER_SIZE default
├── get_next_line.c              # mandatory: one fd, one static buffer
├── get_next_line_utils.c        # mandatory helpers
├── get_next_line_bonus.h        # bonus header
├── get_next_line_bonus.c        # bonus: one static buffer per fd
├── get_next_line_utils_bonus.c  # bonus helpers (identical to the mandatory ones)
└── Main/
    ├── Makefile                 # builds either harness against ../
    ├── GNL_Main/                # mandatory test main + fixture files
    └── GNL_BONUS_Main/          # bonus test main (interleaved fds) + fixtures
```

## Implementation notes

- **The stash is a static array, not a malloc'd string.** Most implementations keep
  the leftovers in a heap-allocated buffer that has to be freed at end of file — and
  which shows up as a leak in every tester that stops reading early. Here the
  remainder lives in `static char buffer[BUFFER_SIZE + 1]`, so there is nothing to
  free and nothing to leak: the only allocation is the line handed back to the caller.
- **The loop reads as little as possible.** Each call reads one chunk, appends it to
  the line being built, and stops as soon as `ft_cleanbuf_gnl` reports a newline — the
  file is never slurped whole and then split.
- **Consuming a line is a shift, not a pointer.** `ft_cleanbuf_gnl` moves the bytes
  after the newline back to index 0 and zero-fills the tail. That keeps `buffer[0]`
  meaningful as the "is there anything stashed?" test, at the cost of an O(n) move per
  line — irrelevant at these buffer sizes.
- **The bonus indexes by file descriptor.** `static char buffer[FOPEN_MAX][BUFFER_SIZE + 1]`
  is one static variable, as the subject requires, while giving each fd its own stash.
  This is why the bonus header includes `<stdio.h>` — `FOPEN_MAX` comes from there.
  Descriptors outside `0 .. FOPEN_MAX - 1` are rejected up front, before the array is
  ever indexed.
- **The mandatory version deliberately does *not* handle several fds.** Interleaving
  two descriptors through the mandatory build will splice their contents together.
  That is within spec — multi-fd support is exactly what the bonus adds.
- **Both headers also pull in `<stdio.h>` and `<fcntl.h>`.** In the bonus that is
  load-bearing; in the mandatory header neither is needed by the implementation, only
  by the test mains.

### Known limitations

- **`FOPEN_MAX` is a conservative bound.** It is the guaranteed number of
  simultaneously open *streams*, not the maximum file descriptor a process can hold —
  that is `RLIMIT_NOFILE`, typically in the thousands. Any fd at or above that bound
  is rejected with `NULL`, which the caller cannot distinguish from a real end of
  file. Descriptors that high are rare in practice, but the ceiling is real.
- **The two `_utils` files are byte-for-byte duplicates.** The subject forces the
  `_bonus` filename suffix, so some duplication is unavoidable, but nothing in the
  helpers actually differs between the two parts.

## Resources

- `man 2 read`, `man 2 open` — the syscalls underneath
- [Static variables](https://en.wikipedia.org/wiki/Static_variable) — the concept the project is built around
- [`Get_next_line.pdf`](../../../../Subjects/Rank_1/Get_next_line.pdf) — the project subject
