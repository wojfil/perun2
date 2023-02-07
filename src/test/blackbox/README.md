# Black-box tests

Here are the most important tests for Uroboros2.
All they do is take a compiled executable file *uro.exe*, run certain Uroboros2 code with it and compare the result (output + exit code) with the expectations.
They test the entire process of code interpretation, as one failure somewhere in the middle would alter the final output.

## Remarks

Tests should be platform independent. For example, path separator may differ between operating systems and be / or \.
If certain test requires it, use function *path()* or the constant value *SEPARATOR*.
The same goes for case size of file names - just use lowercase every time they are involved.
If the output is a collection of files, their order may vary between environments.
They should be ordered just in case.
Directory *res* contains test data.
If you want to introduce more files for tests, create a new subdirectory inside it (like *res/newdir*).
File time attributes (creation time etc.) of files in directory *res* should not be trusted.
Creation and modification time may refer to the repository clone time.
