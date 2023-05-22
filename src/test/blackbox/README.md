# Black-box tests

Here are the main tests for Perun2.
They take a compiled executable file *perun2.exe*, run certain Perun2 code with it and compare the result (output + exit code) with the expectations.

## How to run them?

Just go back the *src* directory and run *_runTests.peru* with Perun2.

## Remarks

Tests should be platform-independent. For example, path separator may differ between operating systems and be / or \\.
If certain test requires it, use function *path()* or the constant value *SEPARATOR*.
Another rule goes for case size of file names. Just use lowercase every time they are involved.
If the output of a command is a collection of files, their order may vary between environments.
They should be ordered just in case.
Directory *res* contains test data.
If you want to introduce more files for tests, create a new subdirectory inside it (like *res/newdir*).
File time attributes (creation time etc.) of files in directory *res* should not be trusted.
Creation and modification time may refer to the repository clone time.
