# Future features

Here are ideas for new features. Especially for new keywords. 
All words of the English dictionary are already reserved for future use in syntax, so we can add almost anything.

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
Directories have four designated functions for them: *anyInside()*, *countInside()*, *existInside()*, *existsInside()*.
The first thing is to make them also work with archives.

```
select '*.zip'
  where anyInside('*.pdf')
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

## Time defined by days and months only

Right now, time can take four forms. 
The idea is to add another, fifth possibility. 

```
select '*.pdf'
  where creation = 15 july
```

In this variant, the year is not important and is not taken into account at all.

## Time defined only by hours, minutes (and seconds)

Two more possibilities for time definitions. They ignore the date.

```
select '*.pdf'
  where creation >= 9:00
    and creation <= 14:30:30
```

## Variable runtime stability check

Look for integer overflows.
These events are rare as 64 bits are used for storing ints... but they are still possible.

```
a = 5000000000000000000;
a += a; // error flows from here
a
```

## Audio variables: *author*, *album*, *release*, *duration*, *bitrate*, ...

*Duration* is a period. *Bitrate* is a number.

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

Get encoding of file. The value probably will be a string.
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

## *Device*, *phone*, *camera*

*Device* refers to the first (and usually the only) external device connected to the computer like phone or camera.
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
inside camera {
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

## Period unit conversion

By using the *as* keyword, express periods in certain unit. As period consists of only integer values, they will be trimmed if necessary.

```
images 
{
  print lifetime as days
}
```

Code below will print *888 hours*.

```
print (2 days + 5 weeks) as hours
```

The operator should have relatively low precedence.

## *Photos*

Maybe we can introduce *photos* as a subgroup of *images*.
All photos are images, but not all images are photos.

```
select photos
```

## Monochromatic images

Another boolean variable. We could apply it to both to *images* and *videos*.

```
select images
  where name like 'img%'
  and monochromatic 
```

## Time expressed as a week

We can introduce three variables - *currentWeek*, *lastWeek*, *nextWeek*.
In this case, time means a period of 7 days.

```
select files 
  where creation = currentWeek
```

There is one problem to be resolved. How to implement time shifting?

```
select files 
  where creation = currentWeek - 30 hours
```

## More time variables

For every day of the week, we can have three more values. For example: *lastMonday*, *currentMonday*, *nextMonday*. We can do the same with months.

```
select files 
  where creation = lastFriday
```

## Integration with printers

Open a popup to print something physically on the paper. 
Unfortunately, keyword *print* is already reserved. Its meaning would be ambiguous anyways.
Maybe we can use *printer* instead as below.

```
inside pendrive {
  open '*.pdf'
    where creation = today
    with printer
}
```

## Internet communication

This feature is quite controversial as of now because of security. We can imagine sending and downloading files.

```
download 'https://some/url'
download 'https://some/url' as 'data.txt'
send 'a.mp4' to 'https://some/url'
send 'a.mp4' to 'https://some/url' as 'newname.mp4'
```

## Google drive

Integration with this third-party software would be useful. Only if we agree on Internet communication first.

```
send videos
  to googleDrive '1pzschX6uMbxUblb5WZ6IleseAUE8MZ-t'
```

## Internet browsers

Keyword *browser* would mean the default Internet browser on this computer.

```
open browser
```

```
open '**/index.html'
  with browser
```

Of course we can have specific keywords like *opera*, *firefox* or *chrome*.

## Open websites

Enter certain website with default browser.

```
website 'https://some/url'
```

Or we can specify the program.

```
website 'https://some/url'
  with opera
```

## Read text context of file

Value *text* would mean the content of a file.

```
copy '*.txt'
  where creation = today
  and text like 'start%'
```

This feature should be implemented along with optimizations.
For example, we usually need only a fraction of a file to be read.

```
copy '**/*.txt'
  where text[0] = '?'
  and substring(text, 25, 3) = 'ABC'
```

## Underscore as a neutral character

Perun2 keywords are already case insensitive. 
We could allow neutral *_* within keywords, so all possible writing notations are acceptable and the user can decide what to use.
All keywords below would mean the same thing.

```
recursiveFiles
recursivefiles
RECURSIVEFILes
recursive_Files
recursive_files
RECURSIVE__FILEs
```

## Run other programming languages

Integration with Python or PowerShell would be great.
In Perun2, string literals can contain new lines.
This little feature help us a lot.

```
powershellCommand 
'
  -some -psh
  -command -here
';
```

We could also read the output.

```
pythonCommand 
'
  print("a.txt")
  print("b.txt")
';

select output
```

This command selects two files: "a.txt" and "b.txt".

## *Resembles*

The Like operator is very useful. However, is case sensitive and very strict.
The *Resembles* operator would be a softer and more forgiving variant of it.

```
select files
  where name resembles 'perun2'
```

It would expect typos and missing letters.
For example, 'perun3' and 'perun' do resemble 'perun2'. 
But not 'pep34'.
It needs some arbitrary rules.
Like, for example, 1 mistake is acceptable for every 3 characters in phrase.
