# Future features

New ideas for features. Especially for new keywords. Feel free to make a pull request.

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

Command above selects the freshest 100 megabytes of pdf files.

## Limit by total duration

Another possible meaning of *limit*. In this scenario, the argument is a period.
Would work with things that have duration: audios and videos.

```
select '*.mp3'
  limit 74 minutes
```

I have one use case. We select some songs in order to burn them on a CD.
The disc can contain only 74 minutes of music.

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

Add boolean variables that describe certain file types: *isImage*, *isAudio*, *isVideo*, *isDocument*, *isSpreadsheet* etc.
This feature should be implemented on a low level, not by a simple extension check.
Should work for exotic and rare file types.

```
select 'c:/data/*'
  where isImage
```

We can go further and introduce collection variables: *images*, *audios*, *videos*, *archives* etc.
There would be analogous variables like: *recursiveImages*, *recursiveAudios* etc.
Maybe the best solution is to add all 3 variables for every type.
For example: *isImage*, *images* and *recursiveImages*.

```
select images
```

They would have their own properties, like duration of a video.

```
delete recursiveVideos
  where duration < 5 seconds
```

```
select directories
  where anyInside(images)
```

How to implement that? There is probably no better way than to just import *libmagic*.

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

## Common programs constants

Introduce constant values with paths to common programs.
They are constants, but not really deterministic.
Values may differ depending on where user has installed them.
If certain program is not installed, then this value is empty string.

```
open images
  with 'C:\Program Files\GIMP 2\bin\gimp-2.10.exe'
```

Instead of this, we can write simply.

```
open images
  with gimp
```

We can introduce hundreds of new constants.
Not every program can be here.
They should be rated by popularity.

## Constant value *pendrive*

This is path to the first connected pendrive. Empty string if there is nothing plugged in. In WindowsOS, this value is usually F:/, but not always.

```
copy '*.pdf'
  to pendrive
```

This feature is cool and should be implemented as soon as possible. We can even introduce a plural variant - *pendrives*.

## Order By acceleration

Perun2 has one performance weak spot. It takes few seconds to sort hundreds of thousands of files. 
It could be improved, as currently program uses a naive iterative single-threaded quicksort. 
Calculations are the bottleneck here. 
Both filesystem access and console logs are very fast. 
Sorting by a string is the slowest and should be improved in the first place.

## "Compile" time function evaluation

If certain function takes constant values as arguments, evaluate it during code interpretation.
Of course not all function are elligible for this feature.

```
copy files
  to path('a', 'b', 'c')
```

In this example, function path() is treated internally as a constant value 'a/b/c' or 'a\b\c' (depending on OS).
Just some internal optimizations. This is nothing user will notice.

## Wildcard pattern matching microoptimizations

These algorithms are used in several places. The most prominent one is the Like operator.
Current implementation is not the most optimal and microoptimizations are possible.
The algorithm is recursive and uses a vector of vectors of chars. It could be iterative.
The size of the internal vector is constant and known at runtime and depends only on the pattern length.
We could use a vector of arrays instead. Even one huge plain array of chars reserved in advance (exchange memory for performance).
However, this is not really important. Microoptimizations are possible, but they would not be noticeable anyways.

## Repositories

Variable *repositories* refers to directories that are code repositories.
Analogously, there are variables *recursiveRepositories* and *isRepository*.
If certain directory contains a directory *.git* or *.svn*, it is a repository for sure.
There are more version control systems.

```
move recursiveRepositories
  to 'c:/my projects'
```

## *LikeAny*, *LikeAll*

Basically the Like operator, but with a collection of patterns.
Brackets are optional (but recommended for readability).

```
select '*.pdf'
  where name likeAny ('a_%', '__b__', 'c%')
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

## Variable *drives*

This is a list of all current hard disc drives.

```
inside drives
{
  delete '**.pdf' where creation < 12 june 2019
}
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

```
copy '*.pdf'
  limit 3k
```

## Infix *K* within an integer literal

Allow one *K* infix to appear inside an integer literal. It multiplies preceding value by one thousand.
However, there are some requirements. It should be preceded by at least one digit, followed by one, two or three digits and no additional suffixes are allowed.
We can shorten some literals, so instead of *2023*, let us just write *2k23*.

```
select '*.pdf'
  where creation.year = 2k20
```

## Audio variables: *author*, *album*, *release*, *duration*, *bitrate*, ...

*Length* is a period. *Bitrate* is a number.

```
select '*.mp3'
  where author = 'Nirvana'
  and duration < 5 minutes
  and bitrate = 128k
```

## Variable *author*

Various different file types may have their *author*, not only audios.
For example, documents and spreadsheets.

## Video variables: of images + of audios + even more like *framerate*

Variables available for videos should be audio variables + image variables combined.
We can check both dimensions of the screen and properties of sound.
There are some special values restricted only to videos like *framerate*.

```
select videos
  where lower(name) like '%tpb%'
  and resolution = 1080p
  and duration > 10 minutes
```

## *Resolution* and some related constants

*Resolution* is an attribute of videos. Its meaning is exactly the same as of *pixels*.
Just *width* and *height* multiplied.
There can be multiple available constant values like *1080p*, *720p*, *480p* as well as *fullhd* or *ultrahd*.

```
select videos
  where resolution = ultrahd
```

## Variable *pages*

This variable is available for documents and spreadsheets. Count how many pages there are.

```
select '*.pdf'
  where pages > 30
```

## Function *durationInside()*

Next context function similar to *anyInside()* or *countInside()*. For directories/archives with audios and videos.

```
select directories
  where durationInside('*.mp3') >= 30 minutes
```

Select directories that contain at least 30 minutes of music.

```
print '*.zip'
  where anyInside('*.mp3')
  order by durationInside('*.mp3') desc
```

Find Zip archives with music. Sort them based on how much music they contain. Print the result.

## Disc properties: *totalSpace*, *usedSpace*, *freeSpace*

Get properties of disc current working location is in.

```
print 'Free space here: ' + (freeSpace / 1gb) + ' gigabytes';

inside pendrive
{
  print 'Free space on pendrive: ' + (freeSpace / 1gb) + ' gigabytes';
}
```

## Variable *lines*

Count lines in text files.

```
select '*.txt'
  where lines > 2000
```

## Variable *encoding*

Get encoding of file. This feature needs good design. This value will probably by a string.
What about case sensitivity?
Maybe implement it like *extension*, so encoding is always in lowercase.

```
select '*.txt'
  where encoding = 'utf-8'
```

Introduction of some constants like *utf8* is a good direction.

```
select '*.txt'
  where encoding = utf8
```

## *Device*

This variable refers to the first (and usually the only) external device connected to the computer like phone or camera.
Perun2 currently works only on hard disc drives. 
Other possible variables: *phone*, *camera*.
For example, we could use this feature to download photos taken today by a camera.

```
inside camera {
  copy images
    where creation = today
    to desktop
}
```

Or delete all photos older than one year. 

```
inside device {
  delete images 
    where lifetime > 1 year
}
```

## Recycle bin management

Commands for recycle bin. Maybe the only thing we can do is to resurrect files or delete them completely.
Variable *death* refers to deletion time.

```
resurrect images
  where death = today
```

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
  where version not like '1.%'
```

```
perun2
{
  print 'Your version of Perun2 is ' + version
}
```

## Variable *fonts*, *recursiveFonts*

Just font files.

```
select fonts
```
