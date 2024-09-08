# Task:

Using the structures and algorithms available in the Boost library, develop a utility for detecting duplicate files. The utility should be able to accept command-line parameters to specify:

* Directories to scan (there may be several).

* Directories to exclude from scanning (there may be several).

* Scanning depth (one level for all directories, 0 - only the specified directory without subdirectories).

* Minimum file size; by default, all files larger than 1 byte are checked.

* Filename masks allowed for comparison (case-insensitive).

* The block size used for reading files, referred to as S in the task.

* One of the existing hashing algorithms (crc32, md5 - determine specific variants yourself), referred to as H in the task.

The result of the utility should be a list of full file paths with identical content, printed to standard output. Each file should be listed on a separate line. Identical files should appear consecutively, as a group. Different groups are separated by an empty line.

An essential feature of the utility is its careful handling of disk I/O. Each file can be represented as a list of blocks of size S. If the file size is not a multiple of S, it should be padded with binary zeros.

For example, the file world.txt containing one line: `Hello, World\n`

With a block size of 5 bytes, it would be represented as:
```
Hello
, Wor
ld\n\0\0
```
Each block should be hashed using the chosen hashing function. Possible collisions are ignored. Assuming:
```
H("Hello") == A
H(", Wor") == B
H("ld\n\0\0") == C
```
Our file `world.txt` can be represented as the sequence `ABC`.
Now consider the second file `cpp.txt: Hello, C++\n`
After hashing the blocks:
```
H("Hello") == A
H(", C++") == D
H("\n\0\0\0\0") == E
```

It can be represented as the sequence ADE.
The file comparison process should be as efficient as possible. Processing the first file world.txt should not involve reading from disk until there is something to compare it with. Once we reach cpp.txt, only then is the first block of both files read. In this case, the blocks are identical, so the second blocks are read, which differ. The files are not identical, and the remaining data is not read. Files are considered identical if the entire sequence of hash blocks matches.
# Self-check:
* Each file block is read from disk no more than once.
* A file block is read only when necessary.
* Don't forget that there may be more than two duplicates.
* The bayan package containing the executable file bayan is published on Bintray.
* Description of parameters in the README.md file in the root of the repository.
* A link to the repository page is submitted for review.

# Verification:
The task is considered successfully completed if, after reviewing the code, connecting the repository, installing the package, and running the binary file with the command (parameters from the description):
`$ bayan [...]`
duplicate files are detected without false positives or missed duplicates. The amount of data read from disk should be minimal.
