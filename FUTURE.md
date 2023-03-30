# Future features

New ideas for features (especially for new keywords). Here should be as many creative ideas as possible.

## Limit by total size

Currently, keyword *limit* works the same as in SQL. We impose limitation on the quantity of elements in a collection.

```
select '*.pdf'
  order by creation desc
  limit 100
```

The idea is to give it another meaning. If the argument is a file size unit, then the limitation takes form of the total size of files and directories.

```
select '*.pdf'
  order by creation desc
  limit 100mb
```

## Archive files

See through the compressed archive files as if they are transparent.
Making operations on compressed files may be not possible.
However, this is not really important. Let us focus on just reading them.
Right now there are four functions for directories: *anyInside()*, *countInside()*, *existInside()*, *existsInside()*.
The first objective is to make them also work with archives.

```
select '*.zip'
  where anyInside('*.pdf' where creation = today)
```

## Function *sizeInside()*

Another context function for directories. Works similar to *anyInside()* or *countInside()*.

```
select directories
  where sizeInside('*.txt') >= 100mb
```

Select directories that contain at least 100 megabytes of text files.

## General file types

Add boolean variables that describe certain file type: *isImage*, *isAudio*, *isVideo*, *isDocument*, *isSpreadsheet* etc.
This feature should be implemented on a low level, not by a simple extension check.
Should work for exotic and rare file encodings.

```
select files
  where isImage
```

Another approach is to introduce collection variables: *images*, *audios*, *videos*, *archives* etc.
There would be analogous variables like: *recursiveImages*, *recursiveAudios* etc.
Maybe the best solution is to add all 3 variables for every type.
For example: *isImage*, *images* and *recursiveImages*.

```
select images
```

They would have their own properties, like length of a video.

```
delete recursiveVideos
  where length < 5 seconds
```

## Image variables: *width*, *height*, *pixels*, *bpp*

Variables that describe images. All four above are numbers.
*Pixels* equals *width* multiplied by *height*.
*Bpp* means bits per pixel.

```
select images
  where width = 1920
  and height = 1080
  and bpp = 24
```

## "Compile" time function evaluation

If certain function takes constant values as arguments, evaluate it during code interpretation.
Of course not all function are elligible for this feature.

```
copy files
  to path('a', 'b', 'c')
```

In this example, function path() is turned into a constant value 'a/b/c' or 'a\b\c' (depending on OS).

## Repositories

Variable *repositories* refers to directories that are code repositories.
Analogously, there are variables *recursiveRepositories* and *isRepository*.
If certain directory contains a directory *.git* or *.svn*, it is a repository for sure.
There are more version control systems.

## *LikeAny*, *LikeAll*

Basically the Like operator, but with a collection of patterns.

```
select '*.pdf'
  where name likeAny ('a_%', 'br[567]%', '%[a-z]__th%')
```

## Double dot ..

Access parent directory. What a shame that something so fundamental is not implemented in Perun2 yet.

```
copy files
  to '../some/path'
```

Should work with Asterisk Patterns as well.

```
select '../../**.txt'
```

## Other logic gates: *nand*, *nor*, *xnor*

These gates are used quite rarely, but they still can be useful.

```
select directories
  where empty nor hidden
```

## Variable *yesteryear*

This is current year minus one.

```
select files
  where creation.year = yesteryear
```

## Time defined by day and month only

Right now, time can take four forms. 
The idea is to add another possibility.

```
select '*.pdf'
  where creation = 15 july
```

This feature looks simple to implement. 
However, there is plenty of problems to solve.
How to compare "15 july" and "june 2020"?
What about subtraction operation "15 april - june 2020"?

## Variable runtime stability check

Look for integer overflows. In case of one, throw runtime exception.
These events are rare as 64 bits are used for storing ints... but they are still possible.

```
a = 5000000000000000000;
a += a; // error flows from here
a
```

## Variable *nothing*

*Nothing* is an empty collection of strings. Why add this feature? Because why not?
Also variable *never* would be an empty list of times.

```
print nothing
```

## Number suffix *K*

Let *K* mean one thousand. So, *15k* written in code means *15000*.
How should then one million be expressed? *KK* or *M*?

## Audio variables: *author*, *album*, *release*, *length*, *bitrate*, ...

*Length* is a period. *Bitrate* is a number.

```
select '*.mp3'
  where author = 'Nirvana'
  and length < 5 minutes
  and bitrate = 128k
```

## Variable *author*

Various different file types may have their *author*, not only audios.
For example, documents and spreadsheets.

## Video variables: of images + of audios + even more like *framerate*

Variables available for videos should be audio variables + image variables combined.
We can check both dimensions of the screen and properties of sound.
There are some special values restricted only to videos like *framerate*.

## Variable *pages*

This variable is available for documents and spreadsheets. Count how many pages there are.

```
select '*.pdf'
  where pages > 30
```

## Variable *lines*

Count lines in text files.

```
select '*.txt'
  where lines > 2000
```

## Variable *encoding*

Get encoding of file. This feature needs good design. What about case sensitivity?
Maybe implement it like *extension*, so encoding is always in lowercase.

```
select '*.txt'
  where encoding = 'utf-8'
```

## *Device*

This variable refers to the first (and usually the only) external device connected to the computer like phone or camera.
Perun2 currently works only on hard disc drives.
For example, we could use this feature to download photos taken today by a camera.

```
inside device {
  copy images
    where creation = today
    to origin
}
```

```
inside device {
  delete images 
    where lifetime > 1 year
}
```

Other possible variables: *phone*, *camera*. There is a possibility to express paths to external devices the following way.


```
open 'device:/some/path/*.txt'
```

## Commands *read* and *write*

Command *write* saves output to a file. The values are the same that would appear in log, separated by new lines.

```
write '*.pdf'
  to 'data.txt'
```

We can also write absolute paths to files.

```
'*.pdf' {
  write path to 'data.txt'
}
```

How would *read* work? 

```
a = read('data.txt')
```

## Command *show* or *message*

Show a popup window with text.

## Command *website*

This command takes one string argument (website URL) and opens it using the default web browser.
Probably a pointless command. 
At this point, Perun2 is so huge and bloated, that this little feature will not make it worse.

```
website 'google'
```

*Https* as default protocol and *com* as default domain (if these values are missing). 
Command above will open *https://google.com*.

## Command *clear*

This command makes files and directories empty. How to implement that? Can anything be cleared?

```
clear '**.txt'
```

## Variable *programs*, *recursivePrograms*

This variable refers to all executable files. Maybe some special properties for them: *version*, *description*.
Boolean variable *signed* that checks file signature.

```
delete programs
  where version != '1.0.6'
```

## Variable *fonts*, *recursiveFonts*

Just font files.

```
select fonts
```

## Variable *scripts*, *recursiveScripts*

Perun2 script files.

## Variable *downloaded*

This is absolute path to the special directory, where files downloaded from the Internet usually land.

